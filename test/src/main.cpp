#include <Arduino.h>

// Khai báo chân LED RGB
#define RED_PIN   18
#define GREEN_PIN 19
#define BLUE_PIN  23

// Hàm điều khiển RGB
void rgb(int r, int g, int b) {
  digitalWrite(RED_PIN,   r ? HIGH : LOW);
  digitalWrite(GREEN_PIN, g ? HIGH : LOW);
  digitalWrite(BLUE_PIN,  b ? HIGH : LOW);
}

void setup() {
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
}

void loop() {
  rgb(1, 0, 0); // Red
  delay(1000);

  rgb(0, 1, 0); // Green
  delay(1000);

  rgb(0, 0, 1); // Blue
  delay(1000);

  rgb(1, 1, 0); // Yellow
  delay(1000);

  rgb(0, 1, 1); // Cyan
  delay(1000);

  rgb(1, 0, 1); // Magenta
  delay(1000);

  rgb(1, 1, 1); // White
  delay(1000);

  rgb(0, 0, 0); // OFF
  delay(1000);
}
