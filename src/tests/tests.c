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
testCreateFlowersMat_5_10(void) 
{
    
    FlowersDataset fl = flowersDatasetNew(Basic_5_10);
    Mat m = flowersToMat(fl);
    MatPrint(m);
    TEST_ASSERT_EQUAL_size_t(10, m.rows);
    TEST_ASSERT_EQUAL_size_t(6, m.cols);
}

static void
testCreateFlowersMat_5_20(void) 
{
    
    FlowersDataset fl = flowersDatasetNew(Basic_5_20);
    Mat m = flowersToMat(fl);
    MatPrint(m);
    TEST_ASSERT_EQUAL_size_t(20, m.rows);
    TEST_ASSERT_EQUAL_size_t(6, m.cols);
}

static void
testCreateFlowersMat_5_30(void) 
{
    
    FlowersDataset fl = flowersDatasetNew(Basic_5_30);
    Mat m = flowersToMat(fl);
    MatPrint(m);
    TEST_ASSERT_EQUAL_size_t(30, m.rows);
    TEST_ASSERT_EQUAL_size_t(6, m.cols);
}

int main(void)
{
    UnityBegin("bzz_test.c");

    RUN_TEST(testCreateFlowersMat_5_10);
    RUN_TEST(testCreateFlowersMat_5_20);
    RUN_TEST(testCreateFlowersMat_5_30);
   
    return UnityEnd();
}
