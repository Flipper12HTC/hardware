#include <Arduino.h>
#include "inputs/button.h"
#include "inputs/config.h"

static ButtonState btnLeft;
static ButtonState btnRight;

void setup() {
  Serial.begin(115200);
  delay(100);
  Serial.println();
  Serial.println("flipper12-hardware — starting up...");

  pinMode(PIN_BTN_LEFT,  INPUT_PULLUP);
  pinMode(PIN_BTN_RIGHT, INPUT_PULLUP);

  btnLeft  = buttonInit(ButtonSide::LEFT);
  btnRight = buttonInit(ButtonSide::RIGHT);

  Serial.println("Skeleton only. No logic yet.");
}

void loop() {
  const uint32_t now = millis();

  ButtonEvent evL = buttonUpdate(btnLeft,  digitalRead(PIN_BTN_LEFT),  now);
  ButtonEvent evR = buttonUpdate(btnRight, digitalRead(PIN_BTN_RIGHT), now);

  if (evL == ButtonEvent::PRESS)   Serial.println("LEFT  — PRESS");
  if (evL == ButtonEvent::RELEASE) Serial.println("LEFT  — RELEASE");
  if (evR == ButtonEvent::PRESS)   Serial.println("RIGHT — PRESS");
  if (evR == ButtonEvent::RELEASE) Serial.println("RIGHT — RELEASE");
}