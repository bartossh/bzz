/// Copyright (c) 2024 Bartosz Lenart

#include <stdlib.h>
#include <stddef.h>
#include "flowers.h"
#include "../nn/nn.h"

static float flowers_rgb_normalizer(int v) { return (float)((float)(v-FLOWER_RGB_MIN)/(float)FLOWER_RGB_MAX);}
static float flowers_size_normalizer(int v) { return (float)((float)(v-FLOWER_SIZE_MIN)/(float)FLOWER_SIZE_MAX);}
static float flowers_petals_normalizer(int v) { return (float)((float)(v-FLOWER_PETALS_MIN)/(float)FLOWER_PETALS_MAX);}
static float flowers_toxicity_normalizer(int v) { return (float)((float)v/(float)FLOWER_TOXICITY_MAX);}

typedef float (*value_normalizer)(int v);

const size_t flowers_dataset_basic_rows = 10; // number of known flowers
const size_t flowers_dataset_basic_cols =  6; // number of parameters per flower
/// flowers_dataset_basic represent dataset describing flower toxicity based on parameters [R, G, B, S, P, T].
///
/// - R is red color saturation,
/// - G is a green color saturation,
/// - B is a blue color saturation,
/// - S is a size of a flower,
/// - P is a number of petals,
/// - T is a toxicity level.
const int flowers_dataset_basic[] = {
    255, 255, 255, 20,  8, 0,
    255,   0, 255, 10, 13, 1,
      0, 255, 255, 30, 21, 2,
    255, 102,   0, 10,  8, 0,
      0,  51,   0, 10,  3, 3,
    128,   0, 128, 35, 34, 1,
    255, 255, 153, 45, 55, 0,
      0,  51, 102, 10, 13, 3,
    128,   0,   0, 40, 55, 4,
    102,   0, 102, 20, 34, 2,
};

const value_normalizer vn[] = {
    &flowers_rgb_normalizer, 
    &flowers_rgb_normalizer, 
    &flowers_rgb_normalizer, 
    &flowers_size_normalizer, 
    &flowers_petals_normalizer, 
    &flowers_toxicity_normalizer
};


Mat flowers_basic_matrix(void)
{
    Mat m = mat_alloc(NULL, flowers_dataset_basic_rows, flowers_dataset_basic_cols);
    for (size_t row = 0; row < flowers_dataset_basic_rows; ++row) {
            for (size_t coll = 0; coll < flowers_dataset_basic_cols; ++coll) {
                MAT_AT(m, row, coll) = vn[coll](FLOWER_PARAM_AT(flowers_dataset_basic, flowers_dataset_basic_cols, row, coll));
        }
    }
    return m;
}

