// Library from rexim.
//
// Extended by bartossh

#ifndef NN_H_
#define NN_H_

#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

// #define NN_BACKPROP_TRADITIONAL

#ifndef NN_ACT
#define NN_ACT ACT_SIG
#endif // NN_ACT

#ifndef NN_RELU_PARAM
#define NN_RELU_PARAM 0.01f
#endif // NN_RELU_PARAM

#ifndef NN_MALLOC
#include <stdlib.h>
#define NN_MALLOC malloc
#endif // NN_MALLOC

#ifndef NN_ASSERT
#include <assert.h>
#define NN_ASSERT assert
#endif // NN_ASSERT

#define ARRAY_LEN(xs) sizeof((xs))/sizeof((xs)[0])

typedef enum {
    ACT_SIG,
    ACT_RELU,
    ACT_TANH,
    ACT_SIN,
} Act;

float rand_float(void);

float sigmoidf(float x);
float reluf(float x);
float tanhf(float x);

// Dispatch to the corresponding activation function
float actf(float x, Act act);

// Derivative of the activation function based on its value
float dactf(float y, Act act);

typedef struct {
    size_t capacity;
    size_t size;
    uintptr_t *words;
} Region;

// capacity is in bytes, but it can allocate more just to keep things
// word aligned
Region region_alloc_alloc(size_t capacity_bytes);
void *region_alloc(Region *r, size_t size_bytes);
#define region_reset(r) (NN_ASSERT((r) != NULL), (r)->size = 0)
#define region_occupied_bytes(r) (NN_ASSERT((r) != NULL), (r)->size*sizeof(*(r)->words))
#define region_save(r) (NN_ASSERT((r) != NULL), (r)->size)
#define region_rewind(r, s) (NN_ASSERT((r) != NULL), (r)->size = s)

typedef struct {
    size_t rows;
    size_t cols;
    float *elements;
} Mat;

typedef struct {
    size_t cols;
    float *elements;
} Row;

#define ROW_AT(row, col) (row).elements[col]

Mat row_as_mat(Row row);
#define row_alloc(r, cols) mat_row(mat_alloc(r, 1, cols), 0)
Row row_slice(Row row, size_t i, size_t cols);
#define row_rand(row, low, high) mat_rand(row_as_mat(row), low, high)
#define row_fill(row, x) mat_fill(row_as_mat(row), x);
#define row_print(row, name, padding) mat_print(row_as_mat(row), name, padding)
#define row_copy(dst, src) mat_copy(row_as_mat(dst), row_as_mat(src))

#define MAT_AT(m, i, j) (m).elements[(i)*(m).cols + (j)]

Mat mat_alloc(Region *r, size_t rows, size_t cols);
void mat_fill(Mat m, float x);
void mat_rand(Mat m, float low, float high);
Row mat_row(Mat m, size_t row);
void mat_copy(Mat dst, Mat src);
void mat_dot(Mat dst, Mat a, Mat b);
void mat_sum(Mat dst, Mat a);
void mat_act(Mat m);
void mat_print(Mat m, const char *name, size_t padding);
void mat_shuffle_rows(Mat m);
#define MAT_PRINT(m) mat_print(m, #m, 0)

typedef struct {
    size_t *arch;
    size_t arch_count;
    Mat *ws; // The amount of activations is arch_count-1
    Row *bs; // The amount of activations is arch_count-1

    // TODO: maybe remove these? It would be better to allocate them in a
    // temporary region during the actual forwarding
    Row *as;
} NN;

#define NN_INPUT(nn) (NN_ASSERT((nn).arch_count > 0), (nn).as[0])
#define NN_OUTPUT(nn) (NN_ASSERT((nn).arch_count > 0), (nn).as[(nn).arch_count-1])

NN nn_alloc(Region *r, size_t *arch, size_t arch_count);
void nn_zero(NN nn);
void nn_print(NN nn, const char *name);
#define NN_PRINT(nn) nn_print(nn, #nn);
void nn_rand(NN nn, float low, float high);
// TODO: make nn_forward signature more natural
//
// Something more like `Mat nn_forward(NN nn, Mat in)`
void nn_forward(NN nn);
float nn_cost(NN nn, Mat t);
NN nn_finite_diff(Region *r, NN nn, Mat t, float eps);
NN nn_backprop(Region *r, NN nn, Mat t);
void nn_learn(NN nn, NN g, float rate);

typedef struct {
    size_t begin;
    float cost;
    bool finished;
} Batch;

void batch_process(Region *r, Batch *b, size_t batch_size, NN nn, Mat t, float rate);

float sigmoidf(float x);

float reluf(float x);

float tanhf(float x);

float dactf(float y, Act act);

float rand_float(void);

Mat mat_alloc(Region *r, size_t rows, size_t cols);

#endif // NN_H_

