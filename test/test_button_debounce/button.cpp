#include "../../src/inputs/button.h"
#include <cstdint>

static constexpr uint16_t DEBOUNCE_MS = 15;

ButtonState buttonInit(ButtonSide side) {
  ButtonState s;
  s.side = side;
  return s;
}

ButtonEvent buttonUpdate(ButtonState& state, bool rawValue, uint32_t nowMs) {
  if (rawValue != state.lastRaw) {
    state.lastRaw      = rawValue;
    state.lastChangeMs = nowMs;
    return ButtonEvent::NONE;
  }

  if ((nowMs - state.lastChangeMs) < DEBOUNCE_MS) {
    return ButtonEvent::NONE;
  }

  if (rawValue != state.lastDebounced) {
    state.lastDebounced = rawValue;
    return rawValue ? ButtonEvent::RELEASE : ButtonEvent::PRESS;
  }

  return ButtonEvent::NONE;
}
