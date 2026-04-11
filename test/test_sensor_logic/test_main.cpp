#include <unity.h>

void setUp()    {}
void tearDown() {}

void test_placeholder() {
  TEST_ASSERT_TRUE(true);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_placeholder);
  return UNITY_END();
}
