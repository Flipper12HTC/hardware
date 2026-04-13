#include "mqtt.h"
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "../inputs/config.h"
#include "../messages/payloads.h"

static WiFiClient   wifiClient;
static PubSubClient mqttClient(wifiClient);

static uint32_t lastHeartbeatMs = 0;

static void onMessage(const char* topic, uint8_t* payload, unsigned int length) {
  Serial.print("[MQTT] Message received on ");
  Serial.println(topic);
}

void setupMqtt() {
  mqttClient.setServer(MQTT_BROKER, MQTT_PORT);
  mqttClient.setCallback(onMessage);
  Serial.println("[MQTT] Client configured.");
}

void ensureMqttConnected() {
  if (mqttClient.connected()) return;

  Serial.print("[MQTT] Connecting to broker...");

  const String clientId = String("flipper-") + DEVICE_ID;

  if (mqttClient.connect(clientId.c_str())) {
    Serial.println(" connected.");
  } else {
    Serial.print(" failed, rc=");
    Serial.println(mqttClient.state());
  }
}

void publishHeartbeat() {
  const uint32_t now = millis();
  if (now - lastHeartbeatMs < HEARTBEAT_INTERVAL_MS) return;
  lastHeartbeatMs = now;

  JsonDocument doc;
  doc["firmware_version"] = FIRMWARE_VERSION;
  doc["uptime_ms"]        = now;
  doc["wifi_rssi"]        = WiFi.RSSI();
  doc["free_heap"]        = ESP.getFreeHeap();

  char buffer[128];
  serializeJson(doc, buffer);

  const String topic = String("pinball/") + DEVICE_ID + "/status";
  mqttClient.publish(topic.c_str(), buffer);

  Serial.print("[MQTT] Heartbeat published: ");
  Serial.println(buffer);
}

void publishButtonPress(ButtonSide side, uint32_t timestampMs) {
  if (!mqttClient.connected()) return;

  char buffer[128];
  buildButtonPressPayload(side, timestampMs, buffer, sizeof(buffer));

  const String topic = String("pinball/") + DEVICE_ID + "/input/button";

  const uint32_t before = millis();
  mqttClient.publish(topic.c_str(), buffer);
  const uint32_t latency = millis() - before;

  Serial.print("[MQTT] Button press published in ");
  Serial.print(latency);
  Serial.print("ms: ");
  Serial.println(buffer);
}

void mqttLoop() {
  mqttClient.loop();
}