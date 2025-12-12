#include "Tile.h"

Tile::Tile(string filename, float x, float y, Game* game)
	: Actor(filename, x, y, 32, 32, game) {

}

void Tile::draw(float scrollX, float scrollY) {
	Actor::draw(scrollX, scrollY); // Llamar al método draw del padre
}