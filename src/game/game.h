/// Copyright (c) 2024 Bartosz Lenart

#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include "raylib.h"
#include "../nn/nn.h"
#include "../levels/levels.h"

#ifndef OCEAN
#define OCEAN CLITERAL(Color){ 0, 102, 102, 255 } // OCEAN
#endif // OCEAN

#ifndef DEEPOCEAN
#define DEEPOCEAN CLITERAL(Color){ 0, 51, 102, 255 } // DEEPOCEAN
#endif // DEEPOCEAN

#ifndef GRASS
#define GRASS CLITERAL(Color){ 14, 129, 3, 250 } // GRASS
#endif // GRASS

#ifndef bzzAssert
#define bzzAssert NNAssert
#endif // bzzAssert


#define MAX_INNER_LAYERS 4
#define MIN_INNER_LAYERS 1
#define MAX_PERCEPTRONS 20
#define MIN_PERCEPTRONS 2

#define KILO 1024

#define MAX_SWARN_SIZE 128
#define MAX_STATIONARIES_SIZE 512

float randInRange(float a, float b);

typedef struct {
    float x;
    float y;
    float w;
    float h;
} BzzRect;

BzzRect bzzRect(float x, float y, float w, float h);

typedef enum {
    GloHorz,
    GloVert,
} BzzLayoutOrient;

typedef struct {
    float *items;
    size_t count;
    size_t capacity;
} BzzPlot;

void bzzPlotFree(BzzPlot *gp);

typedef struct {
    BzzLayoutOrient orient;
    BzzRect         rect;
    size_t          count;
    size_t          i;
    float           gap;
} BzzLayout;

BzzRect bzzLayoutSlotLoc(BzzLayout *l, const char *file_path, int line);

typedef struct {
    BzzLayout *items;
    size_t    count;
    size_t    capacity;
} BzzLayoutStack;

void bzzLayoutStackPush(BzzLayoutStack *ls, BzzLayoutOrient orient, BzzRect rect, size_t count, float gap);
#define bzzLayoutStackSlot(ls) (bzzAssert((ls)->count > 0), bzzLayoutSlotLoc(&(ls)->items[(ls)->count - 1], __FILE__, __LINE__))
#define bzzLayoutStackPop(ls) do { bzzAssert((ls)->count > 0); (ls)->count -= 1; } while (0)

static BzzLayoutStack defaultbzzLayoutStack = {0};

BzzRect bzzRoot(void);
BzzRect bzzFitSquare(BzzRect r);
#define bzzLayoutBegin(orient, rect, count, gap) bzzLayoutStackPush(&defaultbzzLayoutStack, orient, rect, count, gap)
#define bzzLayoutEnd() bzzLayoutStackPop(&defaultbzzLayoutStack)
#define bzzLayoutSlot() bzzLayoutStackSlot(&defaultbzzLayoutStack)

#define DaInitCap 256
#define DaAppend(da, item)                                                          \
    do {                                                                             \
        if ((da)->count >= (da)->capacity) {                                         \
            (da)->capacity = (da)->capacity == 0 ? DaInitCap : (da)->capacity*2;   \
            (da)->items = realloc((da)->items, (da)->capacity*sizeof(*(da)->items)); \
            bzzAssert((da)->items != NULL && "MEMORY EXHAUSTED....");                   \
        }                                                                            \
                                                                                     \
        (da)->items[(da)->count++] = (item);                                         \
    } while (0)

void bzzRenderMatAsHeatmap(Mat m, BzzRect r, size_t max_width);
void bzzRenderNNWeightsHeatmap(NN nn, BzzRect r);
void bzzRenderNNActivationsHeatmap(NN nn, BzzRect r);
void bzzPlot(BzzPlot plot, BzzRect r, Color c);
void bzzSliderHorizontal(float *value, bool *dragging, float rx, float ry, float rw, float rh, Color c_slide, Color c_dot);
void bzzSliderVertical(float *value, bool *dragging, float rx, float ry, float rw, float rh, Color c_slide, Color c_dot);
void bzzNNImageGrayscale(NN nn, void *pixels, size_t width, size_t height, size_t stride, float low, float high);

// renderTextBoxed renders boxed text allowing for box resizing and text wrapping.
void renderTextBoxed(Font font, const char *text, Rectangle rec, float fontSize, float spacing, bool wordWrap, Color tint);

/// ScreenView represents the screen view to be rendered.
typedef enum {
    BeeTrainScreen,
    BeeMapScreen,
    MainMenuScreen
} ScreenView;

/// bzzButton holds functionality to render buttons.
typedef struct {
    Texture2D tx;
    Color     color;
    float     scale;
} BzzButton;

BzzButton bzzButtonNewMinus(float scale, Color color);
BzzButton bzzButtonNewPlus(float scale, Color color);
BzzButton bzzButtonNewBee(float scale, Color color);
BzzButton bzzButtonNewMap(float scale, Color color);
BzzButton bzzButtonNewLearn(float scale, Color color);
BzzButton bzzButtonNewUpdate(float scale, Color color);
BzzButton bzzButtonNewLogo(float scale, Color color);
int bzzRenderButton(BzzButton btn, Vector2 pos);
void bzzUnloadButton(BzzButton btn);

typedef enum {
    LeftRight,
    TopDown,
} AnimationLayout ;


/// BzzObject represents a object that is not scalable. 
typedef struct {
    Texture2D tx;
    Color     color;
} BzzObject;

/// bzzObjectNewBee creates a new BzzObject of type bee.
///
/// color - blend color. 
BzzObject bzzObjectNewBee(Color color);

/// bzzGetTotlaNumberOAvaliablefFlowersTextures returns total number of flower textures avaliable.
///
int bzzGetTotlaNumberOAvaliablefFlowersTextures(void);

/// bzzObjectNewFlower creates a new BzzObject of type flower.
///
/// color - blend color.
/// int - next type of a flower.
BzzObject bzzObjectNewFlower(Color color, int next);

/// bzzUnloadObject unloads texture from the object.
///
/// obj - movable object to unload texture from.
void bzzUnloadObject(BzzObject obj);

/// BzzStationary is a stationary object.
typedef struct {
    BzzObject obj;
    Vector2   pos;
    float     scale;
} BzzStationary;

BzzStationary bzzStationaryNewFlower(BzzObject obj, Vector2 pos, float scale);

/// bzzRenderStationary renders stationary object.
/// Returns 1 if rected on mouse click or 0 otherwise.
///
/// b - BzzStationary animated object.
/// min - minimum rectangle boundary.
/// max - maximum rectangle boundary.
int bzzRenderStationary(BzzStationary *b);

/// BzzAnimated is an animated and movable object.
typedef struct {
    BzzObject       obj;
    AnimationLayout layout;
    int             frame;
    int             total_frames;
    int             pause_time;
    float           speed;
    float           dir;
    Vector2         pos;
    Vector2         target;
} BzzAnimated;


/// bzzAnimatedNewBee creates new movable bee object.
/// 
/// start - starting position at which bee will be drawn.
/// color - blend color. 
BzzAnimated bzzAnimatedNewBee(BzzObject obj, Vector2 start, Vector2 next, AnimationLayout l);

/// bzzRenderAnimated renders moveable object.
///
/// b - BzzAnimated animated object.
/// next - next possition.
void bzzRenderAnimated(BzzAnimated *b, Vector2 next);

/// bzzCheckCollision - checks collision between BzzAnimated object and rectangle.
/// Returns 1 if collision or 0 otherwise.
///
/// b - BzzAnimated object to check collision for.
/// r - Rectangle object to check collision against.
int bzzCheckCollision(BzzAnimated *b, Rectangle r);

// ViewMenu structure representing menu view.
typedef struct {
    BzzButton logo_button;
    Font      font;
} ViewMenu;

/// bzzBzzBeeGameNew return new ViewMenu.
///
/// font - font used for text rendering.
/// bee_button - bee bzzButton object. 
ViewMenu viewMenuNew(Font font, BzzButton logo_button);

// renderMenuView renders main page in to the screen.
//
// m - ViewMenu object holding functionality to render main menu. 
// screen - screen value representing screen to render.
void renderMenuView(ViewMenu m, ScreenView *screen);

/// BzzSwarn represents swarm buffer. 
typedef struct {
    BzzAnimated buffer[MAX_SWARN_SIZE];
    int         swarm_size;
} BzzSwarm;

/// bzzSwarmNew creats a new BzzSwarm buffer.
///
BzzSwarm bzzSwarmNew(void);

/// bzzSwarmAppend appends BzzAnimated object to buffer.
/// 
/// s - BzzSwarm buffer.
/// b - BzzAnimated object to append to buffer.
bool bzzSwarmAppend(BzzSwarm *s, BzzAnimated b); 

/// bzzSwarmGetSize returns size of the buffer;
///
/// s - BzzSwarm buffer.
int bzzSwarmGetSize(BzzSwarm *s);

/// bzzSwarmAt returns pointer to BzzAnimated object at given index or NULL otherwise.
///
/// s - BzzSwarm buffer.
/// idx - index of the BzzAnimated object in BzzSwarm.
BzzAnimated *bzzSwarmAt(BzzSwarm *s, int idx);

/// bzzSwarmRemoveAt - removes object at index.
///
/// s - BzzSwarm buffer.
/// idx - index of the BzzAnimated object in BzzSwarm.
bool bzzSwarmRemoveAt(BzzSwarm *s, int idx);


/// BzzSwarn represents swarm buffer. 
typedef struct {
    BzzStationary buffer[MAX_SWARN_SIZE];
    int         st_size;
} BzzStationaries;

/// bzzStationariesNew creats a new BzzStationaries buffer.
///
BzzStationaries bzzStationariesNew(void);

/// bzzStationariesAppend appends BzzStationary object to buffer.
/// 
/// s - BzzStationaries buffer.
/// b - BzzStationary object to append to buffer.
bool bzzStationariesAppend(BzzStationaries *s, BzzStationary b); 

/// bzzStationariesGetSize returns size of the buffer;
///
/// s - BzzStationaries buffer.
int bzzStationariesGetSize(BzzStationaries *s);

/// bzzStationaryAt returns pointer to BzzStationaries object at given index or NULL otherwise.
///
/// s - BzzStationaries buffer.
/// idx - index of the AssetTexture2D object in BzzStationaries.
BzzStationary *bzzStationariesAt(BzzStationaries *s, int idx);

/// bzzRemoveAt - removes object at index.
///
/// s - BzzStationaries buffer.
/// idx - index of the BzzStationaries object in BzzStationaries.
bool bzzStationatiesRemoveAt(BzzStationaries *s, int idx);

/// PageNN holds all the data required to properly update the nn page view.
typedef struct {
    size_t          max_epoch;
    size_t          epochs_per_frame;
    size_t          epoch;
    float           rate;
    int             inner_layers_count;
    int             inner_layers[MAX_INNER_LAYERS];
    bool            paused;
    bool            reset;
    bool            modified;
    Region          temp;
    FlowersDataset  fl;
    Mat             t;
    NN              nn;
    BzzPlot         plot;
    BzzButton       minus_button;
    BzzButton       plus_button;
    BzzButton       learn_button;
    BzzButton       update_button;
    BzzButton       map_button;
    BzzButton       bee_button;
    BzzSwarm        *swarm;
    BzzStationaries *stationaries;
    Font            font;
} BzzBeeGame;

/// bzzBzzBeeGameNew return new BzzBeeGame.
///
/// font - Font that will be used for text rendering.
/// minus_button - minus BzzButton object.
/// plus_button - plus BzzButton object.
/// learn_button - learn BzzButton object.
/// update_button - update BzzButton object.
/// map_button - map BzzButton object.
/// bee_button - bee BzzButton object.
/// swarm - swarm buffer.
/// inner_layers_count - number of inner NN layers.
/// inner_layers - architecture of inner layers.
BzzBeeGame bzzBzzBeeGameNew(
    Font font, BzzButton minus_button, BzzButton plus_button, BzzButton learn_button, BzzButton update_button,
    BzzButton map_button, BzzButton bee_button, BzzSwarm *swarm, BzzStationaries *stationaries, FlowersDataset fl,
    int inner_layers_count, int inner_layers[MAX_INNER_LAYERS]
);

/// viewBeeRandomize - randomizes BzzBeeGame.
///
/// bee -BzzBeeGame to be randomized. 
void viewBeeRandomize(BzzBeeGame *bee);

/// renderBeeView renders PageNN in to the screen.
///
/// bee - pointer to BzzBeeGame structure.
// screen - screen value representing screen to render.
void renderBeeView(BzzBeeGame *bee, ScreenView *screen);

/// void bzzBzzBeeGameFree frees memory allocated for BzzBeeGame.
///
/// bee -BzzBeeGame to be freed from memory.
void bzzBzzBeeGameFree(BzzBeeGame *bee);

/// bzzRenderNN renders NN from BzzBeeGame with controls.
///
/// bee - BzzBeeGame containing NN.
/// r - bzzRect representing render rectangle.
void bzzRenderNN(BzzBeeGame *bee, BzzRect r);

/// isModified returns true if bee is modified or false otherwise.
bool isModified(BzzBeeGame *bee);

/// renderMapView renders a map view.
///
/// bee - BzzBeeGame holding bee functionality parameters.
// screen - screen value representing screen to render.
void renderMapView(BzzBeeGame *bee, ScreenView *screen);

#endif
