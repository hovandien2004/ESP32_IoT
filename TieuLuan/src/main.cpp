#include <WiFi.h>
#include <WebServer.h>
#include "DHT.h"

#define DHTPIN 16
#define DHTTYPE DHT22
#define LED_PIN 23

const char* ssid = "Wokwi-GUEST";
const char* password = "";

WebServer server(80);
DHT dht(DHTPIN, DHTTYPE);

bool ledState = false;

// ===== DASHBOARD =====
String webpage = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>ESP32 Dashboard</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body {
      font-family: Arial;
      background: #0f172a;
      color: white;
      text-align: center;
    }
    .card {
      background: #1e293b;
      padding: 20px;
      margin: 10px;
      border-radius: 15px;
      display: inline-block;
      width: 220px;
    }
    .value {
      font-size: 28px;
      font-weight: bold;
    }
    button {
      padding: 15px 30px;
      font-size: 18px;
      border: none;
      border-radius: 10px;
      background: #22c55e;
      color: white;
      cursor: pointer;
    }
    button.off {
      background: #ef4444;
    }
  </style>
</head>
<body>

<h1>📡 ESP32 IoT Dashboard</h1>

<div class="card">
  <p>🌡 Temperature</p>
  <div id="temp" class="value">--</div>
</div>

<div class="card">
  <p>💧 Humidity</p>
  <div id="hum" class="value">--</div>
</div>

<div class="card">
  <p>💡 LED</p>
  <button id="btn" onclick="toggle()">---</button>
</div>

<script>
function toggle(){
  fetch('/toggle');
}

setInterval(()=>{
  fetch('/data')
  .then(res=>res.json())
  .then(data=>{
    document.getElementById("temp").innerText = data.temp + " °C";
    document.getElementById("hum").innerText = data.hum + " %";

    let btn = document.getElementById("btn");
    if(data.led){
      btn.classList.remove("off");
      btn.innerText = "LED ON";
    } else {
      btn.classList.add("off");
      btn.innerText = "LED OFF";
    }
  });
},2000);
</script>

</body>
</html>
)rawliteral";

// ===== HANDLER =====
void handleRoot() {
  server.send(200, "text/html", webpage);
}

void handleToggle() {
  ledState = !ledState;
  digitalWrite(LED_PIN, ledState);
  server.send(200, "text/plain", "OK");
}

void handleData() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();

  String json = "{";
  json += "\"temp\":" + String(t) + ",";
  json += "\"hum\":" + String(h) + ",";
  json += "\"led\":" + String(ledState ? "true" : "false");
  json += "}";

  server.send(200, "application/json", json);
}

// ===== SETUP =====
void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  dht.begin();

  WiFi.begin(ssid, password,6);
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }

  Serial.println("\nConnected!");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/toggle", handleToggle);
  server.on("/data", handleData);

  server.begin();
}

void loop() {
  server.handleClient();
}