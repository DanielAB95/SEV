#pragma once
#include "Actor.h"

class Pad : public Actor {
public:
    Pad(float x, float y, Game* game);
    Pad(float x, float y, int width, int height, Game* game); // Constructor con tamaño personalizado
    float getOrientationX(float clickX);
    float getOrientationY(float clickY);
};