/// Copyright (c) 2024 Bartosz Lenart

#include <stddef.h>
#include <stdio.h>
#include "../../test-framework/unity.h"
#include "../flowers/flowers.h"
#include "../nn/nn.h"

void setUp(void) {
}

void tearDown(void) {
}

static void
testCreateFlowersMat(void) 
{
    Mat m = flowers_basic_matrix();
    mat_print(m, "flowers", 2);
    TEST_ASSERT_EQUAL_size_t(10, m.rows);
    TEST_ASSERT_EQUAL_size_t(6, m.cols);
}

int main(void)
{
    UnityBegin("bzz_test.c");

    RUN_TEST(testCreateFlowersMat);
   
    return UnityEnd();
}
