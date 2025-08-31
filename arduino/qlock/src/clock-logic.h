#ifndef CLOCK_LOGIC_H
#define CLOCK_LOGIC_H

#include <time.h>

class ClockLogic
{
public:
  ClockLogic();

  // Extract time components from tm struct
  bool updateTime(const struct tm &timeInfo);

  // Get current time components
  int getHour() const { return hour_; }
  int getMinute() const { return minute_; }
  int getSecond() const { return second_; }

  // Check if time has changed since last update
  bool hasTimeChanged() const { return timeChanged_; }

  // Get formatted time string for debugging
  void getTimeString(char *buffer, size_t bufferSize) const;

private:
  int hour_;
  int minute_;
  int second_;
  bool timeChanged_;

  // Previous values to detect changes
  int prevHour_;
  int prevMinute_;
  int prevSecond_;
};

#endif