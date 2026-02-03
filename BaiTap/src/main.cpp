#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DHTesp.h"

// ================== PIN ==================
#define LED_GREEN   15
#define LED_YELLOW  2
#define LED_RED     4

#define DHT_PIN     16

#define OLED_SDA    13
#define OLED_SCL    12

// ================== OLED ==================
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ================== DHT ==================
DHTesp dht;

// ================== TIMER ==================
unsigned long lastDHTRead = 0;
unsigned long lastBlink = 0;

const unsigned long DHT_INTERVAL = 2000;   // 2s
const unsigned long BLINK_INTERVAL = 1000; // 1s

bool ledState = false;

// ================== DATA ==================
float temperature = 0;
float humidity = 0;

String statusText = "";
int activeLED = -1;

// ================== SETUP ==================
void setup() {
  Serial.begin(115200);

  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_RED, OUTPUT);

  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_YELLOW, LOW);
  digitalWrite(LED_RED, LOW);

  // OLED
  Wire.begin(OLED_SDA, OLED_SCL);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();

  // DHT
  dht.setup(DHT_PIN, DHTesp::DHT22);
}

// ================== LED CONTROL ==================
void updateLEDLogic() {
  if (temperature < 13) {
    statusText = "TOO COLD";
    activeLED = LED_GREEN;
  }
  else if (temperature < 20) {
    statusText = "COLD";
    activeLED = LED_GREEN;
  }
  else if (temperature < 25) {
    statusText = "COOL";
    activeLED = LED_YELLOW;
  }
  else if (temperature < 30) {
    statusText = "WARM";
    activeLED = LED_YELLOW;
  }
  else if (temperature < 35) {
    statusText = "HOT";
    activeLED = LED_RED;
  }
  else {
    statusText = "TOO HOT";
    activeLED = LED_RED;
  }
}

// ================== OLED ==================
void updateOLED() {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Temperature - HOT");

  display.setTextSize(2);
  display.setCursor(0, 14);
  display.print(temperature, 1);
  display.println(" C");

  display.setTextSize(1);
  display.setCursor(0, 40);
  display.print("Humidity: ");
  display.print(humidity, 1);
  display.println(" %");

  display.setCursor(0, 54);
  display.print("Status: ");
  display.print(statusText);

  display.display();
}

// ================== LOOP ==================
void loop() {
  unsigned long now = millis();

  // ===== READ DHT (2s) =====
  if (now - lastDHTRead >= DHT_INTERVAL) {
    lastDHTRead = now;

    TempAndHumidity data = dht.getTempAndHumidity();
    temperature = data.temperature;
    humidity = data.humidity;

    updateLEDLogic();
    updateOLED();

    Serial.printf("Temp: %.1f | Humi: %.1f | %s \r\n", temperature, humidity, statusText.c_str());
  }

  // ===== BLINK LED (1s) =====
  if (now - lastBlink >= BLINK_INTERVAL) {
    lastBlink = now;
    ledState = !ledState;

    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_RED, LOW);

    if (activeLED != -1 && ledState) {
      digitalWrite(activeLED, HIGH);
    }
  }
}
