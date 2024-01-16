/// Copyright (c) 2024 Bartosz Lenart

#include <stdio.h>
#include <raylib.h>
#include <stdlib.h>
#include "assets_loader.h"
#include "logo.h"
#include "minus_button.h"
#include "plus_button.h"
#include "update_button.h"
#include "bee_button.h"
#include "bee_flying.h"
#include "honeycomb.h"
#include "map_button.h"
#include "learn_button.h"

#define printfError(str) printf("ERROR: %s \n", str)

Texture2D assetLoad(enum AssetTexture2D a)
{
    Image img = {0};
    switch (a) {
    case Logo:
        img.format = LOGO_FORMAT;
        img.height = LOGO_HEIGHT;
        img.width = LOGO_WIDTH;
        img.data = LOGO_DATA;
        img.mipmaps = 1;
        break;
    case MinusButton:
        img.format = MINUS_BUTTON_FORMAT;
        img.height = MINUS_BUTTON_HEIGHT;
        img.width = MINUS_BUTTON_WIDTH;
        img.data = MINUS_BUTTON_DATA;
        img.mipmaps = 1;
        break;
    case PlusButton:
        img.format = PLUS_BUTTON_FORMAT;
        img.height = PLUS_BUTTON_HEIGHT;
        img.width = PLUS_BUTTON_WIDTH;
        img.data = PLUS_BUTTON_DATA;
        img.mipmaps = 1;
        break;
    case UpdateButton:
        img.format = UPDATE_BUTTON_FORMAT;
        img.height = UPDATE_BUTTON_HEIGHT;
        img.width = UPDATE_BUTTON_WIDTH;
        img.data = UPDATE_BUTTON_DATA;
        img.mipmaps = 1;
        break;
    case BeeButton:
        img.format = BEE_BUTTON_FORMAT;
        img.height = BEE_BUTTON_HEIGHT;
        img.width = BEE_BUTTON_WIDTH;
        img.data = BEE_BUTTON_DATA;
        img.mipmaps = 1;
        break;
    case BeeFlying: 
        img.format = BEE_FLYING_FORMAT;
        img.height = BEE_FLYING_HEIGHT;
        img.width = BEE_FLYING_WIDTH;
        img.data = BEE_FLYING_DATA;
        img.mipmaps = 1;
        break;
    case Honeycomb:
        img.format = HONEYCOMB_FORMAT;
        img.height = HONEYCOMB_HEIGHT;
        img.width = HONEYCOMB_WIDTH;
        img.data = HONEYCOMB_DATA;
        img.mipmaps = 1;
        break;
    case MapButton:
        img.format = MAP_BUTTON_FORMAT;
        img.height = MAP_BUTTON_HEIGHT;
        img.width = MAP_BUTTON_WIDTH;
        img.data = MAP_BUTTON_DATA;
        img.mipmaps = 1;
        break;
    case LearnButton:
        img.format = LEARN_BUTTON_FORMAT;
        img.height = LEARN_BUTTON_HEIGHT;
        img.width = LEARN_BUTTON_WIDTH;
        img.data = LEARN_BUTTON_DATA;
        img.mipmaps = 1;
        break;
    default:
        printfError("unknown resource");
        exit(1);
    }

    printf("image: %p, %i, %0.3d, %0.3d \n", img.data, img.format, img.width, img.height);

    return LoadTextureFromImage(img);
}

