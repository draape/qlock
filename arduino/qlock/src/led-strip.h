#ifndef LED_STRIP_H
#define LED_STRIP_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

enum class LEDPattern
{
  OFF,
  PULSING_YELLOW, // WiFi connecting
  PULSING_BLUE,   // Time syncing
  PULSING_WHITE,  // Non-critical warning (time refresh failed)
  BLINKING_RED,   // Critical error
  SOLID_GREEN,    // Success (temporary)
  SOLID_WHITE     // Connection successful (stays on)
};

class LedStrip
{
public:
  LedStrip(int pin, int numPixels, int statusPixelIndex = 0);

  // Status LED methods
  void begin();
  void setPattern(LEDPattern pattern);
  void update(); // Call this in loop() for non-blocking patterns

  // Word clock display methods
  void clear();                                                                   // Turn off all pixels
  void show();                                                                    // Update the strip
  void setPixel(int index, uint32_t color);                                       // Set single pixel
  void setPixel(int index, uint8_t r, uint8_t g, uint8_t b);                      // Set single pixel with RGB
  void setPixels(const int *indices, int count, uint32_t color);                  // Set multiple pixels
  void setPixels(const int *indices, int count, uint8_t r, uint8_t g, uint8_t b); // Set multiple pixels with RGB

  // Color helper methods
  uint32_t Color(uint8_t r, uint8_t g, uint8_t b) { return strip_.Color(r, g, b); }
  uint32_t White(uint8_t brightness = 255) { return strip_.Color(brightness, brightness, brightness); }

  // Access to the strip for advanced usage
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
  bool isStatusPixel(int index) const { return index == statusPixelIndex_; }
};

#endif