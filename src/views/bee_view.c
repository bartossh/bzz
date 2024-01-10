/// Copyright (c) 2024 Bartosz Lenart

#include "../flowers/flowers.h"
#include "../nn/nn.h"
#include "raylib.h"
#include "view.h"
#include <float.h>
#include <raylib.h>
#include <raymath.h>
#include <string.h>
#include <time.h>

const float widget_height_multip = 0.8;
const float widget_width_multip = 0.8;
const float widget_padding_multip = 0.1;

static float absf(float a) 
{
    if (a > 0.0f) {
      return a;
    }
    return -a;
}

void gymRenderNN(NN nn, GymRect r)
{
    Color low_color = ORANGE;
    Color high_color = BLACK;
    Color alpha_color = WHITE;

    float thick = r.h * 0.002f;

    float neuron_radius = r.h * 0.03;
    float layer_border_vpad = r.h * 0.08;
    float layer_border_hpad = r.w * 0.06;
    float nn_width = r.w - 2 * layer_border_hpad;
    float nn_height = r.h - 2 * layer_border_vpad;
    float nn_x = r.x + r.w / 2 - nn_width / 2;
    float nn_y = r.y + r.h / 2 - nn_height / 2;
    float layer_hpad = nn_width / nn.arch_count;
    for (size_t l = 0; l < nn.arch_count; ++l) {
      float layer_vpad1 = nn_height / nn.as[l].cols;
      for (size_t i = 0; i < nn.as[l].cols; ++i) {
        float cx1 = nn_x + l * layer_hpad + layer_hpad / 2;
        float cy1 = nn_y + i * layer_vpad1 + layer_vpad1 / 2;
        if (l + 1 < nn.arch_count) {
          float layer_vpad2 = nn_height / nn.as[l + 1].cols;
          for (size_t j = 0; j < nn.as[l + 1].cols; ++j) {
            // i - rows of ws
            // j - cols of ws
            float cx2 = nn_x + (l + 1) * layer_hpad + layer_hpad / 2;
            float cy2 = nn_y + j * layer_vpad2 + layer_vpad2 / 2;
            float value = sigmoidf(MatAt(nn.ws[l], i, j));
            high_color.a = floorf(255.f * value);
            Vector2 start = {cx1, cy1};
            Vector2 end = {cx2, cy2};
            DrawLineEx(start, end, thick,
                       ColorAlphaBlend(low_color, high_color, alpha_color));
          }
        }
        if (l > 0) {
          high_color.a = floorf(255.f * sigmoidf(RowAt(nn.bs[l - 1], i)));
          DrawPoly((Vector2){.x = cx1, .y = cy1}, 6, neuron_radius, 0.0f,
                   ColorAlphaBlend(low_color, high_color, alpha_color));
        } else {
          DrawPoly((Vector2){.x = cx1, .y = cy1}, 6, neuron_radius, 0.0f,
                   ColorAlphaBlend(low_color, high_color, alpha_color));
        }
      }
    }
}

void gymRenderMatAsHeatmap(Mat m, GymRect r, size_t max_width) 
{
    Color low_color = ORANGE;
    Color high_color = BLACK;
    Color alpha_color = WHITE;

    float cell_width = r.w * m.cols / max_width / m.cols;
    float cell_height = r.h / m.rows;

    float full_width = r.w * m.cols / max_width;

    for (size_t y = 0; y < m.rows; ++y) {
      for (size_t x = 0; x < m.cols; ++x) {
        high_color.a = floorf(255.f * sigmoidf(MatAt(m, y, x)));
        Color color = ColorAlphaBlend(low_color, high_color, alpha_color);
        GymRect slot = {
            r.x + r.w / 2 - full_width / 2 + x * cell_width,
            r.y + y * cell_height,
            cell_width,
            cell_height,
        };
        DrawRectangle(ceilf(slot.x), ceilf(slot.y), ceilf(slot.w), ceilf(slot.h),
                      color);
      }
    }
}

void gymRenderNNWeightsHeatmap(NN nn, GymRect r) {
    size_t max_width = 0;
    for (size_t i = 0; i < nn.arch_count - 1; ++i) {
      if (max_width < nn.ws[i].cols) {
        max_width = nn.ws[i].cols;
      }
    }

    GymLayoutBegin(GloVert, r, nn.arch_count - 1, 20);
    for (size_t i = 0; i < nn.arch_count - 1; ++i) {
      gymRenderMatAsHeatmap(nn.ws[i], GymLayoutSlot(), max_width);
    }
    GymLayoutEnd();
}

void gymRenderNNActivationsHeatmap(NN nn, GymRect r) 
{
    size_t max_width = 0;
    for (size_t i = 0; i < nn.arch_count; ++i) {
      if (max_width < nn.as[i].cols) {
        max_width = nn.as[i].cols;
      }
    }

    GymLayoutBegin(GloVert, r, nn.arch_count, 20);
    for (size_t i = 0; i < nn.arch_count; ++i) {
      gymRenderMatAsHeatmap(rowAsMat(nn.as[i]), GymLayoutSlot(), max_width);
    }
    GymLayoutEnd();
}

void gymPlot(GymPlot plot, GymRect r, Color c) 
{
    float min = FLT_MAX, max = FLT_MIN;
    for (size_t i = 0; i < plot.count; ++i) {
      if (max < plot.items[i])
        max = plot.items[i];
      if (min > plot.items[i])
        min = plot.items[i];
    }

    float cost = r.h - r.h * 0.08;
    char buffer[64];
    snprintf(buffer, sizeof(buffer), "%s", "Cost ");
    if (plot.count > 0) {
      snprintf(&buffer[5], sizeof(buffer) - 5, "%f", plot.items[plot.count - 1]);
      cost = r.y * plot.items[plot.count - 1] * 100;
    }
    DrawText(buffer, r.x, r.y, r.h * 0.08, c);
    if (min > 0)
      min = 0;
    float y0 = r.y + (1 - (0 - min) / (max - min)) * r.h;
    if (cost > r.h) {
      cost = r.h - r.h * 0.08;
    }
    float padding_front = 25,
          padding_back = 300; // TODO: Organize plot area better way.
    DrawRectangle(padding_front + r.x, y0 - cost, padding_front + 50, cost, c);
    DrawLineEx((Vector2){r.x, y0}, (Vector2){r.x + r.w - padding_back, y0}, 2, c);
}

void gymSliderHorizontal(float *value, bool *slider_dragging, float rx, float ry, float width, float knob_radius, Color c_slide, Color c_dot) 
{
    Vector2 bar_size = {
        .x = width - 2 * knob_radius,
        .y = knob_radius * 0.25,
    };
    Vector2 bar_position = {.x = rx + knob_radius,
                            .y = ry + knob_radius / 2 - bar_size.y / 2};
    DrawRectangleV(bar_position, bar_size, c_slide);

    Vector2 knob_position = {.x = bar_position.x + bar_size.x * (*value),
                             .y = ry + knob_radius / 2};
    DrawCircleV(knob_position, knob_radius, c_dot);

    if (*slider_dragging) {
      float x = GetMousePosition().x;
      if (x < bar_position.x)
        x = bar_position.x;
      if (x > bar_position.x + bar_size.x)
        x = bar_position.x + bar_size.x;
      *value = (x - bar_position.x) / bar_size.x;
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      Vector2 mouse_position = GetMousePosition();
      if (Vector2Distance(mouse_position, knob_position) <= knob_radius) {
        *slider_dragging = true;
      }
    }

    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
      *slider_dragging = false;
    }
}

void gymSliderVertical(float *value, bool *slider_dragging, float rx, float ry, float width, float knob_radius, Color c_slide, Color c_dot) 
{
    Vector2 bar_size = {
        .x = knob_radius * 0.25,
        .y = width + 2 * knob_radius,
    };
    Vector2 bar_position = {
        .x = rx + knob_radius / 2 - bar_size.x / 2,
        .y = ry + knob_radius,
    };
    DrawRectangleV(bar_position, bar_size, c_slide);

    Vector2 knob_position = {
        .x = rx + knob_radius / 2,
        .y = bar_position.y + bar_size.y * (*value),
    };
    DrawCircleV(knob_position, knob_radius, c_dot);

    if (*slider_dragging) {
      float y = GetMousePosition().y;
      if (y < bar_position.y)
        y = bar_position.y;
      if (y > bar_position.y + bar_size.y)
        y = bar_position.y + bar_size.y;
      *value = (y - bar_position.y) / bar_size.y;
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      Vector2 mouse_position = GetMousePosition();
      if (Vector2Distance(mouse_position, knob_position) <= knob_radius) {
        *slider_dragging = true;
      }
    }

    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
      *slider_dragging = false;
    }
}

void gymNNImageGrayscale(NN nn, void *pixels, size_t width, size_t height, size_t stride, float low, float high) 
{
    GYMAssert(NNInput(nn).cols >= 2);
    GYMAssert(NNOutout(nn).cols >= 1);
    uint32_t *pixels_u32 = pixels;
    for (size_t y = 0; y < height; ++y) {
      for (size_t x = 0; x < width; ++x) {
        RowAt(NNInput(nn), 0) = (float)x / (float)(width - 1);
        RowAt(NNInput(nn), 1) = (float)y / (float)(height - 1);
        nnForward(nn);
        float a = RowAt(NNOutout(nn), 0);
        if (a < low)
          a = low;
        if (a > high)
          a = high;
        uint32_t pixel = (a + low) / (high - low) * 255.f;
        pixels_u32[y * stride + x] = (0xFF << (8 * 3)) | (pixel << (8 * 2)) |
                                     (pixel << (8 * 1)) | (pixel << (8 * 0));
      }
    }
}

GymRect gymRect(float x, float y, float w, float h) 
{
    GymRect r = {0};
    r.x = x;
    r.y = y;
    r.w = w;
    r.h = h;
    return r;
}

GymRect gymLayoutSlotLoc(GymLayout *l, const char *file_path, int line) 
{
    if (l->i >= l->count) {
      fprintf(stderr, "%s:%d: ERROR: Layout overflow\n", file_path, line);
      exit(1);
    }

    GymRect r = {0};

    switch (l->orient) {
    case GloHorz:
      r.w = (l->rect.w - l->gap * (l->count - 1)) / l->count;
      r.h = l->rect.h;
      r.x = l->rect.x + l->i * (r.w + l->gap);
      r.y = l->rect.y;
      break;

    case GloVert:
      r.w = l->rect.w;
      r.h = (l->rect.h - l->gap * (l->count - 1)) / l->count;
      r.x = l->rect.x;
      r.y = l->rect.y + l->i * (r.h + l->gap);
      break;

    default:
      GYMAssert(0 && "Unreachable");
    }

    l->i += 1;

    return r;
}

void gymLayoutStackPush(GymLayoutStack *ls, GymLayoutOrient orient, GymRect rect, size_t count, float gap) 
{
    GymLayout l = {0};
    l.orient = orient;
    l.rect = rect;
    l.count = count;
    l.gap = gap;
    DaAppend(ls, l);
}

GymRect gymRoot(void) 
{
    GymRect root = {0};
    root.w = GetScreenWidth();
    root.h = GetScreenHeight();
    return root;
}

GymRect gymFitSquare(GymRect r) 
{
    if (r.w < r.h) {
        return (GymRect){
            .x = r.x,
            .y = r.y + r.h / 2 - r.w / 2,
            .w = r.w,
            .h = r.w,
        };
    } else {
        return (GymRect){
            .x = r.x + r.w / 2 - r.h / 2,
            .y = r.y,
            .w = r.h,
            .h = r.h,
        };
    }
}

static void viewBeeLearn(ViewBee *bee, GymRect r, float *slider_position, bool *slider_dragging) 
{
    if (!bee) {
        exit(1);
        return;
    }

    const size_t buff_size = 8192, inner_buff_size = 256;

    char buffer[buff_size];
    const float s = r.h * 0.021;
    const float pad = r.w * 0.001;
    const size_t elements_per_page = 32;

    size_t start = 0;
    if (bee->t.rows > elements_per_page && bee->paused) {
        Color low_color = BLUE;
        Color high_color = ORANGE;
        Color alpha_color = WHITE;
        high_color.a = floorf(255.f * (*slider_position));

        gymSliderVertical(slider_position, slider_dragging, r.x + r.w + 200,
                        r.y + r.h * widget_padding_multip,
                        r.h * (widget_height_multip - widget_padding_multip), 10,
                        ColorAlphaBlend(low_color, high_color, alpha_color),
                        ORANGE);

        start = (size_t)(*slider_position * (float)(bee->t.rows - elements_per_page));
    }
    size_t next_pos = 0;
    for (size_t i = start; i < bee->t.rows && i < start + elements_per_page &&
        next_pos < buff_size - inner_buff_size; ++i) {
        
        char inner_buff[inner_buff_size];
        for (size_t j = 0; j < bee->t.cols - 1; ++j) {
          RowAt(NNInput(bee->nn), j) = MatAt(bee->t, i, j);
        }
        nnForward(bee->nn);
        size_t move = printToBuffAtRow(bee->fl, i, inner_buff, 256);
        float expected = getExpectedValueAtRowNorm(bee->fl, i);
        float value = RowAt(NNOutout(bee->nn), 0);
        float diff = absf(expected - value);
        snprintf(inner_buff + move, 28, " = [%.3f] %.3f\n", value, diff);
        size_t inner_buf_len = strlen(inner_buff);
        strncpy(&buffer[next_pos], inner_buff, inner_buf_len);
        next_pos += inner_buf_len;
    }
    char buffer_print[buff_size];
    snprintf(buffer_print, next_pos, "%s", buffer);
    DrawTextBoxed(bee->font, buffer_print, (Rectangle){r.x, r.y, r.w + 200, r.h},
                  s, pad, false, ORANGE);
}

ViewBee viewBeeNew(Font font) 
{
    const size_t max_epoch = 200 * 1000;
    const size_t epochs_per_frame = 300;
    size_t epoch = 0;
    const float rate = 0.7f;

    Region temp = regionAllocAlloc(8 * 1024 * 1024);
    FlowersDataset fl = flowersDatasetNew(Location_6_60);
    Mat t = flowersToMat(fl);

    size_t r0 = rand() % (fl.cols + 20) + fl.cols - 1;
    size_t r1 = rand() % (fl.cols + 5) + 2;

    const size_t arch_template[] = {fl.cols - 1, r0, r1, 1};
    const size_t arch_len = ArrayLen(arch_template);
    size_t *arch = NNMalloc(sizeof(arch_template[0]) * arch_len);
    for (size_t i = 0; i < arch_len; ++i) {
      arch[i] = arch_template[i];
    }

    NN nn = nnAlloc(NULL, arch, arch_len);
    nnRand(nn, -1, 1);
    GymPlot plot = {0};

    ViewBee bee = {
        .max_epoch = max_epoch,
        .epochs_per_frame = epochs_per_frame,
        .epoch = epoch,
        .rate = rate,
        .paused = true,
        .reset = false,
        .temp = temp,
        .fl = fl,
        .t = t,
        .nn = nn,
        .plot = plot,
        .font = font,
    };

    return bee;
}

void viewBeeFree(ViewBee *bee) 
{
    if (!bee) {
        return;
    }

    bee->max_epoch = 0;
    bee->epochs_per_frame = 0;
    bee->epoch = 0;
    bee->rate = 0.0f;
    bee->paused = true;
    bee->reset = false;
    regionFree(&bee->temp);
    flowersDatasetFree(&bee->fl);
    matFree(&bee->t);
    nnFree(&bee->nn);
    bee->plot = (GymPlot){0};
}

float slider_position = 0.0f;
bool slider_dragging = false;

void drawBeeView(ViewBee *bee) 
{
    if (!bee) {
        exit(1);
        return;
    }

    if (bee->reset) {
        bee->epoch = 0;
        nnRand(bee->nn, -1, 1);
        bee->plot.count = 0;
    }

    for (size_t i = 0;i < bee->epochs_per_frame && !bee->paused && bee->epoch < bee->max_epoch;
         ++i) {
        
        NN g = nnBackprop(&bee->temp, bee->nn, bee->t);
        nnLearn(bee->nn, g, bee->rate);
        bee->epoch += 1;
        DaAppend(&(bee->plot), nnCost(bee->nn, bee->t));
    }

    BeginDrawing();
    ClearBackground(OCEAN);
    int w = GetScreenWidth();
    int h = GetScreenHeight();

    DrawText("Train your BEE!", 5, 5, h * 0.025, GREEN);
    char buffer[256];
    snprintf(buffer, sizeof(buffer),
             "<| Epoch: %zu/%zu, Rate: %f, Cost: %f, Temporary Memory: %zu kB |>",
             bee->epoch, bee->max_epoch, bee->rate, nnCost(bee->nn, bee->t),
             RegionOccupiedBytes(&bee->temp) / 1024);
    DrawTextEx(bee->font, buffer, CLITERAL(Vector2){.x = 40, .y = 40}, h * 0.02,
               0, DEEPOCEAN);

    GymRect r;
    r.w = w * widget_height_multip;
    r.h = h * widget_width_multip;
    r.x = 0;
    r.y = h / 2 - r.h / 2;

    GymLayoutBegin(GloHorz, r, 3, 10);
        gymPlot(bee->plot, GymLayoutSlot(), ORANGE);
        gymRenderNN(bee->nn, GymLayoutSlot());
        viewBeeLearn(bee, GymLayoutSlot(), &slider_position, &slider_dragging);
    GymLayoutEnd();

    EndDrawing();

    RegionReset(&bee->temp);
}
