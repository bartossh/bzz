/// Copyright (c) 2024 Bartosz Lenart

#ifndef LOC_H_
#define LOC_H_

#include "raylib.h"

/// BzzBoundingBox describes the borders of the quadrant. 
typedef struct {
    float x_min;
    float y_min;
    float x_max;
    float y_max;
} BzzBoundingBox;

/// BzzQuadTreeNode is a node containing all the hased location in quadrents.
typedef struct BzzQuadTreeNode {
    BzzBoundingBox boundary;
    BzzBoundingBox box;
    struct BzzQuadTreeNode* children[4]; // TL, TR, BL, BR
} BzzQuadTreeNode;

/// bzzGetCenterBox returns center of the BzzBoundingBox.
///
/// box - BzzBoundingBox to caculate center.
Vector2 bzzGetCenterBox(BzzBoundingBox box);

/// bzInsertBox inserts point into BzzQuadTreeNode.
///
/// root - pointer to pointer to root that is the beginning of the three. 
/// box - new box to be added to the three.
/// boundary - boundaries of the whole three.
void bzzInsertBox(BzzQuadTreeNode** root, BzzBoundingBox box, BzzBoundingBox boundary);

/// bzzQuadThreeLookup performs lookup for the closest existing node containing BzzBoundingBox in the three. 
/// Returns BzzBoundingBox.
///
/// root - pointer to pointer to root that is the beginning of the three.
/// x - x coordinate.
/// y - y coordinate.
BzzBoundingBox bzzQuadThreeLookup(BzzQuadTreeNode* root, float x, float y);

/// bzzFreeQuadTree frees tree from memory.
///
///root - pointer to root that is the beginning of the three.
void bzzFreeQuadTree(BzzQuadTreeNode* root);

#endif // !LOC_H
