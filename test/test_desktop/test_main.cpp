#include "unity.h"

#ifdef UNIT_TEST

void test_function(void) {
    TEST_ASSERT_EQUAL(1, 0);
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_function);
    UNITY_END();

    return 0;
}

#endif