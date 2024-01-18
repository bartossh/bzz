/// Copyright (c) 2024 Bartosz Lenart

#include <raylib.h>
#include <stdlib.h>
#include "game.h"
#include "../assets/assets_loader.h"

#define TexturesSize(txs) sizeof(txs)/sizeof(txs[0])

const enum AssetTexture2D textures[36] = {
    FlowerA,
    FlowerB,
    FlowerC,
    FlowerD,
    FlowerE,
    FlowerF,
    FlowerG,
    FlowerH,
    FlowerI,
    FlowerJ,
    FlowerK,
    FlowerL,
    FlowerM,
    FlowerN,
    FlowerO,
    FlowerP,
    FlowerQ,
    FlowerR,
    FlowerS,
    FlowerT,
    FlowerU,
    FlowerW,
    FlowerV,
    FlowerX,
    FlowerY,
    FlowerZ,
    Flower1,
    Flower2,
    Flower3,
    Flower4,
    Flower5,
    Flower6,
    Flower7,
    Flower8,
    Flower9,
    Flower0,
};

BzzObject bzzObjectNewBee(Color color)
{
    BzzObject obj = {
        .tx = assetLoad(BeeFlying),
        .color = color
    };

    return obj;
}

int bzzGetTotalNumberOfAvaliablefFlowersTextures(void)
{
    return TexturesSize(textures);
}

BzzObject bzzObjectNewFlower(Color color, int next)
{
    if (next >= bzzGetTotalNumberOfAvaliablefFlowersTextures()) {
        next = 0;
    }
    enum AssetTexture2D tx = textures[next];

    BzzObject obj = {
        .tx = assetLoad(tx),
        .color = color
    };

    return obj;
}

void bzzUnloadObject(BzzObject obj)
{
    UnloadTexture(obj.tx);  
}
