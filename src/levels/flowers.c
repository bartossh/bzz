/// Copyright (c) 2024 Bartosz Lenart

#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "levels.h"
#include "../nn/nn.h"

#define CalcLevelsDatasetRows(set, cols) sizeof(set) / (sizeof(set[0])*cols)

static float flowersRGBNormalizer(int v) { return (float)((float)(v-FlowerRGBMin)/(float)FlowerRGBMax);}
static float flowersSizeNormalizer(int v) { return (float)((float)(v-FlowerSizeMin)/(float)FlowerSizeMax);}
static float flowersPetalsNormalizer(int v) { return (float)((float)(v-FlowerPetalsMin)/(float)FlowerPetalsMax);}
static float flowersLocationNormalizer(int v) { return (float)((float)(v-FlowerLocationMin)/(float)FlowerLocationMax);}
static float flowersToxicityNormalizer(int v) { return (float)((float)v/(float)FlowerToxicityMax);}

const size_t levelsDatasetToxicityBasic_cols =  6; // number of parameters per flower

const ValueNormalizer value_normalizer_basic[6] = {
    &flowersRGBNormalizer, 
    &flowersRGBNormalizer, 
    &flowersRGBNormalizer, 
    &flowersSizeNormalizer, 
    &flowersPetalsNormalizer, 
    &flowersToxicityNormalizer
};

/// levelsDatasetToxicityBasic_5_10 represent dataset describing flower toxicity based on parameters [R, G, B, S, P, T].
///
/// - R is red color saturation,
/// - G is a green color saturation,
/// - B is a blue color saturation,
/// - S is a size of a flower,
/// - P is a number of petals,
/// - T is a toxicity level.
const int levelsDatasetToxicityBasic_5_10[60] = {
    255, 255, 255, 10,  8, 0,
    255,   0, 255, 20, 13, 0,
      0, 255, 255, 20, 21, 0,
    255, 102,   0, 20,  8, 0,
      0,  51,   0, 20,  3, 1,
    128,   0, 128, 30, 34, 1,
    255, 255, 153, 30, 55, 1,
      0,  51, 102, 35, 13, 2,
    128,   0,   0, 35, 55, 2,
    102,   0, 102, 20, 34, 3,
};

/// levelsDatasetToxicityBasic_5_20 represent dataset describing flower toxicity based on parameters [R, G, B, S, P, T].
///
/// - R is red color saturation,
/// - G is a green color saturation,
/// - B is a blue color saturation,
/// - S is a size of a flower,
/// - P is a number of petals,
/// - T is a toxicity level.
const int levelsDatasetToxicityBasic_5_20[120] = {
    255, 255, 255, 10,  8, 0,
    255,   0, 255, 20, 13, 0,
      0, 255, 255, 20, 21, 0,
    255, 102,   0, 20,  8, 0,
      0,  51,   0, 20,  3, 1,
    128,   0, 128, 30, 34, 1,
    255, 255, 153, 30, 55, 1,
      0,  51, 102, 35, 13, 2,
    128,   0,   0, 35, 55, 2,
    102,   0, 102, 20, 34, 3,
    255, 255, 255, 10, 55, 0,
    255,   0, 255, 20, 55, 0,
      0, 255, 255, 20, 55, 0,
    255, 102,   0, 20, 89, 0,
      0,  51,   0, 20, 55, 1,
    128,   0, 128, 30, 89, 1,
    255, 255, 153, 30, 89, 1,
      0,  51, 102, 35, 89, 2,
    128,   0,   0, 35, 89, 2,
    102,   0, 102, 20, 89, 3,
};

///  levelsDatasetToxicityBasic_5_30 represent dataset describing flower toxicity based on parameters [R, G, B, S, P, T].
///
/// - R is red color saturation,
/// - G is a green color saturation,
/// - B is a blue color saturation,
/// - S is a size of a flower,
/// - P is a number of petals,
/// - T is a toxicity level.
const int levelsDatasetToxicityBasic_5_30[180] = {
    255, 255, 255, 10,  8, 0,
    255,   0, 255, 20, 13, 0,
      0, 255, 255, 20, 21, 0,
    255, 102,   0, 20,  8, 0,
      0,  51,   0, 20,  3, 1,
    128,   0, 128, 30, 34, 1,
    255, 255, 153, 30, 55, 1,
      0,  51, 102, 35, 13, 2,
    128,   0,   0, 35, 55, 2,
    102,   0, 102, 20, 34, 3,
    255, 255, 255, 10, 55, 1,
    255,   0, 255, 20, 55, 1,
      0, 255, 255, 20, 55, 1,
    255, 102,   0, 20, 89, 1,
      0,  51,   0, 20, 55, 2,
    128,   0, 128, 30, 89, 2,
    255, 255, 153, 30, 89, 2,
      0,  51, 102, 35, 89, 3,
    128,   0,   0, 35, 89, 3,
    102,   0, 102, 20, 89, 4,
    255, 255, 255, 15, 55, 2,
    255,   0, 255, 25, 55, 2,
      0, 255, 255, 25, 55, 2,
    255, 102,   0, 25, 89, 2,
      0,  51,   0, 25, 55, 3,
    128,   0, 128, 35, 89, 3,
    255, 255, 153, 35, 89, 4,
      0,  51, 102, 40, 89, 4,
    128,   0,   0, 45, 89, 4,
    102,   0, 102, 55, 89, 4,
};

const size_t levelsDatasetToxicityLocation_cols =  7; // number of parameters per flower

const ValueNormalizer value_normalizer_location[7] = {
    &flowersRGBNormalizer, 
    &flowersRGBNormalizer, 
    &flowersRGBNormalizer, 
    &flowersSizeNormalizer, 
    &flowersPetalsNormalizer, 
    &flowersLocationNormalizer, 
    &flowersToxicityNormalizer
};

/// levelsDatasetToxicityBasic_6_30 represent dataset describing flower toxicity based on parameters [R, G, B, S, P, L, T].
///
/// - R is red color saturation,
/// - G is a green color saturation,
/// - B is a blue color saturation,
/// - S is a size of a flower,
/// - P is a number of petals,
/// - L is a location indicator,
/// - T is a toxicity level.
const int levelsDatasetToxicityLocation_6_60[420] = {
    255, 255, 255, 10,  8, 1, 0,
    255,   0, 255, 20, 13, 1, 0,
      0, 255, 255, 20, 21, 1, 0,
    255, 102,   0, 20,  8, 1, 0,
      0,  51,   0, 20,  3, 1, 1,
    128,   0, 128, 30, 34, 1, 1,
    255, 255, 153, 30, 55, 1, 1,
      0,  51, 102, 35, 13, 1, 2,
    128,   0,   0, 35, 55, 1, 2,
    102,   0, 102, 20, 34, 1, 3,
    255, 255, 255, 10, 55, 1, 1,
    255,   0, 255, 20, 55, 1, 1,
      0, 255, 255, 20, 55, 1, 1,
    255, 102,   0, 20, 89, 1, 1,
      0,  51,   0, 20, 55, 1, 2,
    128,   0, 128, 30, 89, 1, 2,
    255, 255, 153, 30, 89, 1, 2,
      0,  51, 102, 35, 89, 1, 3,
    128,   0,   0, 35, 89, 1, 3,
    102,   0, 102, 20, 89, 1, 4,
    255, 255, 255, 15, 55, 1, 2,
    255,   0, 255, 25, 55, 1, 2,
      0, 255, 255, 25, 55, 1, 2,
    255, 102,   0, 25, 89, 1, 2,
      0,  51,   0, 25, 55, 1, 3,
    128,   0, 128, 35, 89, 1, 3,
    255, 255, 153, 35, 89, 1, 4,
      0,  51, 102, 40, 89, 1, 4,
    128,   0,   0, 45, 89, 1, 4,
    102,   0, 102, 55, 89, 1, 4,
    255, 255, 255, 10,  8, 2, 1,
    255,   0, 255, 20, 13, 2, 1,
      0, 255, 255, 20, 21, 2, 1,
    255, 102,   0, 20,  8, 2, 1,
      0,  51,   0, 20,  3, 2, 2,
    128,   0, 128, 30, 34, 2, 2,
    255, 255, 153, 30, 55, 2, 2,
      0,  51, 102, 35, 13, 2, 2,
    128,   0,   0, 35, 55, 2, 2,
    102,   0, 102, 20, 34, 2, 4,
    255, 255, 255, 10, 55, 2, 2,
    255,   0, 255, 20, 55, 2, 2,
      0, 255, 255, 20, 55, 2, 2,
    255, 102,   0, 20, 89, 2, 2,
      0,  51,   0, 20, 55, 2, 3,
    128,   0, 128, 30, 89, 2, 3,
    255, 255, 153, 30, 89, 2, 3,
      0,  51, 102, 35, 89, 2, 4,
    128,   0,   0, 35, 89, 2, 4,
    102,   0, 102, 20, 89, 2, 4,
    255, 255, 255, 15, 55, 2, 3,
    255,   0, 255, 25, 55, 2, 3,
      0, 255, 255, 25, 55, 2, 3,
    255, 102,   0, 25, 89, 2, 4,
      0,  51,   0, 25, 55, 2, 4,
    128,   0, 128, 35, 89, 2, 4,
    255, 255, 153, 35, 89, 2, 4,
      0,  51, 102, 40, 89, 2, 4,
    128,   0,   0, 45, 89, 2, 4,
    102,   0, 102, 55, 89, 2, 4,
};

LevelsDataset levelsDatasetNew(LevelsDatasetOption o)
{
    int *dataset;
    ValueNormalizer *f;
    size_t cols = 0;
    size_t rows = 0;
    switch (o) {
    case Basic_5_10:
        dataset = (int *)levelsDatasetToxicityBasic_5_10;
        f = (ValueNormalizer *)value_normalizer_basic;
        cols = levelsDatasetToxicityBasic_cols;
        rows = CalcLevelsDatasetRows(levelsDatasetToxicityBasic_5_10, cols);
        break;
    case Basic_5_20:
        dataset = (int *)levelsDatasetToxicityBasic_5_20;
        f = (ValueNormalizer *)value_normalizer_basic;
        cols = levelsDatasetToxicityBasic_cols;
        rows = CalcLevelsDatasetRows(levelsDatasetToxicityBasic_5_20, cols);
        break;
    case Basic_5_30:
        dataset = (int *)levelsDatasetToxicityBasic_5_30;
        f = (ValueNormalizer *)value_normalizer_basic;
        cols = levelsDatasetToxicityBasic_cols;
        rows = CalcLevelsDatasetRows(levelsDatasetToxicityBasic_5_30, cols);
        break;
    case Location_6_60:
        dataset = (int *)levelsDatasetToxicityLocation_6_60;
        f = (ValueNormalizer *)value_normalizer_location;
        cols = levelsDatasetToxicityLocation_cols;
        rows = CalcLevelsDatasetRows(levelsDatasetToxicityLocation_6_60, cols);
        break;

    default:
        dataset = (int *)levelsDatasetToxicityBasic_5_10;
        f = (ValueNormalizer *)value_normalizer_basic;
        cols = levelsDatasetToxicityBasic_cols;
        rows = CalcLevelsDatasetRows(levelsDatasetToxicityBasic_5_10, cols);
    }

    size_t len = rows*cols;
    LevelsDataset dt = {
        .rows = rows,
        .cols = cols,
        .ptr = NNMalloc(sizeof(dataset[0])*len),
        .f = NNMalloc(sizeof(ValueNormalizer)*cols)
    };

    for (size_t i = 0; i < len; ++i) {
        dt.ptr[i] = dataset[i];
    }

    for (size_t i = 0; i < cols; ++i) {
        dt.f[i] = f[i];
    }

    return dt;
}

size_t levelsGetFlowersCount(LevelsDataset *dt)
{
    if (!dt) {
        return 0;
    }
    return dt->rows;
}

void levelsDatasetFree(LevelsDataset *dt)
{
    if (!dt) {
        return;
    }
    free(dt->ptr);
    dt->ptr = NULL;
    free(dt->f);
    dt->f = NULL;
    dt->rows = 0;
    dt->cols = 0;
}

Mat flowersToMat(LevelsDataset dt)
{
    Mat m = matAlloc(NULL, dt.rows, dt.cols);
    for (size_t row = 0; row < dt.rows; ++row) {
            for (size_t coll = 0; coll < dt.cols; ++coll) {
                MatAt(m, row, coll) = dt.f[coll](FlowerParamAt(dt, row, coll));
        }
    }
    return m;
}

size_t printToBuffAtRow(LevelsDataset dt, size_t next, char *buff, size_t len)
{
    if (next >= dt.rows) {
        return 0;
    }

    size_t move = 0;
    for (size_t coll = 0; coll < dt.cols; ++coll) {
        char inner_buff[10];
        int v = FlowerParamAt(dt, next, coll);
        if (coll < dt.cols-1) {
            if (v == 0 ) {
                snprintf(inner_buff, 10, "|  0|");
            } else { 
                snprintf(inner_buff, 10, "|%3.d|", v);
            }
        } else {
            snprintf(inner_buff, 10, " [%.3f]",  dt.f[coll](v));
        }
        size_t inner_len = strlen(inner_buff);

        if (move + inner_len >= len) {
            return move;
        }


        strncpy(&buff[move], inner_buff, inner_len);
        move += inner_len;
    }

    return move;
}

float getExpectedValueAtRowNorm(LevelsDataset dt, size_t row)
{
    if (row >= dt.rows) {
        return 0.0f;
    }
    return dt.f[dt.cols-1](FlowerParamAt(dt, row, dt.cols-1));
}

