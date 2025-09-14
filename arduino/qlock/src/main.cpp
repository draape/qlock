#include <Arduino.h>
#include "wifi-manager.h"
#include "time-manager.h"
#include "led-strip.h"
#include "wifi-config.h" // Contains WIFI_SSID and WIFI_PASSWORD
#include "clock-logic.h"
#include "word-mapping.h"

// LED Configuration
const int LED_PIN = D1; // Your WS2812 strip data pin
const int NUM_LEDS = 115;
const int STATUS_LED_INDEX = 0;

// Global objects
WiFiManager wifiManager(WIFI_SSID, WIFI_PASSWORD);
TimeManager timeManager;
LedStrip ledStrip(LED_PIN, NUM_LEDS, STATUS_LED_INDEX);
ClockLogic clockLogic;
WordMapping wordMap;

// Simple state tracking
bool initialSetupComplete = false;
unsigned long lastTimeFetch = 0;
const unsigned long TIME_FETCH_INTERVAL = 24UL * 60 * 60 * 1000; // 24 hours in milliseconds

// Forward declaration for LED update callback
void updateLedStrip()
{
  ledStrip.update();
}

void attemptTimeRefresh()
{
  Serial.println("Attempting daily time refresh...");
  ledStrip.setPattern(LEDPattern::SOLID_BLUE);

  // Reconnect WiFi
  WiFi.mode(WIFI_STA);
  if (wifiManager.connectWithCallback(updateLedStrip))
  {
    // NTP sync happens automatically in background, no need to reinitialize

    // Try for 10 seconds
    unsigned long syncStart = millis();
    bool syncSuccess = false;
    while (!syncSuccess && (millis() - syncStart < 10000))
    {
      ledStrip.update();
      timeManager.update();
      if (timeManager.isTimeSynced())
      {
        syncSuccess = true;
        break;
      }
      delay(100);
    }

    if (syncSuccess)
    {
      Serial.println("Daily time refresh successful!");
      ledStrip.setPattern(LEDPattern::SOLID_GREEN);
      delay(1000);
      ledStrip.setPattern(LEDPattern::SOLID_WHITE); // Back to white status
      lastTimeFetch = millis();
    }
    else
    {
      Serial.println("Daily time refresh failed - continuing with current time");
      ledStrip.setPattern(LEDPattern::PULSING_WHITE); // Non-critical warning
      // Don't update lastTimeFetch, will try again tomorrow
    }

    // Disconnect WiFi again
    WiFi.disconnect();
    WiFi.mode(WIFI_OFF);
  }
  else
  {
    Serial.println("WiFi connection failed during refresh - continuing with current time");
    ledStrip.setPattern(LEDPattern::PULSING_WHITE); // Non-critical warning
  }
}

void displayWord(int *wordIndices)
{
  for (int i = 0; wordIndices[i] != -1; i++)
  {
    ledStrip.setPixel(wordIndices[i], ledStrip.White(150));
  }
}

void handleWordClock()
{
  struct tm timeInfo;
  if (!timeManager.getCurrentTime(&timeInfo))
  {
    Serial.println("No time available - clock stopped");
    return;
  }

  ledStrip.clear();

  Serial.println(timeInfo.tm_min);
  Serial.println(timeInfo.tm_hour);

  int currentMinute = timeInfo.tm_min;
  int minuteDisplay = currentMinute % 5;
  if (minuteDisplay > 0)
  {
    ledStrip.setPixel(wordMap.getMinuteOne(), ledStrip.Color(255, 255, 255));
  }
  if (minuteDisplay > 1)
  {
    ledStrip.setPixel(wordMap.getMinuteTwo(), ledStrip.Color(255, 255, 255));
  }
  if (minuteDisplay > 2)
  {
    ledStrip.setPixel(wordMap.getMinuteThree(), ledStrip.Color(255, 255, 255));
  }
  if (minuteDisplay > 3)
  {
    ledStrip.setPixel(wordMap.getMinuteFour(), ledStrip.Color(255, 255, 255));
  }

  int currentHour = timeInfo.tm_hour;

  // Take into account the way time is communicated in Norwegian
  if (currentMinute >= 20)
  {
    currentHour = currentHour + 1;
  }

  displayWord(wordMap.getKlokken());
  displayWord(wordMap.getHourWord(currentHour));

  if (currentMinute >= 5 && currentMinute < 10)
  {
    displayWord(wordMap.getFemIndicator());
    displayWord(wordMap.getOver());
  }
  else if (currentMinute >= 10 && currentMinute < 15)
  {
    displayWord(wordMap.getTi());
    displayWord(wordMap.getOver());
  }
  else if (currentMinute >= 15 && currentMinute < 20)
  {
    displayWord(wordMap.getKvart());
    displayWord(wordMap.getOver());
  }
  else if (currentMinute >= 20 && currentMinute < 25)
  {
    displayWord(wordMap.getTi());
    displayWord(wordMap.getPaa2());
    displayWord(wordMap.getHalv());
  }
  else if (currentMinute >= 25 && currentMinute < 30)
  {
    displayWord(wordMap.getFemIndicator());
    displayWord(wordMap.getPaa1());
    displayWord(wordMap.getHalv());
  }
  else if (currentMinute >= 30 && currentMinute < 35)
  {
    displayWord(wordMap.getHalv());
  }
  else if (currentMinute >= 35 && currentMinute < 40)
  {
    displayWord(wordMap.getFemIndicator());
    displayWord(wordMap.getOver());
    displayWord(wordMap.getHalv());
  }
  else if (currentMinute >= 40 && currentMinute < 45)
  {
    displayWord(wordMap.getTi());
    displayWord(wordMap.getOver());
    displayWord(wordMap.getHalv());
  }
  else if (currentMinute >= 45 && currentMinute < 50)
  {
    displayWord(wordMap.getKvart());
    displayWord(wordMap.getPaa3());
  }
  else if (currentMinute >= 50 && currentMinute < 55)
  {
    displayWord(wordMap.getTi());
    displayWord(wordMap.getPaa2());
  }
  else if (currentMinute >= 55 && currentMinute < 60)
  {
    displayWord(wordMap.getFemIndicator());
    displayWord(wordMap.getPaa1());
  }

  Serial.println();

  ledStrip.show();

  // Sleep until the next minute
  int secondsToWait = 60 - timeInfo.tm_sec;
  delay(secondsToWait * 1000);
}

void setup()
{
  Serial.begin(460800);
  delay(500);
  Serial.println();
  Serial.println("Word Clock starting...");

  // Initialize LED strip
  Serial.println("Initializing LED strip...");
  ledStrip.begin();
  Serial.println("Setting pattern to solid yellow...");
  ledStrip.setPattern(LEDPattern::SOLID_YELLOW);

  // Connect to WiFi with LED callback
  Serial.println("Connecting to WiFi for initial setup...");
  if (wifiManager.connectWithCallback(updateLedStrip))
  {
    Serial.println("WiFi connected, fetching time...");
    ledStrip.setPattern(LEDPattern::SOLID_BLUE);

    timeManager.begin();

    // Wait for time sync with timeout
    unsigned long syncStart = millis();
    while (!timeManager.isTimeSynced() && (millis() - syncStart < 15000))
    {
      ledStrip.update(); // Keep the blue pulse going
      timeManager.update();
      delay(100);
    }

    if (timeManager.isTimeSynced())
    {
      Serial.println("Time synchronized successfully!");
      ledStrip.setPattern(LEDPattern::SOLID_GREEN);
      initialSetupComplete = true;
      lastTimeFetch = millis();

      // Disconnect WiFi to save power
      WiFi.disconnect();
      WiFi.mode(WIFI_OFF);
      Serial.println("WiFi disconnected - setup complete!");

      delay(2000);                                  // Show green for 2 seconds
      ledStrip.setPattern(LEDPattern::SOLID_WHITE); // Keep white status LED on
      Serial.println("Status LED set to solid white - system ready!");
    }
    else
    {
      Serial.println("Time sync failed!");
      ledStrip.setPattern(LEDPattern::BLINKING_RED);
      // Stay in error mode - manual reset required
      while (true)
      {
        ledStrip.update();
        delay(100);
      }
    }
  }
  else
  {
    Serial.println("WiFi connection failed!");
    ledStrip.setPattern(LEDPattern::BLINKING_RED);
    // Stay in error mode - manual reset required
    while (true)
    {
      ledStrip.update();
      delay(100);
    }
  }
}

void loop()
{
  ledStrip.update(); // In case we need status updates

  if (!initialSetupComplete)
  {
    // Should not reach here, but safety check
    delay(1000);
    return;
  }

  // Check if it's time for daily time refresh
  if (millis() - lastTimeFetch > TIME_FETCH_INTERVAL)
  {
    attemptTimeRefresh();
  }

  // Your word clock logic here
  handleWordClock();
}
