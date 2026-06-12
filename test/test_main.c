#include "vendor/unity.h"
#include "common/hello.h"

static void test_hello_message(void)
{
    TEST_ASSERT_EQUAL_STRING("Hello World!", hello_message());
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_hello_message);
    return UNITY_END();
}
