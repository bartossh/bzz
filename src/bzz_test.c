/// Copyright (c) 2024 Bartosz Lenart

#include "../test-framework/unity.h"
#include "bzz.h"

void setUp(void) {
}

void tearDown(void) {
}

static void
testClassify(void) {
    classify();
}

int main(void)
{
    UnityBegin("bzz_test.c");

    RUN_TEST(testClassify);
   
    return UnityEnd();
}
