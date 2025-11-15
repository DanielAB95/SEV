#include "SMovingEnemy.h"

SMovingEnemy::SMovingEnemy(float x, float y, Game* game)
	: Enemy(x, y, game) {
	vy = -5;
	lives = 1;
}

void SMovingEnemy::update() {
	animation->update();
	if (y >= HEIGHT * 0.92) {
		vy = -5;
	}
	if (y <= HEIGHT * 0.08) {
		vy = 5;
	}
	vx = -1;
	x = x + vx;
	y = y + vy;
}
