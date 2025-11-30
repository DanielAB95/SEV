#include "EnemyProjectile.h"
#include <cmath>

EnemyProjectile::EnemyProjectile(float x, float y, float targetX, float targetY, Game* game)
	: Actor("res/disparo_enemigo.png", x, y, 20, 20, game) {
	
	speed = 4.0f;
	
	// Calcular dirección hacia el objetivo
	float dx = targetX - x;
	float dy = targetY - y;
	float distance = sqrt(dx * dx + dy * dy);
	
	// Normalizar y aplicar velocidad
	if (distance > 0) {
		vx = (dx / distance) * speed;
		vy = (dy / distance) * speed;
	} else {
		vx = speed;
		vy = 0;
	}
}

void EnemyProjectile::update() {
	// El movimiento lo maneja Space::update()
}
