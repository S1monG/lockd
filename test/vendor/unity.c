#include "unity.h"
#include <stdlib.h>

static int unity_tests_run;
static int unity_tests_failed;
static jmp_buf unity_abort;

void UnityBegin(const char *filename)
{
    unity_tests_run = 0;
    unity_tests_failed = 0;
    printf("UNITY test run from %s\n", filename);
}

int UnityEnd(void)
{
    if (unity_tests_failed == 0) {
        printf("OK (%d tests)\n", unity_tests_run);
        return 0;
    }
    printf("FAIL (%d tests, %d failed)\n", unity_tests_run, unity_tests_failed);
    return 1;
}

void UnityDefaultTestRun(void (*test)(void), const char *name, int line)
{
    unity_tests_run++;
    printf("RUN   %s\n", name);
    if (setjmp(unity_abort) == 0) {
        test();
        printf("PASS  %s\n", name);
    }
}

void UnityFail(const char *message, const char *file, int line)
{
    unity_tests_failed++;
    fprintf(stderr, "%s:%d: %s\n", file, line, message);
    longjmp(unity_abort, 1);
}
