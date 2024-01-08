#ifndef VIEW_H
#define VIEW_H

#include <stdbool.h>
#include "raylib.h"
#include "../nn/nn.h"
#include "../flowers/flowers.h"

#ifndef OCEAN
#define OCEAN  CLITERAL(Color){ 0, 102, 102, 255 } // OCEAN
#endif // OCEAN

#ifndef DEEPOCEAN
#define DEEPOCEAN  CLITERAL(Color){ 0, 51, 102, 255 } // OCEAN
#endif // DEEPOCEAN

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
            GYMAssert((da)->items != NULL && "MEMORY EXHAUSTED....");                   \
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

typedef struct {
    Texture2D logo;
    Font font;
} ViewManu;

/// viewBeeNew return new ViewManu.
///
ViewManu viewManuNew(const char *path, Font font);

// drawManuView draws main page in to the screen.
void drawManuView(ViewManu m);

void cleanupManuView(ViewManu m);

/// PageNN holds all the data required to properly update the nn page view.
typedef struct {
    size_t max_epoch;
    size_t epochs_per_frame;
    size_t epoch; 
    float rate;
    bool paused;
    bool reset;
    Region temp;
    FlowersDataset fl;
    Mat t;
    NN nn;
    GymPlot plot;
    Font font;
} ViewBee;

/// viewBeeNew return new ViewBee.
///
/// font - Font that will be used for text drawing.
ViewBee viewBeeNew(Font font);

/// drawBeeView draws PageNN in to the screen.
///
/// bee - pointer to ViewBee structure.
void drawBeeView(ViewBee *bee);

#endif
