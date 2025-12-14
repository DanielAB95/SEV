#include "EnemyProjectile.h"
#include <cmath>
#include <iostream>
using namespace std;

EnemyProjectile::EnemyProjectile(float x, float y, float targetX, float targetY, Game* game)
	: Actor("res/disparo_enemigo.png", x, y, 20, 20, game) {
	
	speed = 4.0f;
	maxBounces = 4; // Puede rebotar hasta 5 veces
	currentBounces = 0;
	
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
	// Después del movimiento, verificar colisiones con paredes
}

void EnemyProjectile::bounce(bool horizontal) {
	if (currentBounces >= maxBounces) {
		return; // Ya alcanzó el máximo de rebotes
	}
	
	if (horizontal) {
		// Rebotar en pared vertical (izquierda/derecha)
		vx = -vx;
		cout << "¡Proyectil enemigo rebota en pared horizontal! Rebote " << (currentBounces + 1) << "/" << maxBounces << endl;
	} else {
		// Rebotar en pared horizontal (arriba/abajo)
		vy = -vy;
		cout << "¡Proyectil enemigo rebota en pared vertical! Rebote " << (currentBounces + 1) << "/" << maxBounces << endl;
	}
	
	currentBounces++;
}

bool EnemyProjectile::checkWallCollision() {
	// Este método será llamado desde GameLayer después de space->update()
	// Retorna true si debe ser eliminado (alcanzó máximo de rebotes)
	return currentBounces >= maxBounces;
}
