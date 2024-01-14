/// Copyright (c) 2024 Bartosz Lenart

#ifndef VIEW_H
#define VIEW_H

#include <stdbool.h>
#include "raylib.h"
#include "../nn/nn.h"
#include "../flowers/flowers.h"

#ifndef OCEAN
#define OCEAN CLITERAL(Color){ 0, 102, 102, 255 } // OCEAN
#endif // OCEAN

#ifndef DEEPOCEAN
#define DEEPOCEAN CLITERAL(Color){ 0, 51, 102, 255 } // DEEPOCEAN
#endif // DEEPOCEAN

#ifndef GRASS
#define GRASS CLITERAL(Color){ 14, 129, 3, 250 } // GRASS
#endif // GRASS

#ifndef GYMAssert
#define GYMAssert NNAssert
#endif // GYMAssert


#define MAX_INNER_LAYERS 4
#define MIN_INNER_LAYERS 1
#define MAX_PERCEPTRONS 20
#define MIN_PERCEPTRONS 2 

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

void gymPlotFree(GymPlot *gp);

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

void gymRenderMatAsHeatmap(Mat m, GymRect r, size_t max_width);
void gymRenderNNWeightsHeatmap(NN nn, GymRect r);
void gymRenderNNActivationsHeatmap(NN nn, GymRect r);
void gymPlot(GymPlot plot, GymRect r, Color c);
void gymSliderHorizontal(float *value, bool *dragging, float rx, float ry, float rw, float rh, Color c_slide, Color c_dot);
void gymSliderVertical(float *value, bool *dragging, float rx, float ry, float rw, float rh, Color c_slide, Color c_dot);
void gymNNImageGrayscale(NN nn, void *pixels, size_t width, size_t height, size_t stride, float low, float high);

// renderTextBoxed renders boxed text allowing for box resizing and text wrapping.
void renderTextBoxed(Font font, const char *text, Rectangle rec, float fontSize, float spacing, bool wordWrap, Color tint);

/// ScreenView represents the screen view to be rendered.
typedef enum {
    BeeTrainScreen,
    BeeMapScreen,
    MainMenuScreen
} ScreenView;

/// GymButton holds functionality to render buttons.
typedef struct {
    Texture2D tx;
    Color color;
    float scale;
} GymButton;

GymButton gymButtonNewMinus(float scale, Color color);
GymButton gymButtonNewPlus(float scale, Color color);
GymButton gymButtonNewBee(float scale, Color color);
GymButton gymButtonNewMap(float scale, Color color);
GymButton gymButtonNewLearn(float scale, Color color);
GymButton gymButtonNewUpdate(float scale, Color color);
GymButton gymButtonNewLogo(float scale, Color color);
int gymRenderButton(GymButton btn, Vector2 pos);
void gymUnloadButton(GymButton btn);

// ViewMenu structure representing menu view.
typedef struct {
    GymButton logo_button;
    Font font;
} ViewMenu;

/// viewBeeNew return new ViewMenu.
///
/// font - font used for text rendering.
/// bee_button - bee GymButton object. 
ViewMenu viewMenuNew(Font font, GymButton logo_button);

// renderMenuView renders main page in to the screen.
//
// m - ViewMenu object holding functionality to render main menu. 
// screen - screen value representing screen to render.
void renderMenuView(ViewMenu m, ScreenView *screen);

/// PageNN holds all the data required to properly update the nn page view.
typedef struct {
    size_t max_epoch;
    size_t epochs_per_frame;
    size_t epoch;
    float rate;
    int inner_layers_count;
    int inner_layers[MAX_INNER_LAYERS];
    bool paused;
    bool reset;
    bool modified;
    Region temp;
    FlowersDataset fl;
    Mat t;
    NN nn;
    GymPlot plot;
    GymButton minus_button;
    GymButton plus_button;
    GymButton learn_button;
    GymButton update_button;
    GymButton map_button;
    GymButton bee_button;
    Font font;
} BeeParams;

/// viewBeeNew return new BeeParams.
///
/// font - Font that will be used for text rendering.
/// minus_button - minus GymButton object.
/// plus_button - plus GymButton object.
/// learn_button - learn GymButton object.
/// update_button - update GymButton object.
/// map_button - map GymButton object.
/// bee_button - bee GymButton object.
/// inner_layers_count - number of inner NN layers.
/// inner_layers - architecture of inner layers.
BeeParams viewBeeNew(
    Font font, GymButton minus_button, GymButton plus_button, GymButton learn_button, GymButton update_button,
    GymButton map_button, GymButton bee_button, int inner_layers_count, int inner_layers[MAX_INNER_LAYERS]
    );

/// viewBeeRandomize - randomizes BeeParams.
///
/// bee -BeeParams to be randomized. 
void viewBeeRandomize(BeeParams *bee);

/// renderBeeView renders PageNN in to the screen.
///
/// bee - pointer to BeeParams structure.
// screen - screen value representing screen to render.
void renderBeeView(BeeParams *bee, ScreenView *screen);

/// void viewBeeFree frees memory allocated for BeeParams.
///
/// bee -BeeParams to be freed from memory.
void viewBeeFree(BeeParams *bee);

/// gymRenderNN renders NN from BeeParams with controls.
///
/// bee - BeeParams containing NN.
/// r - GymRect representing render rectangle.
void gymRenderNN(BeeParams *bee, GymRect r);

/// isModified returns true if bee is modified or false otherwise.
bool isModified(BeeParams *bee);

/// renderMapView renders a map view.
///
/// bee - BeeParams holding bee functionality parameters.
// screen - screen value representing screen to render.
void renderMapView(BeeParams *bee, ScreenView *screen);

#endif
