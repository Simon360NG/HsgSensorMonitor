
#include <Wire.h>
#include "SSD1306Wire.h"        
#include "images.h"

SSD1306Wire display(0x3c, D6, D5);

#define DEMO_DURATION 3000
typedef void (*Demo)(void);

int demoMode = 0;
int counter = 1;

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();

  display.init();

  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);

    delay(2000); // Pause for 2 seconds

  display.clear();

}

void drawData() {
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 0, "Hello World");
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 13, "Steve woz 'ere");
}

void loop() {
  display.clear();

  drawData();

  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_RIGHT);
  display.drawString(128, 54, String(millis()));
  display.display();

  delay(1000);
}
