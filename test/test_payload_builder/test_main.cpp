#include <unity.h>
#include <ArduinoJson.h>

void setUp()    {}
void tearDown() {}

void test_heartbeat_has_all_fields() {
  JsonDocument doc;
  doc["firmware_version"] = "0.1.0";
  doc["uptime_ms"]        = 0;
  doc["wifi_rssi"]        = -70;
  doc["free_heap"]        = 200000;

  TEST_ASSERT_TRUE(doc.containsKey("firmware_version"));
  TEST_ASSERT_TRUE(doc.containsKey("uptime_ms"));
  TEST_ASSERT_TRUE(doc.containsKey("wifi_rssi"));
  TEST_ASSERT_TRUE(doc.containsKey("free_heap"));
}

void test_heartbeat_uptime_zero() {
  JsonDocument doc;
  doc["firmware_version"] = "0.1.0";
  doc["uptime_ms"]        = 0;
  doc["wifi_rssi"]        = -70;
  doc["free_heap"]        = 200000;

  TEST_ASSERT_EQUAL(0, doc["uptime_ms"].as<int>());
}

void test_heartbeat_negative_rssi() {
  JsonDocument doc;
  doc["firmware_version"] = "0.1.0";
  doc["uptime_ms"]        = 1000;
  doc["wifi_rssi"]        = -90;
  doc["free_heap"]        = 200000;

  TEST_ASSERT_EQUAL(-90, doc["wifi_rssi"].as<int>());
}

void test_heartbeat_serializes_to_valid_json() {
  JsonDocument doc;
  doc["firmware_version"] = "0.1.0";
  doc["uptime_ms"]        = 5000;
  doc["wifi_rssi"]        = -65;
  doc["free_heap"]        = 180000;

  char buffer[128];
  const size_t written = serializeJson(doc, buffer);

  TEST_ASSERT_GREATER_THAN(0, written);

  JsonDocument parsed;
  const DeserializationError err = deserializeJson(parsed, buffer);
  TEST_ASSERT_EQUAL(DeserializationError::Ok, err.code());
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_heartbeat_has_all_fields);
  RUN_TEST(test_heartbeat_uptime_zero);
  RUN_TEST(test_heartbeat_negative_rssi);
  RUN_TEST(test_heartbeat_serializes_to_valid_json);
  return UNITY_END();
}