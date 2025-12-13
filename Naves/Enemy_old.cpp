#include "Enemy.h"

Enemy::Enemy(float x, float y, Game* game)
	: Actor("res/enemigo.png", x, y, 32, 32, game) {

	vx = -1;
	lives = 10; // Inicializar vidas del enemigo
	
	// INICIALIZAR STATS BASE
	maxLives = 10;
	damage = 5;
	moveSpeed = 1.0f;
}

void Enemy::update() {
	animation->update();
	// El movimiento lo maneja Space::update()
	// vx ya está establecido en -1
}

void Enemy::draw(float scrollX, float scrollY) {
	animation->draw(x - scrollX, y - scrollY);
}

void Enemy::scaleStatsForLevel(int level) {
	// Escalar stats según el nivel (cada nivel aumenta un 20%)
	float multiplier = 1.0f + ((level - 1) * 0.2f);
	
	maxLives = static_cast<int>(maxLives * multiplier);
	lives = maxLives; // Restaurar vida completa
	damage = static_cast<int>(damage * multiplier);
	if (damage < 1) damage = 1; // Mínimo 1 de daño
	moveSpeed = moveSpeed * multiplier;
}
