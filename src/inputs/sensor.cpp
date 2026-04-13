#include "sensor.h"
#include "config.h"

constexpr uint16_t DRAIN_DEBOUNCE_MS  = 50;
constexpr uint16_t TILT_WINDOW_MS     = 500;
constexpr uint8_t  TILT_SHAKE_COUNT   = 3;

DrainState drainInit() {
  DrainState s;
  return s;
}

TiltState tiltInit() {
  TiltState s;
  return s;
}

SensorEvent drainUpdate(DrainState& state, bool rawValue, uint32_t nowMs) {
  if (rawValue != state.lastRaw) {
    state.lastRaw      = rawValue;
    state.lastChangeMs = nowMs;
    return SensorEvent::NONE;
  }

  if ((nowMs - state.lastChangeMs) < DRAIN_DEBOUNCE_MS) {
    return SensorEvent::NONE;
  }

  if (rawValue != state.lastDebounced) {
    state.lastDebounced = rawValue;
    // LOW = ball detected in drain
    if (!rawValue) return SensorEvent::DRAIN;
  }

  return SensorEvent::NONE;
}

SensorEvent tiltUpdate(TiltState& state, bool shakeDetected, uint32_t nowMs) {
  if (!shakeDetected) return SensorEvent::NONE;

  if (state.shakeCount == 0) {
    state.firstShakeMs = nowMs;
    state.shakeCount   = 1;
    return SensorEvent::NONE;
  }

  if ((nowMs - state.firstShakeMs) > TILT_WINDOW_MS) {
    // Window expired — reset and start fresh
    state.shakeCount   = 1;
    state.firstShakeMs = nowMs;
    return SensorEvent::NONE;
  }

  state.shakeCount++;

  if (state.shakeCount >= TILT_SHAKE_COUNT) {
    state.shakeCount   = 0;
    state.firstShakeMs = 0;
    return SensorEvent::TILT;
  }

  return SensorEvent::NONE;
}