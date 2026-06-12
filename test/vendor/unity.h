#ifndef UNITY_H
#define UNITY_H

#include <setjmp.h>
#include <stdio.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

void UnityBegin(const char *filename);
int UnityEnd(void);
void UnityDefaultTestRun(void (*test)(void), const char *name, int line);
void UnityFail(const char *message, const char *file, int line);

#define UNITY_BEGIN() UnityBegin(__FILE__)
#define UNITY_END() UnityEnd()
#define RUN_TEST(test) UnityDefaultTestRun((test), #test, __LINE__)

#define TEST_ASSERT_EQUAL_STRING(expected, actual) \
    do { if (strcmp((expected), (actual)) != 0) UnityFail("Expected string equality", __FILE__, __LINE__); } while (0)

#define TEST_ASSERT_EQUAL_INT(expected, actual) \
    do { if ((expected) != (actual)) UnityFail("Expected int equality", __FILE__, __LINE__); } while (0)

#define TEST_ASSERT_TRUE(expr) \
    do { if (!(expr)) UnityFail("Expected true", __FILE__, __LINE__); } while (0)

#ifdef __cplusplus
}
#endif

#endif // UNITY_H
