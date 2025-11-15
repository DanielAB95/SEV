#include "PowerUp.h"

PowerUp::PowerUp(string filename, float x, float y, int width, int height, Game* game) 
	: Actor (filename, x, y, width, height, game) {
	
	vx = -2;
}

void PowerUp::update() {
	x = x + vx;
}

void PowerUp::effect(Player* player) {

}


