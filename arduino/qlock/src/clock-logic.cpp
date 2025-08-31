#include "clock-logic.h"
#include <Arduino.h>
#include <string.h>

ClockLogic::ClockLogic()
    : hour_(0), minute_(0), second_(0), timeChanged_(false),
      prevHour_(-1), prevMinute_(-1), prevSecond_(-1)
{
}

bool ClockLogic::updateTime(const struct tm &timeInfo)
{
  // Extract time components
  int newHour = timeInfo.tm_hour;
  int newMinute = timeInfo.tm_min;
  int newSecond = timeInfo.tm_sec;

  // Check if any component has changed
  timeChanged_ = (newHour != prevHour_) ||
                 (newMinute != prevMinute_) ||
                 (newSecond != prevSecond_);

  // Update current values
  hour_ = newHour;
  minute_ = newMinute;
  second_ = newSecond;

  // Update previous values for next comparison
  prevHour_ = newHour;
  prevMinute_ = newMinute;
  prevSecond_ = newSecond;

  // Debug output when time changes
  if (timeChanged_)
  {
    Serial.printf("Time updated: %02d:%02d:%02d\n", hour_, minute_, second_);
  }

  return timeChanged_;
}

void ClockLogic::getTimeString(char *buffer, size_t bufferSize) const
{
  snprintf(buffer, bufferSize, "%02d:%02d:%02d", hour_, minute_, second_);
}