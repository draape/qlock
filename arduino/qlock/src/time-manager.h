#ifndef TIME_MANAGER_H
#define TIME_MANAGER_H

#include <time.h>

enum class TimeStatus
{
  SYNCING,
  SYNCED,
  FAILED
};

class TimeManager
{
public:
  TimeManager();

  void begin();
  bool isTimeSynced();
  bool getCurrentTime(struct tm *timeInfo);
  TimeStatus getStatus();
  void update();

private:
  TimeStatus status_;
  unsigned long lastSyncCheck_;

  static const char *TZ_INFO;
  static const char *NTP_SERVER_1;
  static const char *NTP_SERVER_2;

  bool checkTimeSync();
};

#endif