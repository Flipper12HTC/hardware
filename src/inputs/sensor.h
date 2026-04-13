#pragma once
#include <cstdint>

enum class SensorEvent : uint8_t { NONE, DRAIN, TILT };

struct TiltState {
  uint8_t  shakeCount   = 0;
  uint32_t firstShakeMs = 0;
};

struct DrainState {
  bool     lastRaw      = true;
  bool     lastDebounced = true;
  uint32_t lastChangeMs = 0;
};

DrainState drainInit();
TiltState  tiltInit();

SensorEvent drainUpdate(DrainState& state, bool rawValue, uint32_t nowMs);
SensorEvent tiltUpdate(TiltState& state, bool shakeDetected, uint32_t nowMs);