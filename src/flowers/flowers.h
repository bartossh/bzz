/// Copyright (c) 2024 Bartosz Lenart

#ifndef FLOWERS_H
#define FLOWERS_H

#include <stddef.h>
#include "../nn/nn.h"

#define FlowerParamAt(m, cols, i, j) (m)[(i)*cols + (j)]

#define FlowerRGBMin 0
#define FlowerRGBMax 255

#define FlowerSizeMin 5
#define FlowerSizeMax 80

#define FlowerPetalsMin 3
#define FlowerPetalsMax 89

#define FlowerToxicityMin 0
#define FlowerToxicityMax 4

/// ValueNormalizer is a pointer to the function type that is normalizing the dataset column. 
typedef float (*ValueNormalizer)(int v);


/// FlowersDataset is a dataset of flowers.
typedef struct {
    size_t rows;
    size_t cols;
    int *ptr;
    ValueNormalizer *f;
} FlowersDataset;

/// FlowersDatasetOption is a option representing FlowersDataset.
typedef enum {
    Basic_5_10,
    Basic_5_20,
    Basic_5_30
} FlowersDatasetOption;


/// flowersDatasetNew is a factory of FlowersDataset based on provided option,
///
/// o - FlowersDatasetOption option representing the new dataset to be created.
FlowersDataset flowersDatasetNew(FlowersDatasetOption o);

/// flowersToMat returns dataset matrix with normalized values between 0 and 1.
///
/// dt - FlowersDataset that contains unnormalized flowers dataset. 
Mat flowersToMat(FlowersDataset dt);


#endif
