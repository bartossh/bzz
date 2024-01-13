/// Copyright (c) 2024 Bartosz Lenart

#include <float.h>
#include <raylib.h>
#include <raymath.h>
#include <string.h>
#include <time.h>
#include "raylib.h"
#include "views.h"
#include "../nn/nn.h"
#include "../flowers/flowers.h"
#include "../assets/assets_loader.h"

#include <stdio.h>

#define KILO 1024

const float widget_padding_multip = 0.1;
const float controles_height_multip = 0.95;
const float widget_height_multip = 0.8;
const float widget_width_multip = 0.8;

static float absf(float a) 
{
    if (a > 0.0f) {
        return a;
    }
    return -a;
}

GymButton gymButtonNewMinus(float scale, Color color)
{
    GymButton btn = {
        .tx = assetLoad(MinusButton),
        .color = color,
        .scale = scale,
    };

    return btn;
}

GymButton gymButtonNewPlus(float scale, Color color)
{
    GymButton btn = {
        .tx = assetLoad(PlusButton),
        .color = color,
        .scale = scale,
    };

    return btn;
}

int gymRenderButton(GymButton btn, Vector2 pos)
{ 
    float width = btn.tx.width * btn.scale;
    float height = btn.tx.height *btn.scale;
    
    Rectangle btnBounds = { pos.x, pos.y, width, height };
    Vector2 mousePoint = GetMousePosition();

    float resize = 1.0f;
    int result = 0;

    if (CheckCollisionPointRec(mousePoint, btnBounds)) {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            resize = 0.9;
        }

        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            result++;
        }    
    }

    Vector2 p = { .x = pos.x + (width - width * resize)/2.0f, .y = pos.y + (height - height * resize)/2.0f };
    
    DrawTextureEx(btn.tx, p, 0.0f, btn.scale*resize, btn.color);

    return result;
}


void gymUnloadButton(GymButton btn)
{
    UnloadTexture(btn.tx);  
}

void gymRenderNN(ViewBee *bee, GymRect r)
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
    float layer_hpad = nn_width / bee->nn.arch_count;
    for (size_t l = 0; l < bee->nn.arch_count; ++l) {
        float layer_vpad1 = nn_height / bee->nn.as[l].cols;
        float cx1 = nn_x + l * layer_hpad + layer_hpad / 2;
        for (size_t i = 0; i < bee->nn.as[l].cols; ++i) {
            float cy1 = nn_y + i * layer_vpad1 + layer_vpad1 / 2;
            if (l + 1 < bee->nn.arch_count) {
                float layer_vpad2 = nn_height / bee->nn.as[l + 1].cols;
                for (size_t j = 0; j < bee->nn.as[l + 1].cols; ++j) {
                    // i - rows of ws
                    // j - cols of ws
                    float cx2 = nn_x + (l + 1) * layer_hpad + layer_hpad / 2;
                    float cy2 = nn_y + j * layer_vpad2 + layer_vpad2 / 2;
                    float value = sigmoidf(MatAt(bee->nn.ws[l], i, j));
                    high_color.a = floorf(255.f * value);
                    Vector2 start = {cx1, cy1};
                    Vector2 end = {cx2, cy2};
                    DrawLineEx(start, end, thick,
                    ColorAlphaBlend(low_color, high_color, alpha_color));
                }
            }
            if (l > 0) {
                high_color.a = floorf(255.f * sigmoidf(RowAt(bee->nn.bs[l - 1], i)));
                DrawPoly((Vector2){.x = cx1, .y = cy1}, 6, neuron_radius, 0.0f, ColorAlphaBlend(low_color, high_color, alpha_color));
            } else {
                DrawPoly((Vector2){.x = cx1, .y = cy1}, 6, neuron_radius, 0.0f, ColorAlphaBlend(low_color, high_color, alpha_color));
            }
        }
        if (l != 0 && l != bee->nn.arch_count-1) {
            int result = 0;
            result += gymRenderButton(bee->plus, (Vector2){.x = cx1, .y = r.y-10});
            result -= gymRenderButton(bee->minus, (Vector2){.x = cx1, .y = r.y+20});
            if ((bee->inner_layers[l-1] < MAX_PERCEPTRONS && result > 0) || (bee->inner_layers[l-1] > MIN_PERCEPTRONS && result < 0)) {
                bee->modified = true;
                bee->inner_layers[l-1] += result;
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
                .x = r.x + r.w / 2 - full_width / 2 + x * cell_width,
                .y = r.y + y * cell_height,
                .w = cell_width,
                .h = cell_height,
            };
            DrawRectangle(ceilf(slot.x), ceilf(slot.y), ceilf(slot.w), ceilf(slot.h), color);
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

void gymPlotFree(GymPlot *gp)
{
    if (!gp) {
        return;
    }
    free(gp->items);
    gp->items = NULL;
    gp->count = 0;
    gp->capacity = 0;
}

void gymSliderHorizontal(float *value, bool *slider_dragging, float rx, float ry, float width, float knob_radius, Color c_slide, Color c_dot) 
{
    Vector2 bar_size = {
        .x = width - 2 * knob_radius,
        .y = knob_radius * 0.25,
    };
    
    Vector2 bar_position = {
        .x = rx + knob_radius,
        .y = ry + knob_radius / 2 - bar_size.y / 2
    };
    
    DrawRectangleV(bar_position, bar_size, c_slide);

    Vector2 knob_position = {
        .x = bar_position.x + bar_size.x * (*value),
        .y = ry + knob_radius / 2
    };
    
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
            pixels_u32[y * stride + x] = (0xFF << (8 * 3)) | (pixel << (8 * 2)) | (pixel << (8 * 1)) | (pixel << (8 * 0));
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

        gymSliderVertical(
            slider_position, slider_dragging, r.x + r.w + 200,
            r.y + r.h * widget_padding_multip,
            r.h * (widget_height_multip - widget_padding_multip), 10,
            ColorAlphaBlend(low_color, high_color, alpha_color), ORANGE
        );

        start = (size_t)(*slider_position * (float)(bee->t.rows - elements_per_page));
    }
    size_t next_pos = 0;
    for (size_t i = start; i < bee->t.rows && i < start + elements_per_page && next_pos < buff_size - inner_buff_size; ++i) {
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
    DrawTextBoxed(
        bee->font, buffer_print, (Rectangle){r.x, r.y, r.w + 200, r.h},
        s, pad, false, ORANGE
    );
}

ViewBee viewBeeNew(Font font, GymButton minus, GymButton plus, int inner_layers_count, int inner_layers[MAX_INNER_LAYERS])
{
    const size_t max_epoch = 200 * 1000;
    const size_t epochs_per_frame = 300;
    size_t epoch = 0;
    const float rate = 0.7f;
    int total_layers_count = 2+inner_layers_count; 

    Region temp = regionAllocAlloc(8 * KILO * KILO);
    FlowersDataset fl = flowersDatasetNew(Location_6_60);
    Mat t = flowersToMat(fl);

    size_t *arch = NNMalloc(sizeof(size_t) * total_layers_count);
    for (int i = 0; i < total_layers_count; ++i) {
        if (i == 0) {
            arch[i] = fl.cols-1;
            continue;
        }
        if (i == total_layers_count-1) {
            arch[i] = 1;
            continue;
        }
        arch[i] = inner_layers[i-1];
    }

    NN nn = nnAlloc(NULL, arch, total_layers_count);
    nnRand(nn, -1, 1);
    GymPlot plot = {0};

    ViewBee bee = {
        .max_epoch = max_epoch,
        .epochs_per_frame = epochs_per_frame,
        .epoch = epoch,
        .rate = rate,
        .inner_layers_count = inner_layers_count,
        .paused = true,
        .reset = false,
        .modified = false,
        .temp = temp,
        .fl = fl,
        .t = t,
        .nn = nn,
        .plot = plot,
        .font = font,
        .minus = minus,
        .plus = plus
    };

    for (int i = 0; i < MAX_INNER_LAYERS; i++) {
        bee.inner_layers[i] = inner_layers[i];
    }

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
    bee->inner_layers_count = 0;
    bee->paused = true;
    bee->reset = false;
    bee->modified = false;
    regionFree(&bee->temp);
    flowersDatasetFree(&bee->fl);
    matFree(&bee->t);
    nnFree(&bee->nn);
    gymPlotFree(&bee->plot);
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
        gymPlotFree(&bee->plot);
    }

    for (size_t i = 0;i < bee->epochs_per_frame && !bee->paused && bee->epoch < bee->max_epoch;++i) {
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
    char description_buffer[256];
    snprintf(description_buffer, sizeof(description_buffer),
             "<| Epoch: %zu/%zu, Rate: %f, Cost: %f, Temporary Memory: %zu kB |>",
             bee->epoch, bee->max_epoch, bee->rate, nnCost(bee->nn, bee->t),
             RegionOccupiedBytes(&bee->temp) / KILO);
    DrawTextEx(bee->font, description_buffer, CLITERAL(Vector2){.x = 40, .y = 40}, h * 0.02, 0, DEEPOCEAN);

    GymRect r;
    r.w = w * widget_height_multip;
    r.h = h * widget_width_multip;
    r.x = 0;
    r.y = h / 2 - r.h / 2;

    GymLayoutBegin(GloHorz, r, 3, 10);
        gymPlot(bee->plot, GymLayoutSlot(), ORANGE);
        gymRenderNN(bee, GymLayoutSlot());
        viewBeeLearn(bee, GymLayoutSlot(), &slider_position, &slider_dragging);
    GymLayoutEnd();
    
    char controles_buffer[256];
    int inner_layers_count = bee->inner_layers_count;
    inner_layers_count -= gymRenderButton(bee->minus, CLITERAL(Vector2){ .x = r.w/3 +40, .y = h*controles_height_multip}); 
    inner_layers_count += gymRenderButton(bee->plus, CLITERAL(Vector2){ .x = r.w/3 + 80, .y = h*controles_height_multip});
    if (bee->inner_layers_count < inner_layers_count && bee->inner_layers_count < MAX_INNER_LAYERS) {
        bee->inner_layers_count = inner_layers_count;
        bee->inner_layers[bee->inner_layers_count-1] = 5;
        bee->modified = true;
    }
    if (bee->inner_layers_count > inner_layers_count && bee->inner_layers_count > MIN_INNER_LAYERS ) {
        bee->inner_layers_count = inner_layers_count;
        bee->modified = true;
    }

    snprintf(controles_buffer, sizeof(controles_buffer),
             "Bee brain arch: [ %i %i %i %i ]", 
             bee->inner_layers[0], bee->inner_layers[1], bee->inner_layers[2], bee->inner_layers[3]
             );
    DrawTextEx(bee->font, controles_buffer, CLITERAL(Vector2){.x = 20, .y = h*controles_height_multip+10}, h * 0.016, 0, YELLOW);

    EndDrawing();

    RegionReset(&bee->temp);
}

bool isModified(ViewBee *bee)
{
    return  bee->modified;
}
