#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "raylib.h"
#include "location_hash.h"

static inline BzzQuadTreeNode* bzzCreateBzzQuadTreeNode(BzzBoundingBox boundary, BzzBoundingBox box) {
    BzzQuadTreeNode* node = (BzzQuadTreeNode*)malloc(sizeof(BzzQuadTreeNode));
    node->boundary = boundary;
    node->box = box;
    for (int i = 0; i < 4; i++) {
        node->children[i] = NULL;
    }
    return node;
}

inline Vector2 bzzGetCenterBox(BzzBoundingBox box)
{
    Vector2 center = {
        .x = (box.x_min + box.x_max) / 2.0f,
        .y = (box.y_min + box.y_max) / 2.0f
    };
    return center;
}

void bzzInsertBox(BzzQuadTreeNode** root, BzzBoundingBox box, BzzBoundingBox boundary) {
    if (*root == NULL) {
        *root = bzzCreateBzzQuadTreeNode(boundary, box);
        return;
    }

    Vector2 mid = bzzGetCenterBox(boundary);
    Vector2 center = bzzGetCenterBox(box); 
    int quadrant = 0; // TL

    if (center.x > mid.x) {
        if (center.y > mid.y) quadrant = 3; // TR
        else quadrant = 1; // BR 
    } else {
        if (center.y > mid.y) quadrant = 2; // TL 
        else quadrant = 0; 
    }

    if ((*root)->children[quadrant] == NULL) {
        BzzBoundingBox childBoundary;

        switch (quadrant) {
        case 0:
            childBoundary.x_min = boundary.x_min;
            childBoundary.y_min = boundary.y_min;
            childBoundary.x_max = mid.x;
            childBoundary.y_max = mid.y;
            break;
        case 1:
            childBoundary.x_min = mid.x;
            childBoundary.y_min = boundary.y_min;
            childBoundary.x_max = boundary.x_max;
            childBoundary.y_max = mid.y;
            break;
        case 2:
            childBoundary.x_min = boundary.x_min;
            childBoundary.y_min = mid.y;
            childBoundary.x_max = mid.x;
            childBoundary.y_max = boundary.y_max;
            break;
        case 3:
            childBoundary.x_min = mid.x;
            childBoundary.y_min = mid.y;
            childBoundary.x_max = boundary.x_max;
            childBoundary.y_max = boundary.y_max;
            break;
        }

        (*root)->children[quadrant] = bzzCreateBzzQuadTreeNode(childBoundary, box);
    } else {
        bzzInsertBox(&((*root)->children[quadrant]), box, (*root)->children[quadrant]->boundary);
    }
}

BzzBoundingBox bzzQuadThreeLookup(BzzQuadTreeNode* root, float x, float y) {
    BzzQuadTreeNode* current = root;

    while (current != NULL) {
        Vector2 mid = bzzGetCenterBox(current->boundary); 
        int quadrant = 0;

        if (x > mid.x) {
            if (y > mid.y) quadrant = 3;
            else quadrant = 1;
        } else {
            if (y > mid.y) quadrant = 2;
            else quadrant = 0; 
        }

        if (current->children[quadrant] == NULL) {
            return current->box;
        }

        current = current->children[quadrant];
    }

    BzzBoundingBox defaultbox = { 
        .x_min = 0.0f, .y_min = 0.0f,
        .x_max = 10.0f, .y_max = 10.0f 
    };
    return defaultbox;
}

void bzzFreeQuadTree(BzzQuadTreeNode* root) {
    if (root == NULL) return;

    for (int i = 0; i < 4; i++) {
        bzzFreeQuadTree(root->children[i]);
    }

    free(root);
}

