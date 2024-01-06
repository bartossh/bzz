// Library from rexim.
//
// Extended by bartossh

#ifndef GYM_H_
#define GYM_H_

#include <float.h>
#include <raylib.h>
#include <raymath.h>
#include "nn.h"

#ifndef GYMAssert
#define GYMAssert NNAssert
#endif // GYMAssert


typedef struct {
    float x;
    float y;
    float w;
    float h;
} GymRect;

GymRect gymRect(float x, float y, float w, float h);

typedef enum {
    GloHorz,
    GloVert,
} GymLayoutOrient;

typedef struct {
    float *items;
    size_t count;
    size_t capacity;
} GymPlot;

typedef struct {
    GymLayoutOrient orient;
    GymRect rect;
    size_t count;
    size_t i;
    float gap;
} GymLayout;

GymRect gymLayoutSlotLoc(GymLayout *l, const char *file_path, int line);

typedef struct {
    GymLayout *items;
    size_t count;
    size_t capacity;
} GymLayoutStack;

void gymLayoutStackPush(GymLayoutStack *ls, GymLayoutOrient orient, GymRect rect, size_t count, float gap);
#define GymLayoutStackSlot(ls) (GYMAssert((ls)->count > 0), gymLayoutSlotLoc(&(ls)->items[(ls)->count - 1], __FILE__, __LINE__))
#define GymLayoutStackPop(ls) do { GYMAssert((ls)->count > 0); (ls)->count -= 1; } while (0)

static GymLayoutStack defaultGymLayoutStack = {0};

GymRect gymRoot(void);
GymRect gymFitSquare(GymRect r);
#define GymLayoutBegin(orient, rect, count, gap) gymLayoutStackPush(&defaultGymLayoutStack, orient, rect, count, gap)
#define GymLayoutEnd() GymLayoutStackPop(&defaultGymLayoutStack)
#define GymLayoutSlot() GymLayoutStackSlot(&defaultGymLayoutStack)

#define DaInitCap 256
#define DaAppend(da, item)                                                          \
    do {                                                                             \
        if ((da)->count >= (da)->capacity) {                                         \
            (da)->capacity = (da)->capacity == 0 ? DaInitCap : (da)->capacity*2;   \
            (da)->items = realloc((da)->items, (da)->capacity*sizeof(*(da)->items)); \
            GYMAssert((da)->items != NULL && "Buy more RAM lol");                   \
        }                                                                            \
                                                                                     \
        (da)->items[(da)->count++] = (item);                                         \
    } while (0)

void gymRenderNN(NN nn, GymRect r);
void gymRenderMatAsHeatmap(Mat m, GymRect r, size_t max_width);
void gymRenderNNWeightsHeatmap(NN nn, GymRect r);
void gymRenderNNActivationsHeatmap(NN nn, GymRect r);
void gymPlot(GymPlot plot, GymRect r, Color c);
void gymSlider(float *value, bool *dragging, float rx, float ry, float rw, float rh);
void gymNNImageGrayscale(NN nn, void *pixels, size_t width, size_t height, size_t stride, float low, float high);

#endif
