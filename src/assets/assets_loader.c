#include <raylib.h>
#include <stdlib.h>
#include "assets_loader.h"
#include "logo.h"

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
    default:
        exit(1);
    }

    return LoadTextureFromImage(img);
}
