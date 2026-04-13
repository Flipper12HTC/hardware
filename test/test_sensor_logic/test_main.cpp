#include <unity.h>
#include "../../src/inputs/sensor.h"

void setUp()    {}
void tearDown() {}

// ─── Drain tests ─────────────────────────────────────────

void test_drain_fast_change_ignored() {
  DrainState s = drainInit();
  SensorEvent ev = drainUpdate(s, false, 0);
  TEST_ASSERT_EQUAL(SensorEvent::NONE, ev);
  ev = drainUpdate(s, false, 20);
  TEST_ASSERT_EQUAL(SensorEvent::NONE, ev);
}

void test_drain_stable_detected() {
  DrainState s = drainInit();
  drainUpdate(s, false, 0);
  SensorEvent ev = drainUpdate(s, false, 60);
  TEST_ASSERT_EQUAL(SensorEvent::DRAIN, ev);
}

void test_drain_no_double_trigger() {
  DrainState s = drainInit();
  drainUpdate(s, false, 0);
  drainUpdate(s, false, 60);
  SensorEvent ev = drainUpdate(s, false, 120);
  TEST_ASSERT_EQUAL(SensorEvent::NONE, ev);
}

// ─── Tilt tests ──────────────────────────────────────────

void test_tilt_one_shake_no_tilt() {
  TiltState s = tiltInit();
  SensorEvent ev = tiltUpdate(s, true, 0);
  TEST_ASSERT_EQUAL(SensorEvent::NONE, ev);
}

void test_tilt_two_shakes_no_tilt() {
  TiltState s = tiltInit();
  tiltUpdate(s, true, 0);
  SensorEvent ev = tiltUpdate(s, true, 100);
  TEST_ASSERT_EQUAL(SensorEvent::NONE, ev);
}

void test_tilt_three_shakes_triggers() {
  TiltState s = tiltInit();
  tiltUpdate(s, true, 0);
  tiltUpdate(s, true, 100);
  SensorEvent ev = tiltUpdate(s, true, 200);
  TEST_ASSERT_EQUAL(SensorEvent::TILT, ev);
}

void test_tilt_window_expired_resets() {
  TiltState s = tiltInit();
  tiltUpdate(s, true, 0);
  tiltUpdate(s, true, 100);
  // Third shake after window expired
  SensorEvent ev = tiltUpdate(s, true, 600);
  TEST_ASSERT_EQUAL(SensorEvent::NONE, ev);
}

void test_tilt_no_shake_no_event() {
  TiltState s = tiltInit();
  SensorEvent ev = tiltUpdate(s, false, 0);
  TEST_ASSERT_EQUAL(SensorEvent::NONE, ev);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_drain_fast_change_ignored);
  RUN_TEST(test_drain_stable_detected);
  RUN_TEST(test_drain_no_double_trigger);
  RUN_TEST(test_tilt_one_shake_no_tilt);
  RUN_TEST(test_tilt_two_shakes_no_tilt);
  RUN_TEST(test_tilt_three_shakes_triggers);
  RUN_TEST(test_tilt_window_expired_resets);
  RUN_TEST(test_tilt_no_shake_no_event);
  return UNITY_END();
}
