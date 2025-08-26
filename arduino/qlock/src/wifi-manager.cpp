#include "wifi-manager.h"
#include <Arduino.h>

WiFiManager::WiFiManager(const char *ssid, const char *password)
    : ssid_(ssid), password_(password), status_(WiFiConnectionStatus::DISCONNECTED), lastReconnectAttempt_(0)
{
}

void WiFiManager::connect()
{
  Serial.println("Starting WiFi connection...");
  status_ = WiFiConnectionStatus::CONNECTING;

  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);
  WiFi.setSleepMode(WIFI_NONE_SLEEP);
  WiFi.begin(ssid_, password_);
}

bool WiFiManager::isConnected()
{
  if (WiFi.status() == WL_CONNECTED && status_ != WiFiConnectionStatus::CONNECTED)
  {
    status_ = WiFiConnectionStatus::CONNECTED;
    Serial.println("WiFi Connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else if (WiFi.status() != WL_CONNECTED && status_ == WiFiConnectionStatus::CONNECTED)
  {
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