#include "CollectibleItem.h"
#include <cmath>

CollectibleItem::CollectibleItem(float x, float y, Game* game)
	: Actor("res/item.png", x, y, 20, 20, game) {
	collected = false;
	points = 10;
}

void CollectibleItem::update() {
	// Animación simple de movimiento vertical
	y += (float)(sin(SDL_GetTicks() * 0.005) * 0.5);
}

void CollectibleItem::collect() {
	collected = true;
}