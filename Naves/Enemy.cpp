#include "Enemy.h"

Enemy::Enemy(float x, float y, Game* game)
	: Actor("res/enemigo.png", x, y, 32, 32, game) {

	vx = -1;
	lives = 10; // Inicializar vidas del enemigo
	
	// INICIALIZAR STATS BASE
	maxLives = 10;
	damage = 5;
	moveSpeed = 1.0f;
	coinReward = 80; // AUMENTADO: Recompensa base de 80 monedas (era 15)
}

void Enemy::update() {
	animation->update();
	// El movimiento lo maneja Space::update()
	// vx ya esta establecido en -1
}

void Enemy::draw(float scrollX, float scrollY) {
	animation->draw(x - scrollX, y - scrollY);
}

void Enemy::scaleStatsForLevel(int level) {
	// Progresion mas suave: 15% por nivel
	// Nivel 1: 1.0x, Nivel 2: 1.15x, Nivel 3: 1.30x, Nivel 4: 1.45x, Nivel 5: 1.60x
	float multiplier = 1.0f + ((level - 1) * 0.15f);
	
	// Calcular con float y redondear para mantener granularidad
	float scaledLives = maxLives * multiplier;
	float scaledDamage = damage * multiplier;
	
	maxLives = static_cast<int>(scaledLives + 0.5f); // Redondeo correcto
	lives = maxLives; // Restaurar vida completa
	damage = static_cast<int>(scaledDamage + 0.5f); // Redondeo correcto
	if (damage < 5) damage = 5; // Minimo 5 de dano
	moveSpeed = moveSpeed * multiplier;
	
	// MEJORADO: Escalar recompensa de monedas MÁS generosamente
	// Bonus adicional por nivel para hacer el dinero más abundante
	float coinMultiplier = multiplier + (0.25f * (level - 1)); // Bonus extra por nivel
	float scaledCoins = coinReward * coinMultiplier;
	coinReward = static_cast<int>(scaledCoins + 0.5f);
	if (coinReward < 60) coinReward = 60; // Mínimo 60 monedas (era 10)
}
