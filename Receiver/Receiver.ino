#include <Wire.h>
#include <SPI.h>
#include <RF24.h>
#include <Adafruit_SSD1306.h>
#include "BluetoothSerial.h"
#include <math.h>

// ---------------- BLUETOOTH ----------------
BluetoothSerial SerialBT;

// ---------------- NRF ----------------
RF24 radio(4, 5);
const byte address[6] = "CANS1";

// ---------------- OLED ----------------
Adafruit_SSD1306 display(128, 64, &Wire, -1);

// ---------------- LED ----------------
#define LED_PIN 2

// ---------------- DATA STRUCT ----------------
struct DataPacket {
  float temp;
  float hum;
  float pressure;
  float ax;
  float ay;
  float az;
  float lat;
  float lng;
};

DataPacket data;

// ---------------- ALTITUDE ----------------
float altitude(float p) {
  return 44330.0 * (1.0 - pow(p / 1013.25, 0.1903));
}

void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);

  // OLED INIT
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED FAILED");
    while (1);
  }

  display.clearDisplay();

  // BLUETOOTH
  SerialBT.begin("CANSAT_GROUND");

  // NRF
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_HIGH);
  radio.startListening();

  Serial.println("GROUND STATION READY");
}

void loop() {

  if (radio.available()) {

    radio.read(&data, sizeof(data));

    float alt = altitude(data.pressure);

    float accelMag = sqrt(data.ax * data.ax +
                          data.ay * data.ay +
                          data.az * data.az);

    // ---------------- TURBULENCE LED ----------------
    if (accelMag > 12.0) {
      digitalWrite(LED_PIN, HIGH);
    } else {
      digitalWrite(LED_PIN, LOW);
    }

    // ---------------- GOOGLE MAP LINK ----------------
    String mapLink = "https://www.google.com/maps?q=" +
                     String(data.lat, 6) + "," +
                     String(data.lng, 6);

    // ---------------- BLUETOOTH OUTPUT ----------------
    SerialBT.println("\n📡 CANSAT GROUND STATION");

    SerialBT.println("Temp: " + String(data.temp));
    SerialBT.println("Humidity: " + String(data.hum));
    SerialBT.println("Pressure: " + String(data.pressure));
    SerialBT.println("Altitude: " + String(alt));

    SerialBT.println("AX: " + String(data.ax));
    SerialBT.println("AY: " + String(data.ay));
    SerialBT.println("AZ: " + String(data.az));

    SerialBT.println("🌍 LIVE LOCATION:");
    SerialBT.println(mapLink);
    SerialBT.println("----------------------");

    // ---------------- SERIAL DEBUG ----------------
    Serial.println("DATA RECEIVED OK");

    // ---------------- OLED DISPLAY ----------------
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);

    display.setCursor(0, 0);
    display.print("T:"); display.print(data.temp);

    display.setCursor(0, 10);
    display.print("H:"); display.print(data.hum);

    display.setCursor(0, 20);
    display.print("P:"); display.print(data.pressure);

    display.setCursor(0, 30);
    display.print("ALT:"); display.print(alt);

    display.setCursor(0, 42);
    display.print("AX:"); display.print(data.ax);
    display.print(" AY:"); display.print(data.ay);

    display.setCursor(0, 54);
    display.print("AZ:"); display.print(data.az);

    display.display();
  }
}