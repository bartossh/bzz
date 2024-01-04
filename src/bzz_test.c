/// Copyright (c) 2024 Bartosz Lenart

#include "../test-framework/unity.h"
#include "bzz.h"

void setUp(void) {
}

void tearDown(void) {
}

static void
testDummy(void) {}

int main(void)
{
    UnityBegin("bzz_test.c");

    RUN_TEST(testDummy);
   
    return UnityEnd();
}
