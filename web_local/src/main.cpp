#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <LittleFS.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include "DHT.h"

// WiFi
const char* ssid = "LAB502-09 5785";
const char* password = "123456789";

// Web server
AsyncWebServer server(80);

// DHT22
#define DHTPIN D4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// LED
#define LED_PIN D2
bool ledState = false;

// Sensor data
float temp = 0;
float hum = 0;

void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  dht.begin();

  // SPIFFS
  if (!LittleFS.begin()) {
    Serial.println("SPIFFS lỗi");
    return;
  }

  // WiFi
  WiFi.begin(ssid, password);
  Serial.print("Đang kết nối WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nĐã kết nối!");
  Serial.println(WiFi.localIP());

  // Load web
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/index.html", "text/html");
  });

  server.serveStatic("/", LittleFS, "/");

  // Toggle LED
  server.on("/toggle", HTTP_GET, [](AsyncWebServerRequest *request) {
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState);
    request->send(200, "text/plain", "OK");
  });

  // API data
  server.on("/data", HTTP_GET, [](AsyncWebServerRequest *request) {
    StaticJsonDocument<200> doc;
    doc["temp"] = temp;
    doc["hum"] = hum;

    String json;
    serializeJson(doc, json);

    request->send(200, "application/json", json);
  });

  server.begin();
}

void loop() {
  // đọc DHT22
  float t = dht.readTemperature();
  float h = dht.readHumidity();

  if (!isnan(t) && !isnan(h)) {
    temp = t;
    hum = h;
  }

  delay(2000);
}