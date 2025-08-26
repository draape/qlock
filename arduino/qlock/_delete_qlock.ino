#include <Adafruit_NeoPixel.h>

#define LED_PIN    D1        // GPIO5 on Wemos D1 mini
#define LED_COUNT  20        // total number of LEDs on your strip

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.show();              // Initialize all pixels to 'off'
  strip.setBrightness(20);   // Brightness (0-255), 50 is safe for testing

  pinMode(LED_BUILTIN, OUTPUT); // Setup onboard LED
}

void loop() {
  // Turn ON the first 10 LEDs to white
  for (int i = 0; i < 10; i++) {
    strip.setPixelColor(i, strip.Color(0, 255, 0)); // white
  }
  strip.show();

  // Blink onboard LED so you know code is running
  digitalWrite(LED_BUILTIN, LOW);   // ON
  delay(500);
  digitalWrite(LED_BUILTIN, HIGH);  // OFF
  delay(500);
}