# flipper12-hardware

ESP32 firmware for the Flipper 12 physical arcade machine.
Reads buttons and sensors, applies software debounce, and publishes
events to the MQTT broker consumed by the game engine.

See `flipper12-product` for the full spec, CDC, and backlog.

## Role

The ESP32 is a **spine, not a brain**:

- Reads GPIO pins (left/right buttons, tilt, drain)
- Applies software debounce
- Publishes events on `pinball/<device_id>/input/...` via MQTT
- Publishes a heartbeat on `pinball/<device_id>/status` every 5s

No game logic, no score, no game state. The backend decides everything.

## Stack

- ESP32 + PlatformIO + Arduino framework
- C++17
- PubSubClient (MQTT)
- ArduinoJson v7
- Unity (native unit tests — no hardware needed)

## Prerequisites

- PlatformIO Core (VS Code extension or `pipx install platformio`)
- ESP32 DevKitC + USB cable (only needed to flash)

## Setup

```bash
# Copy secrets template and fill in your WiFi credentials
cp src/secrets.h.example src/secrets.h
```

## Commands

```bash
# Compile for ESP32 (no board needed)
pio run

# Flash to a connected ESP32
pio run --target upload

# Open serial monitor
pio device monitor

# Run unit tests without hardware
pio test -e native

# Format code
clang-format -i src/**/*.cpp src/**/*.h
```

## Structure

```
src/
├── main.cpp
├── inputs/       GPIO reading + debounce (buttons, drain, tilt)
├── net/          WiFi + MQTT client
└── messages/     ArduinoJson payload builders

test/
├── test_button_debounce/
├── test_payload_builder/
└── test_sensor_logic/

contracts/        MQTT schemas synced from flipper12-backend
docs/             Wiring diagram, flashing guide
```