/// Copyright (c) 2024 Bartosz Lenart

#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include "nn.h"

float sigmoidf(float x)
{
    return 1.f / (1.f + expf(-x));
}

float reluf(float x)
{
    return x > 0 ? x : x*NNReluParem;
}

float tanhf(float x)
{
    float ex = expf(x);
    float enx = expf(-x);
    return (ex - enx)/(ex + enx);
}

float actf(float x, Act act)
{
    switch (act) {
    case ActSig:  return sigmoidf(x);
    case ActRelu: return reluf(x);
    case ActThan: return tanhf(x);
    case ActSin:  return sinf(x);
    }
    NNAssert(0 && "Unreachable");
    return 0.0f;
}

float dactf(float y, Act act)
{
    switch (act) {
    case ActSig:  return y*(1 - y);
    case ActRelu: return y >= 0 ? 1 : NNReluParem;
    case ActThan: return 1 - y*y;
    case ActSin:  return cosf(asinf(y));
    }
    NNAssert(0 && "Unreachable");
    return 0.0f;
}

float randFloat(void)
{
    return (float) rand() / (float) RAND_MAX;
}

Mat matAlloc(Region *r, size_t rows, size_t cols)
{
    Mat m;
    m.rows = rows;
    m.cols = cols;

    m.elements = regionAlloc(r, sizeof(*m.elements)*rows*cols);
    NNAssert(m.elements != NULL);
    return m;
}

void matDot(Mat dst, Mat a, Mat b)
{
    NNAssert(a.cols == b.rows);
    size_t n = a.cols;
    NNAssert(dst.rows == a.rows);
    NNAssert(dst.cols == b.cols);

    for (size_t i = 0; i < dst.rows; ++i) {
        for (size_t j = 0; j < dst.cols; ++j) {
            MatAt(dst, i, j) = 0;
            for (size_t k = 0; k < n; ++k) {
                MatAt(dst, i, j) += MatAt(a, i, k) * MatAt(b, k, j);
            }
        }
    }
}

Row matRow(Mat m, size_t row)
{
    return (Row) {
        .cols = m.cols,
        .elements = &MatAt(m, row, 0),
    };
}

void matCopy(Mat dst, Mat src)
{
    NNAssert(dst.rows == src.rows);
    NNAssert(dst.cols == src.cols);
    for (size_t i = 0; i < dst.rows; ++i) {
        for (size_t j = 0; j < dst.cols; ++j) {
            MatAt(dst, i, j) = MatAt(src, i, j);
        }
    }
}

void matSum(Mat dst, Mat a)
{
    NNAssert(dst.rows == a.rows);
    NNAssert(dst.cols == a.cols);
    for (size_t i = 0; i < dst.rows; ++i) {
        for (size_t j = 0; j < dst.cols; ++j) {
            MatAt(dst, i, j) += MatAt(a, i, j);
        }
    }
}

void matAct(Mat m)
{
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            MatAt(m, i, j) = actf(MatAt(m, i, j), NNAct);
        }
    }
}

void matPrintf(Mat m, const char *name, size_t padding)
{
    printf("%*s%s = [\n", (int) padding, "", name);
    for (size_t i = 0; i < m.rows; ++i) {
        printf("%*s    ", (int) padding, "");
        for (size_t j = 0; j < m.cols; ++j) {
            printf("%f ", MatAt(m, i, j));
        }
        printf("\n");
    }
    printf("%*s]\n", (int) padding, "");
}

void matFill(Mat m, float x)
{
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            MatAt(m, i, j) = x;
        }
    }
}

void matRand(Mat m, float low, float high)
{
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            MatAt(m, i, j) = randFloat()*(high - low) + low;
        }
    }
}

NN nnAlloc(Region *r, size_t *arch, size_t arch_count)
{
    NNAssert(arch_count > 0);

    NN nn;
    nn.arch = arch;
    nn.arch_count = arch_count;

    nn.ws = regionAlloc(r, sizeof(*nn.ws)*(nn.arch_count - 1));
    NNAssert(nn.ws != NULL);
    nn.bs = regionAlloc(r, sizeof(*nn.bs)*(nn.arch_count - 1));
    NNAssert(nn.bs != NULL);
    nn.as = regionAlloc(r, sizeof(*nn.as)*nn.arch_count);
    NNAssert(nn.as != NULL);

    nn.as[0] = RowAlloc(r, arch[0]);
    for (size_t i = 1; i < arch_count; ++i) {
        nn.ws[i-1] = matAlloc(r, nn.as[i-1].cols, arch[i]);
        nn.bs[i-1] = RowAlloc(r, arch[i]);
        nn.as[i]   = RowAlloc(r, arch[i]);
    }

    return nn;
}

void nnZero(NN nn)
{
    for (size_t i = 0; i < nn.arch_count - 1; ++i) {
        matFill(nn.ws[i], 0);
        RowFill(nn.bs[i], 0);
        RowFill(nn.as[i], 0);
    }
    RowFill(nn.as[nn.arch_count - 1], 0);
}

void nnPrint(NN nn, const char *name)
{
    char buf[256];
    printf("%s = [\n", name);
    for (size_t i = 0; i < nn.arch_count-1; ++i) {
        snprintf(buf, sizeof(buf), "ws%zu", i);
        matPrintf(nn.ws[i], buf, 4);
        snprintf(buf, sizeof(buf), "bs%zu", i);
        RowPrint(nn.bs[i], buf, 4);
    }
    printf("]\n");
}

void nnRand(NN nn, float low, float high)
{
    for (size_t i = 0; i < nn.arch_count-1; ++i) {
        matRand(nn.ws[i], low, high);
        RowRand(nn.bs[i], low, high);
    }
}

void nnForward(NN nn)
{
    for (size_t i = 0; i < nn.arch_count-1; ++i) {
        matDot(rowAsMat(nn.as[i+1]), rowAsMat(nn.as[i]), nn.ws[i]);
        matSum(rowAsMat(nn.as[i+1]), rowAsMat(nn.bs[i]));
        matAct(rowAsMat(nn.as[i+1]));
    }
}

float nnCost(NN nn, Mat t)
{
    NNAssert(NNInput(nn).cols + NNOutout(nn).cols == t.cols);
    size_t n = t.rows;

    float c = 0;
    for (size_t i = 0; i < n; ++i) {
        Row row = matRow(t, i);
        Row x = rowSlice(row, 0, NNInput(nn).cols);
        Row y = rowSlice(row, NNInput(nn).cols, NNOutout(nn).cols);

        RowCopy(NNInput(nn), x);
        nnForward(nn);
        size_t q = y.cols;
        for (size_t j = 0; j < q; ++j) {
            float d = RowAt(NNOutout(nn), j) - RowAt(y, j);
            c += d*d;
        }
    }

    return c/n;
}

NN nnBackprop(Region *r, NN nn, Mat t)
{
    size_t n = t.rows;
    NNAssert(NNInput(nn).cols + NNOutout(nn).cols == t.cols);

    NN g = nnAlloc(r, nn.arch, nn.arch_count);
    nnZero(g);

    // i - current sample
    // l - current layer
    // j - current activation
    // k - previous activation

    for (size_t i = 0; i < n; ++i) {
        Row row = matRow(t, i);
        Row in = rowSlice(row, 0, NNInput(nn).cols);
        Row out = rowSlice(row, NNInput(nn).cols, NNOutout(nn).cols);

        RowCopy(NNInput(nn), in);
        nnForward(nn);

        for (size_t j = 0; j < nn.arch_count; ++j) {
            RowFill(g.as[j], 0);
        }

        for (size_t j = 0; j < out.cols; ++j) {
#ifdef NNBackpropTraditional
            RowAt(NNOutout(g), j) = 2*(RowAt(NNOutout(nn), j) - RowAt(out, j));
#else
            RowAt(NNOutout(g), j) = RowAt(NNOutout(nn), j) - RowAt(out, j);
#endif // NNBackpropTraditional
        }

#ifdef NNBackpropTraditional
        float s = 1;
#else
        float s = 2;
#endif // NNBackpropTraditional

        for (size_t l = nn.arch_count-1; l > 0; --l) {
            for (size_t j = 0; j < nn.as[l].cols; ++j) {
                float a = RowAt(nn.as[l], j);
                float da = RowAt(g.as[l], j);
                float qa = dactf(a, NNAct);
                RowAt(g.bs[l-1], j) += s*da*qa;
                for (size_t k = 0; k < nn.as[l-1].cols; ++k) {
                    // j - weight matrix col
                    // k - weight matrix row
                    float pa = RowAt(nn.as[l-1], k);
                    float w = MatAt(nn.ws[l-1], k, j);
                    MatAt(g.ws[l-1], k, j) += s*da*qa*pa;
                    RowAt(g.as[l-1], k) += s*da*qa*w;
                }
            }
        }
    }

    for (size_t i = 0; i < g.arch_count-1; ++i) {
        for (size_t j = 0; j < g.ws[i].rows; ++j) {
            for (size_t k = 0; k < g.ws[i].cols; ++k) {
                MatAt(g.ws[i], j, k) /= n;
            }
        }
        for (size_t k = 0; k < g.bs[i].cols; ++k) {
            RowAt(g.bs[i], k) /= n;
        }
    }

    return g;
}

NN nnFiniteDiff(Region *r, NN nn, Mat t, float eps)
{
    float saved;
    float c = nnCost(nn, t);

    NN g = nnAlloc(r, nn.arch, nn.arch_count);

    for (size_t i = 0; i < nn.arch_count-1; ++i) {
        for (size_t j = 0; j < nn.ws[i].rows; ++j) {
            for (size_t k = 0; k < nn.ws[i].cols; ++k) {
                saved = MatAt(nn.ws[i], j, k);
                MatAt(nn.ws[i], j, k) += eps;
                MatAt(g.ws[i], j, k) = (nnCost(nn, t) - c)/eps;
                MatAt(nn.ws[i], j, k) = saved;
            }
        }

        for (size_t k = 0; k < nn.bs[i].cols; ++k) {
            saved = RowAt(nn.bs[i], k);
            RowAt(nn.bs[i], k) += eps;
            RowAt(g.bs[i], k) = (nnCost(nn, t) - c)/eps;
            RowAt(nn.bs[i], k) = saved;
        }
    }

    return g;
}

void nnLearn(NN nn, NN g, float rate)
{
    for (size_t i = 0; i < nn.arch_count-1; ++i) {
        for (size_t j = 0; j < nn.ws[i].rows; ++j) {
            for (size_t k = 0; k < nn.ws[i].cols; ++k) {
                MatAt(nn.ws[i], j, k) -= rate*MatAt(g.ws[i], j, k);
            }
        }

        for (size_t k = 0; k < nn.bs[i].cols; ++k) {
            RowAt(nn.bs[i], k) -= rate*RowAt(g.bs[i], k);
        }
    }
}

void matShuffleRows(Mat m)
{
    for (size_t i = 0; i < m.rows; ++i) {
         size_t j = i + rand()%(m.rows - i);
         if (i != j) {
             for (size_t k = 0; k < m.cols; ++k) {
                 float t = MatAt(m, i, k);
                 MatAt(m, i, k) = MatAt(m, j, k);
                 MatAt(m, j, k) = t;
             }
         }
    }
}

// TODO: find is it need to keep batchProcess for future ues
//static void batchProcess(Region *r, Batch *b, size_t batch_size, NN nn, Mat t, float rate)
//{
//    if (b->finished) {
//        b->finished = false;
//        b->begin = 0;
//        b->cost = 0;
//    }
//
//    size_t size = batch_size;
//    if (b->begin + batch_size >= t.rows)  {
//        size = t.rows - b->begin;
//    }
//
//    // TODO: introduce similar to rowSlice operation but for Mat that will give you subsequence of rows
//    Mat batch_t = {
//        .rows = size,
//        .cols = t.cols,
//        .elements = &MatAt(t, b->begin, 0),
//    };
//
//    NN g = nnBackprop(r, nn, batch_t);
//    nnLearn(nn, g, rate);
//    b->cost += nnCost(nn, batch_t);
//    b->begin += batch_size;
//
//    if (b->begin >= t.rows) {
//        size_t batch_count = (t.rows + batch_size - 1)/batch_size;
//        b->cost /= batch_count;
//        b->finished = true;
//    }
//}

Region regionAllocAlloc(size_t capacity_bytes)
{
    Region r = {0};

    size_t word_size = sizeof(*r.words);
    size_t capacity_words = (capacity_bytes + word_size - 1)/word_size;

    void *words = NNMalloc(capacity_words*word_size);
    NNAssert(words != NULL);
    r.capacity = capacity_words;
    r.words = words;
    return r;
}

void *regionAlloc(Region *r, size_t size_bytes)
{
    if (r == NULL) return NNMalloc(size_bytes);
    size_t word_size = sizeof(*r->words);
    size_t size_words = (size_bytes + word_size - 1)/word_size;

    NNAssert(r->size + size_words <= r->capacity);
    if (r->size + size_words > r->capacity) return NULL;
    void *result = &r->words[r->size];
    r->size += size_words;
    return result;
}

Mat rowAsMat(Row row)
{
    return (Mat) {
        .rows = 1,
        .cols = row.cols,
        .elements = row.elements,
    };
}

Row rowSlice(Row row, size_t i, size_t cols)
{
    NNAssert(i < row.cols);
    NNAssert(i + cols <= row.cols);
    return (Row) {
        .cols = cols,
        .elements = &RowAt(row, i),
    };
}

