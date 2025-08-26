#include "wifi-manager.h"

WiFiManager::WiFiManager(const char *ssid, const char *password)
    : ssid_(ssid), password_(password), status_(WiFiConnectionStatus::DISCONNECTED), lastReconnectAttempt_(0)
{
}

bool WiFiManager::connect()
{
  Serial.println("Connecting to WiFi");
  status_ = WiFiConnectionStatus::CONNECTING;

  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);
  WiFi.setSleepMode(WIFI_NONE_SLEEP);
  WiFi.begin(ssid_, password_);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20)
  {
    printWiFiStatus(WiFi.status());
    delay(500);
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED)
  {
    status_ = WiFiConnectionStatus::CONNECTED;
    Serial.println("WiFi Connected!");
    return true;
  }
  else
  {
    status_ = WiFiConnectionStatus::FAILED;
    Serial.println("WiFi Connection failed!");
    return false;
  }
}

bool WiFiManager::isConnected()
{
  return WiFi.status() == WL_CONNECTED && status_ == WiFiConnectionStatus::CONNECTED;
}

void WiFiManager::handleReconnection()
{
  if (!isConnected() && (millis() - lastReconnectAttempt_ > 30000))
  {
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