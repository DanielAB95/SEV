#include "Pad.h"

Pad::Pad(float x, float y, Game* game)
    : Actor("res/pad.png", x, y, 120, 120, game) {
}

Pad::Pad(float x, float y, int width, int height, Game* game)
    : Actor("res/pad.png", x, y, width, height, game) {
}

float Pad::getOrientationX(float clickX) {
    return clickX - x;
}

// Implementación análoga para el eje vertical
float Pad::getOrientationY(float clickY) {
    return clickY - y;
}