#include <Wire.h>
#include <SPI.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <RF24.h>
#include <DHT.h>
#include <Adafruit_BMP085.h>
#include <Adafruit_ADXL345_U.h>

// ---------------- WIFI ----------------
const char* ssid = "YOUR_WIFI";
const char* password = "YOUR_PASSWORD";

// ---------------- THINGSPEAK ----------------
String apiKey = "YOUR_THINGSPEAK_KEY";
const char* server = "http://api.thingspeak.com/update";

// ---------------- NRF ----------------
RF24 radio(4, 5);
const byte address[6] = "CANS1";

// ---------------- SENSORS ----------------
#define DHTPIN 15
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

Adafruit_BMP085 bmp;
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

// ---------------- DATA ----------------
struct DataPacket {
  float temp;
  float hum;
  float pressure;
  float ax;
  float ay;
  float az;
};

DataPacket data;

unsigned long lastUpload = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("\n===== TRANSMITTER STARTED =====");

  // ---------------- WIFI (NON-BLOCKING FIX) ----------------
  WiFi.begin(ssid, password);
  Serial.print("Connecting WiFi");

  int timeout = 0;
  while (WiFi.status() != WL_CONNECTED && timeout < 20) {
    delay(500);
    Serial.print(".");
    timeout++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi Connected");
  } else {
    Serial.println("\nWiFi FAILED (continuing offline)");
  }

  // ---------------- SENSORS ----------------
  dht.begin();
  Serial.println("DHT OK");

  if (!bmp.begin())
    Serial.println("BMP180 FAILED");
  else
    Serial.println("BMP180 OK");

  if (!accel.begin())
    Serial.println("ADXL345 FAILED");
  else
    Serial.println("ADXL345 OK");

  // ---------------- NRF ----------------
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_HIGH);
  radio.stopListening();

  Serial.println("NRF READY");
}

void loop() {

  data.temp = dht.readTemperature();
  data.hum = dht.readHumidity();
  data.pressure = bmp.readPressure() / 100.0;

  sensors_event_t event;
  accel.getEvent(&event);

  data.ax = event.acceleration.x;
  data.ay = event.acceleration.y;
  data.az = event.acceleration.z;

  // ---------------- SERIAL MONITOR OUTPUT ----------------
  Serial.println("\n----- SENSOR DATA -----");
  Serial.print("Temp: "); Serial.println(data.temp);
  Serial.print("Hum: "); Serial.println(data.hum);
  Serial.print("Pressure: "); Serial.println(data.pressure);
  Serial.print("AX: "); Serial.println(data.ax);
  Serial.print("AY: "); Serial.println(data.ay);
  Serial.print("AZ: "); Serial.println(data.az);

  // ---------------- NRF SEND ----------------
  bool ok = radio.write(&data, sizeof(data));

  if (ok)
    Serial.println("NRF SEND OK");
  else
    Serial.println("NRF SEND FAIL");

  // ---------------- THINGSPEAK ----------------
  if (millis() - lastUpload > 15000) {

    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;

      String url = String(server) +
        "?api_key=" + apiKey +
        "&field1=" + String(data.temp) +
        "&field2=" + String(data.hum) +
        "&field3=" + String(data.pressure) +
        "&field4=" + String(data.ax) +
        "&field5=" + String(data.ay) +
        "&field6=" + String(data.az);

      http.begin(url);
      int code = http.GET();
      http.end();

      Serial.print("ThingSpeak Code: ");
      Serial.println(code);
    }

    lastUpload = millis();
  }

  delay(1000);
}