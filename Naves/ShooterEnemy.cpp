#include "ShooterEnemy.h"
#include <cmath>
#include <iostream>
using namespace std;

ShooterEnemy::ShooterEnemy(float x, float y, Game* game)
	: Enemy(x, y, game) {
	
	lives = 2;
	speed = 0.8f; // Más lento que BasicEnemy
	minDistance = 150.0f; // Distancia mínima
	maxDistance = 250.0f; // Distancia máxima
	shootCadence = 90; // Dispara cada 3 segundos aprox (90 frames a 30 FPS)
	shootTimer = 0;
	
	// Sprite específico para ShooterEnemy
	texture = game->getTexture("res/shooter_enemy.png");
	aMoving = new Animation("res/shooter_enemy_anim.png", width, height,
		108, 40, 6, 3, game);
	animation = aMoving;
}

void ShooterEnemy::update() {
	animation->update();
	
	// Actualizar temporizador de disparo
	if (shootTimer > 0) {
		shootTimer--;
	}
	
	// El movimiento se actualiza desde GameLayer
}

void ShooterEnemy::maintainDistance(Player* player) {
	if (player == nullptr) return;
	
	// Calcular distancia al jugador
	float dx = player->x - x;
	float dy = player->y - y;
	float distance = sqrt(dx * dx + dy * dy);
	
	// Si está muy lejos (fuera de vista), acercarse rápidamente
	if (distance > maxDistance * 2.0f) {
		// Moverse rápido hacia el jugador para volver a la vista
		vx = (dx / distance) * speed * 2.0f;
		vy = (dy / distance) * speed * 2.0f;
		return;
	}
	
	// Si está muy cerca, alejarse
	if (distance < minDistance) {
		// Huir del jugador
		vx = -(dx / distance) * speed;
		vy = -(dy / distance) * speed;
	}
	// Si está muy lejos, acercarse
	else if (distance > maxDistance) {
		// Acercarse al jugador
		vx = (dx / distance) * speed;
		vy = (dy / distance) * speed;
	}
	// Si está en la distancia óptima, moverse lateral o quedarse quieto
	else {
		// Movimiento lateral para dificultar ser alcanzado
		// Perpendicular al jugador
		vx = -(dy / distance) * speed * 0.5f;
		vy = (dx / distance) * speed * 0.5f;
	}
}

EnemyProjectile* ShooterEnemy::shoot(Player* player) {
	if (player == nullptr || shootTimer > 0) {
		return nullptr;
	}
	
	// Calcular distancia
	float dx = player->x - x;
	float dy = player->y - y;
	float distance = sqrt(dx * dx + dy * dy);
	
	// Solo disparar si está dentro del rango
	if (distance <= maxDistance * 1.2f) {
		shootTimer = shootCadence;
		cout << "¡ShooterEnemy dispara!" << endl;
		return new EnemyProjectile(x, y, player->x, player->y, game);
	}
	
	return nullptr;
}

bool ShooterEnemy::canShoot() {
	return shootTimer == 0;
}

void ShooterEnemy::resetShootTimer() {
	shootTimer = shootCadence;
}
