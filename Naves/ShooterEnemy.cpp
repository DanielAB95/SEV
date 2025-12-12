#include "ShooterEnemy.h"
#include <cmath>
#include <iostream>
using namespace std;

ShooterEnemy::ShooterEnemy(float x, float y, Game* game)
	: Enemy(x, y, game) {
	
	lives = 2;
	speed = 1.5f; // Aumentada para mejor movimiento
	minDistance = 150.0f; // Distancia mínima
	maxDistance = 250.0f; // Distancia máxima
	shootCadence = 90; // Dispara cada 3 segundos aprox (90 frames a 30 FPS)
	shootTimer = 0;
	
	// Sprite específico para ShooterEnemy
	texture = game->getTexture("res/AlienTirador.png");
	aMoving = new Animation("res/AlienTirador.png", width, height,
		96, 32, 10, 3, game);
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

bool ShooterEnemy::isVisibleOnScreen(float scrollX, float scrollY) {
	// Verificar si el enemigo está dentro de los límites de la pantalla visible
	// SIN margen extra - debe estar realmente en pantalla
	float screenLeft = scrollX;
	float screenRight = scrollX + WIDTH;
	float screenTop = scrollY;
	float screenBottom = scrollY + HEIGHT;
	
	bool visible = (x + width / 2 > screenLeft && 
	                x - width / 2 < screenRight &&
	                y + height / 2 > screenTop && 
	                y - height / 2 < screenBottom);
	
	return visible;
}

void ShooterEnemy::maintainDistance(Player* player) {
	// Versión antigua sin scroll - mantener para compatibilidad
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
		vy = (dx / distance) * speed;
	}
	// Si está en la distancia óptima, moverse lateral o quedarse quieto
	else {
		// Movimiento lateral para dificultar ser alcanzado
		// Perpendicular al jugador
		vx = -(dy / distance) * speed * 0.5f;
		vy = (dx / distance) * speed * 0.5f;
	}
}

void ShooterEnemy::maintainDistance(Player* player, float scrollX, float scrollY) {
	if (player == nullptr) return;
	
	// Calcular distancia al jugador
	float dx = player->x - x;
	float dy = player->y - y;
	float distance = sqrt(dx * dx + dy * dy);
	
	// Verificar si está visible en pantalla
	bool visible = isVisibleOnScreen(scrollX, scrollY);
	
	// DEBUG: Mostrar información
	static int debugCounter = 0;
	debugCounter++;
	if (debugCounter % 60 == 0) { // Cada 2 segundos aprox
		cout << "ShooterEnemy en (" << x << ", " << y << ") - ";
		cout << "Visible: " << (visible ? "SI" : "NO") << " - ";
		cout << "Distancia: " << distance << " - ";
		cout << "Player: (" << player->x << ", " << player->y << ")" << endl;
		cout << "Scroll: (" << scrollX << ", " << scrollY << ")" << endl;
	}
	
	// PRIORIDAD 1: Si NO está visible en pantalla O está muy lejos del jugador
	// Perseguir activamente hasta estar visible Y cerca
	if (!visible || distance > maxDistance * 1.5f) {
		if (!visible) {
			cout << ">>> ShooterEnemy FUERA DE PANTALLA - Persiguiendo" << endl;
		}
		// Normalizar y multiplicar por velocidad alta
		float normalizedDx = dx / distance;
		float normalizedDy = dy / distance;
		vx = normalizedDx * speed * 3.0f; // Triple velocidad para perseguir
		vy = normalizedDy * speed * 3.0f;
		return;
	}
	
	// PRIORIDAD 2: Comportamiento normal cuando está visible y en rango razonable
	// Si está muy cerca, alejarse
	if (distance < minDistance) {
		// Huir del jugador
		float normalizedDx = dx / distance;
		float normalizedDy = dy / distance;
		vx = -normalizedDx * speed;
		vy = -normalizedDy * speed;
		cout << ">>> ShooterEnemy MUY CERCA - Alejándose" << endl;
	}
	// Si está lejos pero en rango de disparo, acercarse lentamente
	else if (distance > maxDistance) {
		// Acercarse al jugador
		float normalizedDx = dx / distance;
		float normalizedDy = dy / distance;
		vx = normalizedDx * speed;
		vy = normalizedDy * speed;
		cout << ">>> ShooterEnemy en rango - Acercándose lentamente" << endl;
	}
	// Si está en la distancia óptima, moverse lateralmente
	else {
		// Movimiento lateral para dificultar ser alcanzado
		// Perpendicular al jugador
		float normalizedDx = dx / distance;
		float normalizedDy = dy / distance;
		vx = -normalizedDy * speed * 0.5f;
		vy = normalizedDx * speed * 0.5f;
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
