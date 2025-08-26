#include "time-manager.h"
#include <Arduino.h>

const char *TimeManager::TZ_INFO = "CET-1CEST,M3.5.0/02,M10.5.0/03";
const char *TimeManager::NTP_SERVER_1 = "pool.ntp.org";
const char *TimeManager::NTP_SERVER_2 = "time.google.com";

TimeManager::TimeManager() : status_(TimeStatus::SYNCING), lastSyncCheck_(0)
{
}

void TimeManager::begin()
{
  Serial.println("Setting up time synchronization...");
  status_ = TimeStatus::SYNCING;

  configTime(0, 0, NTP_SERVER_1, NTP_SERVER_2);
  setenv("TZ", TZ_INFO, 1);
  tzset();

  delay(1000);
}

bool TimeManager::isTimeSynced()
{
  return checkTimeSync() && status_ == TimeStatus::SYNCED;
}

bool TimeManager::getCurrentTime(struct tm *timeInfo)
{
  if (!checkTimeSync())
  {
    return false;
  }

  time_t now = time(nullptr);
  localtime_r(&now, timeInfo);
  return true;
}

TimeStatus TimeManager::getStatus()
{
  return status_;
}

void TimeManager::update()
{
  if (millis() - lastSyncCheck_ > 5000)
  {
    lastSyncCheck_ = millis();

    if (checkTimeSync())
    {
      if (status_ != TimeStatus::SYNCED)
      {
        status_ = TimeStatus::SYNCED;
        Serial.println("Time synchronized successfully!");
      }
    }
    else
    {
      if (status_ == TimeStatus::SYNCED)
      {
        Serial.println("Warning: Time sync lost!");
        status_ = TimeStatus::FAILED;
      }
      else if (status_ == TimeStatus::SYNCING && millis() > 30000)
      {
        status_ = TimeStatus::FAILED;
        Serial.println("Time synchronization failed!");
      }
    }
  }
}

bool TimeManager::checkTimeSync()
{
  time_t now = time(nullptr);
  // Check if time is reasonable (after year 2020)
  return now > 1577836800; // 2020-01-01 00:00:00 UTC
}