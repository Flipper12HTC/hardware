#include <unity.h>
#include "../../src/inputs/button.h"

void setUp()    {}
void tearDown() {}

void test_fast_change_ignored() {
  ButtonState s = buttonInit(ButtonSide::LEFT);
  ButtonEvent ev = buttonUpdate(s, false, 0);
  TEST_ASSERT_EQUAL(ButtonEvent::NONE, ev);
  ev = buttonUpdate(s, false, 10);
  TEST_ASSERT_EQUAL(ButtonEvent::NONE, ev);
}

void test_stable_press_detected() {
  ButtonState s = buttonInit(ButtonSide::LEFT);
  buttonUpdate(s, false, 0);
  ButtonEvent ev = buttonUpdate(s, false, 20);
  TEST_ASSERT_EQUAL(ButtonEvent::PRESS, ev);
}

void test_stable_release_detected() {
  ButtonState s = buttonInit(ButtonSide::RIGHT);
  buttonUpdate(s, false, 0);
  buttonUpdate(s, false, 20);
  buttonUpdate(s, true, 30);
  ButtonEvent ev = buttonUpdate(s, true, 50);
  TEST_ASSERT_EQUAL(ButtonEvent::RELEASE, ev);
}

void test_no_double_press() {
  ButtonState s = buttonInit(ButtonSide::LEFT);
  buttonUpdate(s, false, 0);
  buttonUpdate(s, false, 20);
  ButtonEvent ev = buttonUpdate(s, false, 40);
  TEST_ASSERT_EQUAL(ButtonEvent::NONE, ev);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_fast_change_ignored);
  RUN_TEST(test_stable_press_detected);
  RUN_TEST(test_stable_release_detected);
  RUN_TEST(test_no_double_press);
  return UNITY_END();
}