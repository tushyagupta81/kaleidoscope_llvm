#include "unity.h"
#include "tests.h"

void setUp() {}
void tearDown() {}

int main() {
  UNITY_BEGIN();
  RUN_TEST(random_test);
  return UNITY_END();
}
