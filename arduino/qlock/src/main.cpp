#include <Arduino.h>
#include "wifi-manager.h"
#include "time-manager.h"
#include "led-strip.h"

// Configuration
const char *WIFI_SSID = "WapipapoZen";
const char *WIFI_PASSWORD = "ganskesikkert";

// LED Configuration
const int LED_PIN = D4;         // GPIO pin for WS2812 strip
const int NUM_LEDS = 144;       // Total number of LEDs in your strip
const int STATUS_LED_INDEX = 0; // Which LED to use for status (first LED)

// Global objects
WiFiManager wifiManager(WIFI_SSID, WIFI_PASSWORD);
TimeManager timeManager;
WS2812Status statusLED(LED_PIN, NUM_LEDS, STATUS_LED_INDEX);

// System state
enum class SystemState
{
  WIFI_CONNECTING,
  TIME_SYNCING,
  RUNNING,
  ERROR
};

SystemState currentState = SystemState::WIFI_CONNECTING;

void setup()
{
  Serial.begin(460800);
  delay(500);
  Serial.println();
  Serial.println("Word Clock starting...");

  // Initialize LED strip
  statusLED.begin();
  statusLED.setPattern(LEDPattern::PULSING_YELLOW);

  // Connect to WiFi
  if (wifiManager.connect())
  {
    Serial.println("WiFi connected, starting time sync...");
    currentState = SystemState::TIME_SYNCING;
    statusLED.setPattern(LEDPattern::PULSING_BLUE);
    timeManager.begin();
  }
  else
  {
    Serial.println("WiFi connection failed!");
    currentState = SystemState::ERROR;
    statusLED.setPattern(LEDPattern::BLINKING_RED);
  }
}

void loop()
{
  // Always update LED patterns and managers
  statusLED.update();
  timeManager.update();
  wifiManager.handleReconnection();

  // State machine
  switch (currentState)
  {
  case SystemState::WIFI_CONNECTING:
    if (wifiManager.isConnected())
    {
      currentState = SystemState::TIME_SYNCING;
      statusLED.setPattern(LEDPattern::PULSING_BLUE);
      timeManager.begin();
    }
    delay(100);
    break;

  case SystemState::TIME_SYNCING:
    if (timeManager.isTimeSynced())
    {
      Serial.println("System ready!");
      currentState = SystemState::RUNNING;
      statusLED.setPattern(LEDPattern::SOLID_GREEN);
    }
    else if (timeManager.getStatus() == TimeStatus::FAILED)
    {
      Serial.println("Time sync failed!");
      currentState = SystemState::ERROR;
      statusLED.setPattern(LEDPattern::BLINKING_RED);
    }
    delay(100);
    break;

  case SystemState::RUNNING:
    handleWordClock();

    // Check for connection issues
    if (!wifiManager.isConnected())
    {
      Serial.println("WiFi disconnected!");
      currentState = SystemState::ERROR;
      statusLED.setPattern(LEDPattern::BLINKING_RED);
    }
    else if (!timeManager.isTimeSynced())
    {
      Serial.println("Time sync lost!");
      currentState = SystemState::TIME_SYNCING;
      statusLED.setPattern(LEDPattern::PULSING_BLUE);
    }
    break;

  case SystemState::ERROR:
    // Try to recover
    if (wifiManager.isConnected() && timeManager.isTimeSynced())
    {
      Serial.println("System recovered!");
      currentState = SystemState::RUNNING;
      statusLED.setPattern(LEDPattern::SOLID_GREEN);
    }
    else if (wifiManager.isConnected())
    {
      Serial.println("WiFi recovered, resyncing time...");
      currentState = SystemState::TIME_SYNCING;
      statusLED.setPattern(LEDPattern::PULSING_BLUE);
      timeManager.begin();
    }
    delay(1000); // Wait longer in error state
    break;
  }
}

void handleWordClock()
{
  struct tm timeInfo;
  if (!timeManager.getCurrentTime(&timeInfo))
  {
    Serial.println("Waiting for time sync...");
    delay(1000);
    return;
  }

  // Print current time
  char timeBuffer[32];
  strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", &timeInfo);
  Serial.println(timeBuffer);

  // TODO: Add your word clock logic here:
  // - Map time to letters
  // - Clear the strip: statusLED.clear()
  // - Set pixels for time display: statusLED.getStrip().setPixelColor(i, color)
  // - Update the strip: statusLED.show()
  // - Note: Status LED (index STATUS_LED_INDEX) will be overwritten by status system

  // Example: Set a few LEDs to white for testing
  // statusLED.clear();
  // for (int i = 1; i < 10; i++) {  // Skip status LED at index 0
  //   statusLED.getStrip().setPixelColor(i, statusLED.getStrip().Color(50, 50, 50));
  // }
  // statusLED.show();

  // Sleep until the next minute
  int secondsToWait = 60 - timeInfo.tm_sec;
  delay(secondsToWait * 1000);
}