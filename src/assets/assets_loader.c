#include <raylib.h>
#include <stdlib.h>
#include "assets_loader.h"
#include "logo.h"
#include "minus_button.h"
#include "plus_button.h"

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
        img.format =  MINUS_BUTTON_FORMAT;
        img.height = MINUS_BUTTON_HEIGHT;
        img.width = MINUS_BUTTON_WIDTH;
        img.data = MINUS_BUTTON_DATA;
        img.mipmaps = 1;
        break;
    case PlusButton:
        img.format =  PLUS_BUTTON_FORMAT;
        img.height = PLUS_BUTTON_HEIGHT;
        img.width = PLUS_BUTTON_WIDTH;
        img.data = PLUS_BUTTON_DATA;
        img.mipmaps = 1;
        break;
    default:
        exit(1);
    }

    return LoadTextureFromImage(img);
}
