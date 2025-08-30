#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <ESP8266WiFi.h>

enum class WiFiConnectionStatus
{
  CONNECTING,
  CONNECTED,
  FAILED,
  DISCONNECTED
};

class WiFiManager
{
public:
  WiFiManager(const char *ssid, const char *password);

  bool connect();
  bool connectWithCallback(void (*updateCallback)() = nullptr);
  bool isConnected();
  void handleReconnection();
  WiFiConnectionStatus getStatus();

private:
  const char *ssid_;
  const char *password_;
  WiFiConnectionStatus status_;
  unsigned long lastReconnectAttempt_;

  void printWiFiStatus(wl_status_t status);
};

#endif