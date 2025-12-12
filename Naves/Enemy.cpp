#include "Enemy.h"

Enemy::Enemy(float x, float y, Game* game)
	: Actor("res/enemigo.png", x, y, 32, 32, game) {

	vx = -1;
	lives = 1; // Inicializar vidas del enemigo

}

void Enemy::update() {
	animation->update();
	// El movimiento lo maneja Space::update()
	// vx ya está establecido en -1
}

void Enemy::draw(float scrollX, float scrollY) {
	animation->draw(x - scrollX, y - scrollY);
}
