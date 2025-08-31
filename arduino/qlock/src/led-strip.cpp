#include "led-strip.h"
#include <Arduino.h>

LedStrip::LedStrip(int pin, int numPixels, int statusPixelIndex)
    : strip_(numPixels, pin, NEO_GRB + NEO_KHZ800),
      statusPixelIndex_(statusPixelIndex),
      currentPattern_(LEDPattern::OFF),
      lastUpdate_(0),
      pulseValue_(0),
      pulseDirection_(1),
      blinkState_(false)
{
}

void LedStrip::begin()
{
  strip_.begin();
  strip_.clear();
  strip_.show();
}

void LedStrip::setPattern(LEDPattern pattern)
{
  if (currentPattern_ != pattern)
  {
    currentPattern_ = pattern;
    lastUpdate_ = millis();
    pulseValue_ = 0;
    pulseDirection_ = 1;
    blinkState_ = false;

    // Handle immediate patterns
    switch (pattern)
    {
    case LEDPattern::OFF:
      strip_.setPixelColor(statusPixelIndex_, 0);
      strip_.show();
      break;
    case LEDPattern::SOLID_GREEN:
    case LEDPattern::SOLID_WHITE:
      strip_.setPixelColor(statusPixelIndex_, colorFromPattern(pattern));
      strip_.show();
      break;
    default:
      // Animated patterns handled in update()
      break;
    }
  }
}

void LedStrip::update()
{
  switch (currentPattern_)
  {
  case LEDPattern::PULSING_YELLOW:
  case LEDPattern::PULSING_BLUE:
  case LEDPattern::PULSING_WHITE:
    updatePulse();
    break;

  case LEDPattern::BLINKING_RED:
    updateBlink();
    break;

  default:
    // Static patterns don't need updates
    break;
  }
}

void LedStrip::updatePulse()
{
  unsigned long currentTime = millis();

  if (currentTime - lastUpdate_ > 20)
  { // Update every 20ms for smooth pulse
    lastUpdate_ = currentTime;

    pulseValue_ += pulseDirection_ * 5;

    if (pulseValue_ >= 255)
    {
      pulseValue_ = 255;
      pulseDirection_ = -1;
    }
    else if (pulseValue_ <= 0)
    {
      pulseValue_ = 0;
      pulseDirection_ = 1;
    }

    uint32_t color = colorFromPattern(currentPattern_, pulseValue_);
    strip_.setPixelColor(statusPixelIndex_, color);
    strip_.show();
  }
}

void LedStrip::updateBlink()
{
  unsigned long currentTime = millis();

  if (currentTime - lastUpdate_ > 300)
  { // Blink every 300ms
    lastUpdate_ = currentTime;
    blinkState_ = !blinkState_;

    if (blinkState_)
    {
      strip_.setPixelColor(statusPixelIndex_, colorFromPattern(currentPattern_));
    }
    else
    {
      strip_.setPixelColor(statusPixelIndex_, 0);
    }
    strip_.show();
  }
}

void LedStrip::clear()
{
  strip_.clear();
  // Restore status LED if it should be on
  if (currentPattern_ == LEDPattern::SOLID_WHITE || currentPattern_ == LEDPattern::SOLID_GREEN)
  {
    strip_.setPixelColor(statusPixelIndex_, colorFromPattern(currentPattern_));
  }
}

void LedStrip::show()
{
  strip_.show();
}

void LedStrip::setPixel(int index, uint32_t color)
{
  // Don't overwrite the status pixel unless explicitly intended
  if (!isStatusPixel(index))
  {
    strip_.setPixelColor(index, color);
  }
}

void LedStrip::setPixel(int index, uint8_t r, uint8_t g, uint8_t b)
{
  setPixel(index, strip_.Color(r, g, b));
}

void LedStrip::setPixels(const int *indices, int count, uint32_t color)
{
  for (int i = 0; i < count; i++)
  {
    setPixel(indices[i], color);
  }
}

void LedStrip::setPixels(const int *indices, int count, uint8_t r, uint8_t g, uint8_t b)
{
  uint32_t color = strip_.Color(r, g, b);
  setPixels(indices, count, color);
}

uint32_t LedStrip::colorFromPattern(LEDPattern pattern, int brightness)
{
  switch (pattern)
  {
  case LEDPattern::PULSING_YELLOW:
    return strip_.Color((brightness * 255) / 255, (brightness * 200) / 255, 0); // Yellow
  case LEDPattern::PULSING_BLUE:
    return strip_.Color(0, 0, brightness); // Blue
  case LEDPattern::PULSING_WHITE:
    return strip_.Color(brightness, brightness, brightness); // White
  case LEDPattern::BLINKING_RED:
    return strip_.Color(brightness, 0, 0); // Red
  case LEDPattern::SOLID_GREEN:
    return strip_.Color(0, brightness, 0); // Green
  case LEDPattern::SOLID_WHITE:
    return strip_.Color(brightness, brightness, brightness); // White
  default:
    return 0; // Off
  }
}