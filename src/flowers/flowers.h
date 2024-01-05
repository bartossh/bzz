/// Copyright (c) 2024 Bartosz Lenart

#ifndef FLOWERS_H
#define FLOWERS_H

#include <stddef.h>
#include "../nn/nn.h"

#define FLOWER_PARAM_AT(m, cols, i, j) (m)[(i)*cols + (j)]

#define FLOWER_RGB_MIN 0
#define FLOWER_RGB_MAX 255

#define FLOWER_SIZE_MIN 5
#define FLOWER_SIZE_MAX 80

#define FLOWER_PETALS_MIN 3
#define FLOWER_PETALS_MAX 89

#define FLOWER_TOXICITY_MIN 0
#define FLOWER_TOXICITY_MAX 4

/// value_normalizer is a pointer to the function type that is normalizing the dataset column. 
typedef float (*value_normalizer)(int v);


/// FlowersDataset is a dataset of flowers.
typedef struct {
    size_t rows;
    size_t cols;
    int *ptr;
    value_normalizer *f;
} FlowersDataset;

/// FlowersDatasetOption is a option representing FlowersDataset.
typedef enum {
    Basic_5_10,
    Basic_5_20,
    Basic_5_30
} FlowersDatasetOption;


/// flowers_dataset_new is a factory of FlowersDataset based on provided option,
///
/// o - FlowersDatasetOption option representing the new dataset to be created.
FlowersDataset flowers_dataset_new(FlowersDatasetOption o);

/// flowers_to_mat returns dataset matrix with normalized values between 0 and 1.
///
/// dt - FlowersDataset that contains unnormalized flowers dataset. 
Mat flowers_to_mat(FlowersDataset dt);


#endif
