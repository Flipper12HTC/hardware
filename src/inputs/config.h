#pragma once

// ─── Pins ───────────────────────────────────────────────
constexpr uint8_t PIN_BTN_LEFT  = 34;
constexpr uint8_t PIN_BTN_RIGHT = 35;

// ─── Debounce ───────────────────────────────────────────
constexpr uint16_t DEBOUNCE_MS = 15;

// ─── Device identity ────────────────────────────────────
constexpr const char* DEVICE_ID        = "flipper-01";
constexpr const char* FIRMWARE_VERSION = "0.1.0";

// ─── MQTT ───────────────────────────────────────────────
constexpr const char* MQTT_BROKER = "192.168.1.100";
constexpr uint16_t   MQTT_PORT   = 1883;

// ─── Heartbeat ──────────────────────────────────────────
constexpr uint32_t HEARTBEAT_INTERVAL_MS = 5000;