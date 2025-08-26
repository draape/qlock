#ifndef WS2812_STATUS_H
#define WS2812_STATUS_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

enum class LEDPattern
{
  OFF,
  PULSING_YELLOW, // WiFi connecting
  PULSING_BLUE,   // Time syncing
  BLINKING_RED,   // Error
  SOLID_GREEN     // All good
};

class WS2812Status
{
public:
  WS2812Status(int pin, int numPixels, int statusPixelIndex = 0);

  void begin();
  void setPattern(LEDPattern pattern);
  void update();
  void clear();
  void show();

  Adafruit_NeoPixel &getStrip() { return strip_; }

private:
  Adafruit_NeoPixel strip_;
  int statusPixelIndex_;
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