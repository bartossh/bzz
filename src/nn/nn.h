/// Copyright (c) 2024 Bartosz Lenart
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

// #define NNBackpropTraditional

#ifndef NNAct
#define NNAct ActSig
#endif // NNAct

#ifndef NNReluParem
#define NNReluParem 0.01f
#endif // NNReluParem

#ifndef NNMalloc
#include <stdlib.h>
#define NNMalloc malloc
#define NNFree free
#endif // NNMalloc

#ifndef NNAssert
#include <assert.h>
#define NNAssert assert
#endif // NNAssert

#define ArrayLen(xs) sizeof((xs))/sizeof((xs)[0])

typedef enum {
    ActSig,
    ActRelu,
    ActThan,
    ActSin,
} Act;

float randFloat(void);

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
Region regionAllocAlloc(size_t capacity_bytes);
void *regionAlloc(Region *r, size_t size_bytes);
#define RegionReset(r) (NNAssert((r) != NULL), (r)->size = 0)
#define RegionOccupiedBytes(r) (NNAssert((r) != NULL), (r)->size*sizeof(*(r)->words))
#define RegionSave(r) (NNAssert((r) != NULL), (r)->size)
#define RegionRewind(r, s) (NNAssert((r) != NULL), (r)->size = s)

typedef struct {
    size_t rows;
    size_t cols;
    float *elements;
} Mat;

typedef struct {
    size_t cols;
    float *elements;
} Row;

#define RowAt(row, col) (row).elements[col]

Mat rowAsMat(Row row);
#define RowAlloc(r, cols) matRow(matAlloc(r, 1, cols), 0)
Row rowSlice(Row row, size_t i, size_t cols);
#define RowRand(row, low, high) matRand(rowAsMat(row), low, high)
#define RowFill(row, x) matFill(rowAsMat(row), x);
#define RowPrint(row, name, padding) matPrintf(rowAsMat(row), name, padding)
#define RowCopy(dst, src) matCopy(rowAsMat(dst), rowAsMat(src))

#define MatAt(m, i, j) (m).elements[(i)*(m).cols + (j)]

Mat matAlloc(Region *r, size_t rows, size_t cols);
void matFill(Mat m, float x);
void matRand(Mat m, float low, float high);
Row matRow(Mat m, size_t row);
void matCopy(Mat dst, Mat src);
void matDot(Mat dst, Mat a, Mat b);
void matSum(Mat dst, Mat a);
void matAct(Mat m);
void matPrintf(Mat m, const char *name, size_t padding);
void matShuffleRows(Mat m);
#define MatPrint(m) matPrintf(m, #m, 0)

typedef struct {
    size_t *arch;
    size_t arch_count;
    Mat *ws; // The amount of activations is arch_count-1
    Row *bs; // The amount of activations is arch_count-1

    // TODO: maybe remove these? It would be better to allocate them in a
    // temporary region during the actual forwarding
    Row *as;
} NN;

#define NNInput(nn) (NNAssert((nn).arch_count > 0), (nn).as[0])
#define NNOutout(nn) (NNAssert((nn).arch_count > 0), (nn).as[(nn).arch_count-1])

NN nnAlloc(Region *r, size_t *arch, size_t arch_count);
void nnZero(NN nn);
void nnPrint(NN nn, const char *name);
#define NNPrint(nn) nnPrint(nn, #nn);
void nnRand(NN nn, float low, float high);
void nnForward(NN nn);
float nnCost(NN nn, Mat t);
NN nnFiniteDiff(Region *r, NN nn, Mat t, float eps);
NN nnBackprop(Region *r, NN nn, Mat t);
void nnLearn(NN nn, NN g, float rate);

typedef struct {
    size_t begin;
    float cost;
    bool finished;
} Batch;

#endif // NN_H_

