#pragma once
#include <cstdint>
#include "../inputs/button.h"

void setupMqtt();
void ensureMqttConnected();
void publishHeartbeat();
void publishButtonPress(ButtonSide side, uint32_t timestampMs);
void mqttLoop();