/// Copyright (c) 2024 Bartosz Lenart

#ifndef ASSETS_LOADER_H
#define ASSETS_LOADER_H

#include <raylib.h>

/// AssetTexture2D are predefined types of textures to load.
enum AssetTexture2D {
    Logo,
};

/// assetLoad loads assets to the GPU and returns texture 2d.
/// The Texture user is required to unload texture from the GPU.
/// To unload use `UnloadTexture defined in raylib.h`.
///
/// a - AssetTexture2D predefined type of the texture to be loaded.
Texture2D assetLoad(enum AssetTexture2D a);

#endif // !ASSETS_LOADER_H

