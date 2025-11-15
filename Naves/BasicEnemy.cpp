#include "BasicEnemy.h"

BasicEnemy::BasicEnemy(float x, float y, Game* game)
	: Enemy(x, y, game) {
	lives = 3;
}

void BasicEnemy::update() {
	animation->update();
	vx = -1;
	x = x + vx;
}
