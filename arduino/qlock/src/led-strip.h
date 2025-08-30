#ifndef WS2812_STATUS_H
#define WS2812_STATUS_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

enum class LEDPattern
{
  OFF,
  PULSING_YELLOW, // WiFi connecting
  PULSING_BLUE,   // Time syncing
  PULSING_WHITE,  // Non-critical warning (time refresh failed)
  BLINKING_RED,   // Critical error
  SOLID_GREEN     // Success
};

class LedStrip
{
public:
  LedStrip(int pin, int numPixels, int statusPixelIndex = 0);

  void begin();
  void setPattern(LEDPattern pattern);
  void update(); // Call this in loop() for non-blocking patterns
  void clear();  // Turn off all pixels
  void show();   // Update the strip

  // Access to the strip for your word clock logic
  Adafruit_NeoPixel &getStrip() { return strip_; }

private:
  Adafruit_NeoPixel strip_;
  int statusPixelIndex_; // Which pixel to use for status indication
  LEDPattern currentPattern_;
  unsigned long lastUpdate_;
  int pulseValue_;
  int pulseDirection_;
  bool blinkState_;

  void updatePulse();
  void updateBlink();
  uint32_t colorFromPattern(LEDPattern pattern, int brightness = 255);
};

#endif