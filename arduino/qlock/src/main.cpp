#include <Arduino.h>
#include "wifi-manager.h"
#include "time-manager.h"
#include "led-strip.h"
#include "wifi-config.h" // Contains WIFI_SSID and WIFI_PASSWORD

// LED Configuration
const int LED_PIN = D1; // Your WS2812 strip data pin
const int NUM_LEDS = 144;
const int STATUS_LED_INDEX = 0;

// Global objects
WiFiManager wifiManager(WIFI_SSID, WIFI_PASSWORD);
TimeManager timeManager;
LedStrip ledStrip(LED_PIN, NUM_LEDS, STATUS_LED_INDEX);

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
  ledStrip.setPattern(LEDPattern::PULSING_BLUE);

  // Reconnect WiFi
  WiFi.mode(WIFI_STA);
  if (wifiManager.connectWithCallback(updateLedStrip))
  {
    timeManager.begin();

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

void handleWordClock()
{
  struct tm timeInfo;
  if (!timeManager.getCurrentTime(&timeInfo))
  {
    Serial.println("No time available - clock stopped");
    return;
  }

  // Print current time
  char timeBuffer[32];
  strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", &timeInfo);
  Serial.println(timeBuffer);

  // TODO: Add your word clock logic here:
  // - Clear the strip: statusLED.clear()
  // - Set pixels for time display: statusLED.getStrip().setPixelColor(i, color)
  // - Update the strip: statusLED.show()

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
  Serial.println("Setting pattern to pulsing yellow...");
  ledStrip.setPattern(LEDPattern::PULSING_YELLOW);

  // Connect to WiFi with LED callback
  Serial.println("Connecting to WiFi for initial setup...");
  if (wifiManager.connectWithCallback(updateLedStrip))
  {
    Serial.println("WiFi connected, fetching time...");
    ledStrip.setPattern(LEDPattern::PULSING_BLUE);

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
