// Library from rexim.
//
// Extended by bartossh

#ifndef GYM_H_
#define GYM_H_

#include <float.h>
#include <raylib.h>
#include <raymath.h>
#include "nn.h"

#ifndef GYM_ASSERT
#define GYM_ASSERT NN_ASSERT
#endif // GYM_ASSERT

// The Tsoding Background Color
#define GYM_BACKGROUND CLITERAL(Color) { 0x18, 0x18, 0x18, 0xFF }

typedef struct {
    float x;
    float y;
    float w;
    float h;
} Gym_Rect;

Gym_Rect gym_rect(float x, float y, float w, float h);

typedef enum {
    GLO_HORZ,
    GLO_VERT,
} Gym_Layout_Orient;

typedef struct {
    float *items;
    size_t count;
    size_t capacity;
} Gym_Plot;

typedef struct {
    Gym_Layout_Orient orient;
    Gym_Rect rect;
    size_t count;
    size_t i;
    float gap;
} Gym_Layout;

Gym_Rect gym_layout_slot_loc(Gym_Layout *l, const char *file_path, int line);

typedef struct {
    Gym_Layout *items;
    size_t count;
    size_t capacity;
} Gym_Layout_Stack;

void gym_layout_stack_push(Gym_Layout_Stack *ls, Gym_Layout_Orient orient, Gym_Rect rect, size_t count, float gap);
#define gym_layout_stack_slot(ls) (GYM_ASSERT((ls)->count > 0), gym_layout_slot_loc(&(ls)->items[(ls)->count - 1], __FILE__, __LINE__))
#define gym_layout_stack_pop(ls) do { GYM_ASSERT((ls)->count > 0); (ls)->count -= 1; } while (0)

static Gym_Layout_Stack default_gym_layout_stack = {0};

Gym_Rect gym_root(void);
Gym_Rect gym_fit_square(Gym_Rect r);
#define gym_layout_begin(orient, rect, count, gap) gym_layout_stack_push(&default_gym_layout_stack, orient, rect, count, gap)
#define gym_layout_end() gym_layout_stack_pop(&default_gym_layout_stack)
// TODO: allow a single slot to take up several slots
#define gym_layout_slot() gym_layout_stack_slot(&default_gym_layout_stack)

#define DA_INIT_CAP 256
#define da_append(da, item)                                                          \
    do {                                                                             \
        if ((da)->count >= (da)->capacity) {                                         \
            (da)->capacity = (da)->capacity == 0 ? DA_INIT_CAP : (da)->capacity*2;   \
            (da)->items = realloc((da)->items, (da)->capacity*sizeof(*(da)->items)); \
            GYM_ASSERT((da)->items != NULL && "Buy more RAM lol");                   \
        }                                                                            \
                                                                                     \
        (da)->items[(da)->count++] = (item);                                         \
    } while (0)

void gym_render_nn(NN nn, Gym_Rect r);
void gym_render_mat_as_heatmap(Mat m, Gym_Rect r, size_t max_width);
void gym_render_nn_weights_heatmap(NN nn, Gym_Rect r);
void gym_render_nn_activations_heatmap(NN nn, Gym_Rect r);
void gym_plot(Gym_Plot plot, Gym_Rect r, Color c);
void gym_slider(float *value, bool *dragging, float rx, float ry, float rw, float rh);
void gym_nn_image_grayscale(NN nn, void *pixels, size_t width, size_t height, size_t stride, float low, float high);

#endif
