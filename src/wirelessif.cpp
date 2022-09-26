/**
 * @file wirelessif.cpp
 * @author Adrian Saldana
 * @brief
 * @version 0.1
 * @date 2022-09-18
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "wirelessif.h"
#include <Arduino.h>
#include "sdkconfig.h"
// Load Wi-Fi library
#include <WiFi.h>
// Load MQTT library
#include <AsyncMqttClient.h>

static const char *TAG = "wirelessif";

// Define MQTT Broker and port
// Just to test development time 
#define MQTT_HOST IPAddress(192, 168, 0, 2) 
#define MQTT_PORT 1883

AsyncMqttClient mqttClient;
TimerHandle_t mqttReconnectTimer;
TimerHandle_t wifiReconnectTimer;
bool disconnected;

void connectToWifi();
void connectToMqtt();
void WiFiEvent(WiFiEvent_t event);
void onMqttConnect(bool sessionPresent);
void onMqttDisconnect(AsyncMqttClientDisconnectReason reason);
void onMqttSubscribe(uint16_t packetId, uint8_t qos);
void onMqttUnsubscribe(uint16_t packetId);
void onMqttMessage(char *topic, char *payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total);
void onMqttPublish(uint16_t packetId);

void init_iot_client()
{
  mqttReconnectTimer = xTimerCreate("mqttTimer", pdMS_TO_TICKS(2000), pdFALSE, (void *)0, reinterpret_cast<TimerCallbackFunction_t>(connectToMqtt));
  wifiReconnectTimer = xTimerCreate("wifiTimer", pdMS_TO_TICKS(2000), pdFALSE, (void *)0, reinterpret_cast<TimerCallbackFunction_t>(connectToWifi));

  WiFi.onEvent(WiFiEvent);

  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  mqttClient.onSubscribe(onMqttSubscribe);
  mqttClient.onUnsubscribe(onMqttUnsubscribe);
  mqttClient.onMessage(onMqttMessage);
  mqttClient.onPublish(onMqttPublish);
  mqttClient.setServer(MQTT_HOST, MQTT_PORT);

  connectToWifi();
}

void connectToWifi()
{
  ESP_LOGI(TAG, "Connecting to Wi-Fi...");
  //WiFi.begin(ssid, password);
  ESP_LOGI(TAG, "Local IP: %s", WiFi.localIP());
}

void connectToMqtt()
{
  ESP_LOGI(TAG, "Connecting to MQTT...");
  mqttClient.connect();
}

void WiFiEvent(WiFiEvent_t event)
{
  ESP_LOGI(TAG, "[WiFi-event] event: %d", event);
  switch (event)
  {
  case SYSTEM_EVENT_STA_GOT_IP:
    ESP_LOGI(TAG, "WiFi connected, IP address: %s", WiFi.localIP());
    connectToMqtt();
    break;
  case SYSTEM_EVENT_STA_DISCONNECTED:
    ESP_LOGI(TAG, "WiFi lost connection");
    xTimerStop(mqttReconnectTimer, 0); // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
    xTimerStart(wifiReconnectTimer, 0);
    break;
  case SYSTEM_EVENT_STA_WPS_ER_PBC_OVERLAP:
    break;
  default:
    break;
  }
}

void onMqttConnect(bool sessionPresent)
{
  ESP_LOGI(TAG, "Connected to MQTT. Session present: %d", sessionPresent);
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason)
{
  ESP_LOGI(TAG, "Disconnected from MQTT. Reason: %d", reason);

  if (WiFi.isConnected() && !disconnected)
  {
    xTimerStart(mqttReconnectTimer, 0);
  }
}

void onMqttSubscribe(uint16_t packetId, uint8_t qos)
{
  ESP_LOGI(TAG, "Subscribe acknowledged.   packetId: %d   qos: %d", packetId, qos);
}

void onMqttUnsubscribe(uint16_t packetId)
{
  ESP_LOGI(TAG, "Unsubscribe acknowledged.   packetId: %d", packetId);
}

void onMqttMessage(char *topic, char *payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total)
{
  ESP_LOGI(TAG, "Publish received.   topic: %s   qos: %d"
                "dup: %d   retain: %d  len: %d  index: %d   total: %d",
           topic, properties.qos, properties.dup, properties.retain, len, index, total);
}

void onMqttPublish(uint16_t packetId)
{
  ESP_LOGI(TAG, "Publish acknowledged.   packetId: %d", packetId);
}

int publish(const char *topic, const char *payload, size_t length)
{
  mqttClient.publish(topic, 2, true, payload, length);
  ESP_LOGI(TAG, "Publishing at QoS 2");
  return 0;
}

int disconnect()
{
  mqttClient.disconnect();
  xTimerStop(mqttReconnectTimer, 0); // ensure we don't reconnect to MQTT
  disconnected = true;
  return 0;
}
