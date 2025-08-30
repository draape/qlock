#include "wifi-manager.h"
#include <Arduino.h>

WiFiManager::WiFiManager(const char *ssid, const char *password)
    : ssid_(ssid), password_(password), status_(WiFiConnectionStatus::DISCONNECTED), lastReconnectAttempt_(0)
{
}

bool WiFiManager::connectWithCallback(void (*updateCallback)())
{
  Serial.println("Connecting to WiFi...");
  Serial.printf("SSID: %s\n", ssid_);
  Serial.printf("Password length: %d\n", strlen(password_));

  status_ = WiFiConnectionStatus::CONNECTING;

  // Try disconnecting first to clear any old connections
  WiFi.disconnect(true);
  delay(1000);

  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);
  WiFi.setSleepMode(WIFI_NONE_SLEEP);

  Serial.println("Starting WiFi.begin()...");
  WiFi.begin(ssid_, password_);

  unsigned long startTime = millis();
  unsigned long lastStatusPrint = 0;
  const unsigned long timeout = 60000;      // 30 second timeout
  const unsigned long statusInterval = 500; // Print status every 500ms

  while (WiFi.status() != WL_CONNECTED)
  {
    // Check timeout
    if (millis() - startTime > timeout)
    {
      Serial.println("WiFi Connection failed - timeout!");
      status_ = WiFiConnectionStatus::FAILED;
      return false;
    }

    // Print status periodically (non-blocking)
    if (millis() - lastStatusPrint > statusInterval)
    {
      printWiFiStatus(WiFi.status());
      lastStatusPrint = millis();
    }

    // Call the update callback frequently for smooth LED animation
    if (updateCallback)
    {
      updateCallback();
    }

    delay(50); // Much shorter delay for smooth LED updates
  }

  status_ = WiFiConnectionStatus::CONNECTED;
  Serial.println("WiFi Connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Signal strength: ");
  Serial.println(WiFi.RSSI());
  return true;
}

bool WiFiManager::connect()
{
  return connectWithCallback(nullptr);
}

bool WiFiManager::isConnected()
{
  if (WiFi.status() == WL_CONNECTED && status_ != WiFiConnectionStatus::CONNECTED)
  {
    // Just connected!
    status_ = WiFiConnectionStatus::CONNECTED;
    Serial.println("WiFi Connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else if (WiFi.status() != WL_CONNECTED && status_ == WiFiConnectionStatus::CONNECTED)
  {
    // Disconnected
    status_ = WiFiConnectionStatus::DISCONNECTED;
  }

  return WiFi.status() == WL_CONNECTED;
}

void WiFiManager::handleReconnection()
{
  if (!isConnected() && (millis() - lastReconnectAttempt_ > 30000))
  { // Try every 30 seconds
    lastReconnectAttempt_ = millis();
    Serial.println("Attempting WiFi reconnection...");
    connect();
  }
}

WiFiConnectionStatus WiFiManager::getStatus()
{
  // Update status based on actual WiFi state
  if (WiFi.status() != WL_CONNECTED && status_ == WiFiConnectionStatus::CONNECTED)
  {
    status_ = WiFiConnectionStatus::DISCONNECTED;
  }
  return status_;
}

void WiFiManager::printWiFiStatus(wl_status_t s)
{
  const char *msg = "UNKNOWN";
  switch (s)
  {
  case WL_IDLE_STATUS:
    msg = "IDLE";
    break;
  case WL_NO_SSID_AVAIL:
    msg = "NO_SSID_AVAIL";
    break;
  case WL_SCAN_COMPLETED:
    msg = "SCAN_COMPLETED";
    break;
  case WL_CONNECTED:
    msg = "CONNECTED";
    break;
  case WL_CONNECT_FAILED:
    msg = "CONNECT_FAILED";
    break;
  case WL_CONNECTION_LOST:
    msg = "CONNECTION_LOST";
    break;
  case WL_DISCONNECTED:
    msg = "DISCONNECTED";
    break;
  }
  Serial.printf("WiFi.status(): %d (%s)\n", s, msg);
}