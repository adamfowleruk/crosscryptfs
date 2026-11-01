#pragma once
#ifndef PROJECT_TEST_H
#define PROJECT_TEST_H

// Define the test result strings
#define PASS "PASS"
#define FAIL "FAIL"

#include <stdio.h>
#include <string.h>
#include <math.h>

// Initialize the test counter
static int count = 1;

int assertTrue(int condition) {
    printf("#%d: %s\n", count, condition ? PASS : FAIL);
    if (!condition) {
        printf("#%d: had true, wanted false\n", count);
        count++;
        return 1;
    }

    count++;
    return 0;
}

int assertFalse(int condition) {
    printf("#%d: %s\n", count, !condition ? PASS : FAIL);
    if (condition) {
        printf("#%d: had false, wanted true\n", count);
        count++;
        return 1;
    }

    count++;
    return 0;
}

int assertNull(void* ptr) {
    printf("#%d: %s\n", count, ptr == 0 ? PASS : FAIL);
    if (ptr != 0) {
        printf("#%d: why is <%p> not null\n", count, ptr);
        count++;
        return 1;
    }

    count++;
    return 0;
}

int assertNotNull(void* ptr) {
    printf("#%d: %s\n", count, ptr != 0 ? PASS : FAIL);
    if (ptr == 0) {
        printf("#%d: this pointer shouldn't be null bro\n", count);
        count++;
        return 1;
    }

    count++;
    return 0;
}

int assertEquals(int a, int b) {
    printf("#%d: %s\n", count, a == b ? PASS : FAIL);
    if (a != b) {
        printf("#%d: had <%d>, wanted <%d>\n", count, a, b);
        count++;
        return 1;
    }

    count++;
    return 0;
}

int assertFloatEquals(float a, float b) {
    // use fabs for float comparison because of precision issues
    printf("#%d: %s\n", count, fabs(a - b) < 0.0001f ? PASS : FAIL);
    if (fabs(a - b) >= 0.0001f) {
        printf("#%d: had <%f>, wanted <%f>\n", count, a, b);
        count++;
        return 1;
    }

    count++;
    return 0;
}

int assertDoubleEquals(double a, double b) {
    // use fabs for double comparison because of precision issues
    printf("#%d: %s\n", count, fabs(a - b) < 0.0001 ? PASS : FAIL);
    if (fabs(a - b) >= 0.0001) {
        printf("#%d: had <%f>, wanted <%f>\n", count, a, b);
        count++;
        return 1;
    }

    count++;
    return 0;
}

int assertNotEquals(int a, int b) {
    printf("#%d: %s\n", count, a != b ? PASS : FAIL);
    if (a == b) {
        printf("#%d: had <%d>, didn't want <%d>\n", count, a, b);
        count++;
        return 1;
    }

    count++;
    return 0;
}

int assertGreaterThan(int a, int b) {
    printf("#%d: %s\n", count, a > b ? PASS : FAIL);
    if (a <= b) {
        printf("#%d: wanted <%d> to be greater than <%d>\n", count, a, b);
        count++;
        return 1;
    }

    count++;
    return 0;
}

int assertLessThan(int a, int b) {
    printf("#%d: %s\n", count, a < b ? PASS : FAIL);
    if (a >= b) {
        printf("#%d: wanted <%d> to be less than <%d>\n", count, a, b);
        count++;
        return 1;
    }

    count++;
    return 0;
}

int assertIn(double value, double min, double max) {
    printf("#%d: %s\n", count, value >= min && value <= max ? PASS : FAIL);
    if (value < min || value > max) {
        printf("#%d: expected <%f> to be in range <%f> to <%f>\n", count, value, min, max);
        count++;
        return 1;
    }

    count++;
    return 0;
}

int assertPointersMatch(void* a, void* b) {
    printf("#%d: %s\n", count, a == b ? PASS : FAIL);
    if (a != b) {
        printf("#%d: expected pointers <%p> and <%p> to match\n", count, a, b);
        count++;
        return 1;
    }

    count++;
    return 0;
}

int assertStringsMatch(char* a, char* b) {
    if (a == 0 || b == 0) {
        printf("#%d: %s\n", count, a == b ? PASS : FAIL);
        if (a != b) {
            printf("#%d: expected strings <%s> and <%s> to match\n", count, a, b);
            count++;
            return 1;
        }
    }

    printf("#%d: %s\n", count, strcmp(a, b) == 0 ? PASS : FAIL);
    if (strcmp(a, b) != 0) {
        printf("#%d: expected strings <%s> and <%s> to match\n", count, a, b);
        count++;
        return 1;
    }

    count++;
    return 0;
}

#endif