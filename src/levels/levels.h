/// Copyright (c) 2024 Bartosz Lenart

#ifndef FLOWERS_H
#define FLOWERS_H

#include <stddef.h>
#include "../nn/nn.h"

#define FlowerParamAt(m, i, j) (m.ptr)[(i)*((m).cols) + (j)]

#define FlowerRGBMin 0
#define FlowerRGBMax 255

#define FlowerSizeMin 5
#define FlowerSizeMax 80

#define FlowerPetalsMin 3
#define FlowerPetalsMax 89

#define FlowerLocationMin 0
#define FlowerLocationMax 4

#define FlowerToxicityMin 0
#define FlowerToxicityMax 4


/// ValueNormalizer is a pointer to the function type that is normalizing the dataset column. 
typedef float (*ValueNormalizer)(int v);


/// LevelsDataset is a dataset of flowers.
typedef struct {
    size_t          rows;
    size_t          cols;
    int             *ptr;
    ValueNormalizer *f;
} LevelsDataset;

/// LevelsDatasetOption is a option representing FlowersDataset.
typedef enum {
    Basic_5_10,
    Basic_5_20,
    Basic_5_30,
    Location_6_60
} LevelsDatasetOption;


/// levelsDatasettNew is a factory of LevelsDataset based on provided option,
///
/// o - LevelsDatasetOption option representing the new dataset to be created.
LevelsDataset levelsDatasetNew(LevelsDatasetOption o);

/// levelsGetFlowersCount returns number of flowers in dataset.
///
/// dt - pointer to LevelsDataset.
size_t levelsGetFlowersCount(LevelsDataset *dt);

/// void levelsDatasettFree frees the dataset allocated.
///
/// dt -LevelsDataset to be freed.
void levelsDatasettFree(LevelsDataset *dt);

/// flowersToMat returns dataset matrix with normalized values between 0 and 1.
///
/// dt - LevelsDataset that contains unnormalized flowers dataset. 
Mat flowersToMat(LevelsDataset dt);

/// printToBuffAtRow  prints LevelsDataset into char buff.
/// Returns number of printed characters.
///
/// dt - LevelsDataset to be printed.
/// next - next row to print.
/// buff - buffer to print into.
/// len - len of a buffer.
size_t printToBuffAtRow(LevelsDataset dt, size_t next, char *buff, size_t len);

/// getExpectedValueAtRow returns expected value at row.
///
/// dt - LevelsDataset to get expected value from.
/// row - row number.
float getExpectedValueAtRowNorm(LevelsDataset dt, size_t row);

#endif
