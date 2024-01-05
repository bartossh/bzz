/// Copyright (c) 2024 Bartosz Lenart

#include <stdlib.h>
#include <stddef.h>
#include "flowers.h"
#include "../nn/nn.h"

#define CALC_FLOWERS_DATASET_ROWS(set, cols) sizeof(set) / (sizeof(set[0])*cols)

static float flowers_rgb_normalizer(int v) { return (float)((float)(v-FLOWER_RGB_MIN)/(float)FLOWER_RGB_MAX);}
static float flowers_size_normalizer(int v) { return (float)((float)(v-FLOWER_SIZE_MIN)/(float)FLOWER_SIZE_MAX);}
static float flowers_petals_normalizer(int v) { return (float)((float)(v-FLOWER_PETALS_MIN)/(float)FLOWER_PETALS_MAX);}
static float flowers_toxicity_normalizer(int v) { return (float)((float)v/(float)FLOWER_TOXICITY_MAX);}

const size_t flowers_dataset_basic_cols =  6; // number of parameters per flower

const value_normalizer value_normalizer_basic[6] = {
    &flowers_rgb_normalizer, 
    &flowers_rgb_normalizer, 
    &flowers_rgb_normalizer, 
    &flowers_size_normalizer, 
    &flowers_petals_normalizer, 
    &flowers_toxicity_normalizer
};

/// flowers_dataset_basic_5_10 represent dataset describing flower toxicity based on parameters [R, G, B, S, P, T].
///
/// - R is red color saturation,
/// - G is a green color saturation,
/// - B is a blue color saturation,
/// - S is a size of a flower,
/// - P is a number of petals,
/// - T is a toxicity level.
const int flowers_dataset_basic_5_10[60] = {
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

/// flowers_dataset_basic_5_20 represent dataset describing flower toxicity based on parameters [R, G, B, S, P, T].
///
/// - R is red color saturation,
/// - G is a green color saturation,
/// - B is a blue color saturation,
/// - S is a size of a flower,
/// - P is a number of petals,
/// - T is a toxicity level.
const int flowers_dataset_basic_5_20[120] = {
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

/// flowers_dataset_basic_5_20 represent dataset describing flower toxicity based on parameters [R, G, B, S, P, T].
///
/// - R is red color saturation,
/// - G is a green color saturation,
/// - B is a blue color saturation,
/// - S is a size of a flower,
/// - P is a number of petals,
/// - T is a toxicity level.
const int flowers_dataset_basic_5_30[180] = {
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

FlowersDataset flowers_dataset_new(FlowersDatasetOption o)
{
    int *dataset;
    value_normalizer *f;
    size_t cols = 0;
    size_t rows = 0;
    switch (o) {
    case Basic_5_10:
        dataset = (int *)flowers_dataset_basic_5_10;
        f = (value_normalizer *)value_normalizer_basic;
        cols = flowers_dataset_basic_cols;
        rows = CALC_FLOWERS_DATASET_ROWS(flowers_dataset_basic_5_10, cols);
        break;
    case Basic_5_20:
        dataset = (int *)flowers_dataset_basic_5_20;
        f = (value_normalizer *)value_normalizer_basic;
        cols = flowers_dataset_basic_cols;
        rows = CALC_FLOWERS_DATASET_ROWS(flowers_dataset_basic_5_20, cols);
        break;
    case Basic_5_30:
        dataset = (int *)flowers_dataset_basic_5_30;
        f = (value_normalizer *)value_normalizer_basic;
        cols = flowers_dataset_basic_cols;
        rows = CALC_FLOWERS_DATASET_ROWS(flowers_dataset_basic_5_30, cols);
        break;

    default:
        dataset = (int *)flowers_dataset_basic_5_10;
        f = (value_normalizer *)value_normalizer_basic;
        cols = flowers_dataset_basic_cols;
        rows = CALC_FLOWERS_DATASET_ROWS(flowers_dataset_basic_5_10, cols);
    }

    size_t len = rows*cols;
    FlowersDataset dt = {
        .rows = rows,
        .cols = cols,
        .ptr = malloc(sizeof(dataset[0])*len),
        .f = malloc(sizeof(value_normalizer)*cols)
    };

    for (size_t i = 0; i < len; ++i) {
        dt.ptr[i] = dataset[i];
    }

    for (size_t i = 0; i < cols; ++i) {
        dt.f[i] = f[i];
    }

    return dt;
}

Mat flowers_to_mat(FlowersDataset dt)
{
    Mat m = mat_alloc(NULL, dt.rows, dt.cols);
    for (size_t row = 0; row < dt.rows; ++row) {
            for (size_t coll = 0; coll < dt.cols; ++coll) {
                MAT_AT(m, row, coll) = dt.f[coll](FLOWER_PARAM_AT(dt.ptr, dt.cols, row, coll));
        }
    }
    return m;
}

