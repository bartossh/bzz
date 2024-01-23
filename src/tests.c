/// Copyright (c) 2024 Bartosz Lenart

#include <stddef.h>
#include <stdio.h>
#include <raylib.h>
#include "nn/nn.h"
#include "game/game.h"
#include "levels/levels.h"
#include "../test-framework/unity.h"

#define BuffLen(xs) sizeof((xs))/sizeof((xs)[0])

void setUp(void) {
}

void tearDown(void) {
}

static void
testCreateFlowersMat_5_10(void) 
{
    
    LevelsDataset fl = levelsDatasetNew(Basic_5_10);
    Mat m = flowersToMat(fl);
    MatPrint(m);
    TEST_ASSERT_EQUAL_size_t(10, m.rows);
    TEST_ASSERT_EQUAL_size_t(6, m.cols);
}

static void
testCreateFlowersMat_5_20(void) 
{
    
    LevelsDataset fl = levelsDatasetNew(Basic_5_20);
    Mat m = flowersToMat(fl);
    MatPrint(m);
    TEST_ASSERT_EQUAL_size_t(20, m.rows);
    TEST_ASSERT_EQUAL_size_t(6, m.cols);
}

static void
testCreateFlowersMat_5_30(void) 
{
    
    LevelsDataset fl = levelsDatasetNew(Basic_5_30);
    Mat m = flowersToMat(fl);
    MatPrint(m);
    TEST_ASSERT_EQUAL_size_t(30, m.rows);
    TEST_ASSERT_EQUAL_size_t(6, m.cols);
}

static void
testCreateSwarm(void) 
{
    BzzSwarm s = bzzSwarmNew();
    TEST_ASSERT_EQUAL_size_t(BuffLen(s.buffer), MAX_SWARN_SIZE);
    TEST_ASSERT_EQUAL_size_t(s.swarm_size, 0);
}

static void
testCreateCRUDSwarm(void)
{
    // START: prepare window and graphic.
    size_t WindowFactor = 100;
    size_t WindowWidth = (16 * WindowFactor);
    size_t WindowHeight = (9 * WindowFactor);
    InitWindow(WindowWidth, WindowHeight, "BZZ!");
    int w = GetScreenWidth();
    int h = GetScreenHeight();
    // END: prepare window and graphic.

    // START: test append in range. 
    const int max_elems = MAX_SWARN_SIZE;
    BzzSwarm s = bzzSwarmNew();
    BzzObject obj = bzzObjectNewBee(ORANGE);
    for (size_t i = 0; i < max_elems; i++) {
        BzzAnimated a = bzzAnimatedNewBee(obj, (Vector2){0.1f, 0.1f * (float)i}, (Vector2){0.0f, 0.0f}, (Vector2){w, h}, TopDown);
        bool result =  bzzSwarmAppend(&s, a); 
        TEST_ASSERT_TRUE(result);
    }
    // END: test append in range. 

    // START: test append outside the range.
    BzzAnimated a = bzzAnimatedNewBee(obj, (Vector2){0.1f, 0.1f}, (Vector2){9.0f, 9.0f}, (Vector2){w, h}, TopDown);
    bool result =  bzzSwarmAppend(&s, a); 
    TEST_ASSERT_FALSE(result);
    // END: test append outside the range.
    
    // START: test number of entities.
    int size =  bzzSwarmGetSize(&s);
    TEST_ASSERT_EQUAL_INT(max_elems, size);
    // END: test number of entities.
    
    // START: test get at index.
    for (size_t i = 0; i < max_elems; i++) {
        BzzAnimated *anim =  bzzSwarmAt(&s, (int)i);
        TEST_ASSERT_EQUAL_FLOAT(0.1f * (float)i, anim->pos.y);
    }
    // END: test get at index.

    // START: remove at index in range.
    result = bzzSwarmRemoveAt(&s, 9);
    TEST_ASSERT_TRUE(result);
    size =  bzzSwarmGetSize(&s);
    TEST_ASSERT_EQUAL_INT(max_elems-1, size);
    int j = 0;
    for (size_t i = 0; i < max_elems-1; i++) {
        BzzAnimated *anim =  bzzSwarmAt(&s, (int)i);
        if (i == 9) {
            j++;
        }
        TEST_ASSERT_EQUAL_FLOAT(0.1f * (float)j, anim->pos.y);
        j++;
    }
    // END: remove at index in range.

    // START: cleanup 
    bzzUnloadObject(obj);
    CloseWindow();
    // END: cleanup 
}

int main(void)
{
    UnityBegin("bzz_test.c");

    RUN_TEST(testCreateFlowersMat_5_10);
    RUN_TEST(testCreateFlowersMat_5_20);
    RUN_TEST(testCreateFlowersMat_5_30);
    
    RUN_TEST(testCreateSwarm);
    RUN_TEST(testCreateCRUDSwarm);
   
    return UnityEnd();
}
