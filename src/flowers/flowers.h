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

Mat flowers_basic_matrix(void);

#endif
