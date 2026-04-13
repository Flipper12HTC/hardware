#include <Arduino.h>
#include <WiFi.h>
#include "inputs/button.h"
#include "inputs/config.h"
#include "net/mqtt.h"

static ButtonState btnLeft;
static ButtonState btnRight;

static void connectWifi() {
  const char* ssid     = "YOUR_SSID";
  const char* password = "YOUR_PASSWORD";
  Serial.print("[WiFi] Connecting to ");
  Serial.print(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print(" connected — IP: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);
  delay(100);
  Serial.println();
  Serial.println("flipper12-hardware — starting up...");

  pinMode(PIN_BTN_LEFT,  INPUT_PULLUP);
  pinMode(PIN_BTN_RIGHT, INPUT_PULLUP);

  btnLeft  = buttonInit(ButtonSide::LEFT);
  btnRight = buttonInit(ButtonSide::RIGHT);

  connectWifi();
  setupMqtt();

  Serial.println("Setup complete.");
}

void loop() {
  const uint32_t now = millis();

  ensureMqttConnected();
  publishHeartbeat();

  ButtonEvent evL = buttonUpdate(btnLeft,  digitalRead(PIN_BTN_LEFT),  now);
  ButtonEvent evR = buttonUpdate(btnRight, digitalRead(PIN_BTN_RIGHT), now);

  if (evL == ButtonEvent::PRESS)   { Serial.println("LEFT  — PRESS");    publishButtonEvent("left",  "press"); }
  if (evL == ButtonEvent::RELEASE) { Serial.println("LEFT  — RELEASE");  publishButtonEvent("left",  "release"); }
  if (evR == ButtonEvent::PRESS)   { Serial.println("RIGHT — PRESS");    publishButtonEvent("right", "press"); }
  if (evR == ButtonEvent::RELEASE) { Serial.println("RIGHT — RELEASE");  publishButtonEvent("right", "release"); }

  mqttLoop();
}