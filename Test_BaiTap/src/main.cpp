#include "main.h"
#include "ultils.h"

// ===== OBJECT =====
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
DHTesp dht;

// ===== DATA =====
float temperature = 0;
float humidity = 0;
String statusText = "";
int activeLED = -1;

// ===== INIT =====
void systemInit() {
  Serial.begin(115200);

  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_RED, OUTPUT);

  Wire.begin(OLED_SDA, OLED_SCL);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();

  dht.setup(DHT_PIN, DHTesp::DHT22);
}

// ===== LOOP =====
void systemLoop() {
  unsigned long now = millis();

  readDHTTask(now);
  ledBlinkTask(now);
  oledTask();
}

// ===== ARDUINO =====
void setup() {
  systemInit();
}

void loop() {
  systemLoop();
}
