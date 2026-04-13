#pragma once
#include <cstdint>

void setupMqtt();
void ensureMqttConnected();
void publishHeartbeat();
void publishButtonEvent(const char* side, const char* event);
void mqttLoop();