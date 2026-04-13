#include <unity.h>
#include <ArduinoJson.h>
#include "../../src/messages/payloads.h"

void setUp()    {}
void tearDown() {}

// ─── Heartbeat tests ─────────────────────────────────────

void test_heartbeat_has_all_fields() {
  JsonDocument doc;
  doc["firmware_version"] = "0.1.0";
  doc["uptime_ms"]        = 0;
  doc["wifi_rssi"]        = -70;
  doc["free_heap"]        = 200000;

  TEST_ASSERT_TRUE(doc["firmware_version"].is<JsonVariant>());
  TEST_ASSERT_TRUE(doc["uptime_ms"].is<JsonVariant>());
  TEST_ASSERT_TRUE(doc["wifi_rssi"].is<JsonVariant>());
  TEST_ASSERT_TRUE(doc["free_heap"].is<JsonVariant>());
}

void test_heartbeat_negative_rssi() {
  JsonDocument doc;
  doc["wifi_rssi"] = -90;
  TEST_ASSERT_EQUAL(-90, doc["wifi_rssi"].as<int>());
}

// ─── Button press payload tests ──────────────────────────

void test_button_press_left_has_all_fields() {
  char buffer[128];
  buildButtonPressPayload(ButtonSide::LEFT, 12345, buffer, sizeof(buffer));

  JsonDocument doc;
  deserializeJson(doc, buffer);

  TEST_ASSERT_TRUE(doc["device_id"].is<JsonVariant>());
  TEST_ASSERT_TRUE(doc["side"].is<JsonVariant>());
  TEST_ASSERT_TRUE(doc["timestamp_ms"].is<JsonVariant>());
  TEST_ASSERT_TRUE(doc["event"].is<JsonVariant>());
}

void test_button_press_left_side_is_L() {
  char buffer[128];
  buildButtonPressPayload(ButtonSide::LEFT, 12345, buffer, sizeof(buffer));

  JsonDocument doc;
  deserializeJson(doc, buffer);

  TEST_ASSERT_EQUAL_STRING("L", doc["side"].as<const char*>());
}

void test_button_press_right_side_is_R() {
  char buffer[128];
  buildButtonPressPayload(ButtonSide::RIGHT, 99999, buffer, sizeof(buffer));

  JsonDocument doc;
  deserializeJson(doc, buffer);

  TEST_ASSERT_EQUAL_STRING("R", doc["side"].as<const char*>());
}

void test_button_press_timestamp_correct() {
  char buffer[128];
  buildButtonPressPayload(ButtonSide::LEFT, 12345, buffer, sizeof(buffer));

  JsonDocument doc;
  deserializeJson(doc, buffer);

  TEST_ASSERT_EQUAL(12345, doc["timestamp_ms"].as<int>());
}

void test_button_press_event_is_press() {
  char buffer[128];
  buildButtonPressPayload(ButtonSide::RIGHT, 0, buffer, sizeof(buffer));

  JsonDocument doc;
  deserializeJson(doc, buffer);

  TEST_ASSERT_EQUAL_STRING("press", doc["event"].as<const char*>());
}

void test_button_press_is_valid_json() {
  char buffer[128];
  buildButtonPressPayload(ButtonSide::LEFT, 5000, buffer, sizeof(buffer));

  JsonDocument doc;
  const DeserializationError err = deserializeJson(doc, buffer);
  TEST_ASSERT_EQUAL(DeserializationError::Ok, err.code());
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_heartbeat_has_all_fields);
  RUN_TEST(test_heartbeat_negative_rssi);
  RUN_TEST(test_button_press_left_has_all_fields);
  RUN_TEST(test_button_press_left_side_is_L);
  RUN_TEST(test_button_press_right_side_is_R);
  RUN_TEST(test_button_press_timestamp_correct);
  RUN_TEST(test_button_press_event_is_press);
  RUN_TEST(test_button_press_is_valid_json);
  return UNITY_END();
}
