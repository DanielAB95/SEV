#include "BasicEnemy.h"
#include <cmath>

BasicEnemy::BasicEnemy(float x, float y, Game* game)
	: Enemy(x, y, game) {
	lives = 3;
	speed = 1.5f; // Velocidad moderada
	
	// Sprite específico para BasicEnemy
	texture = game->getTexture("res/AlienBasico.png");
	aMoving = new Animation("res/AlienBasico.png", width, height,
		192, 32, 5, 6, game);
	animation = aMoving;
}

void BasicEnemy::update() {
	animation->update();
	// El movimiento se actualiza desde GameLayer llamando a moveTowardsPlayer
}

void BasicEnemy::moveTowardsPlayer(Player* player) {
	if (player == nullptr) return;
	
	// Calcular dirección hacia el jugador
	float dx = player->x - x;
	float dy = player->y - y;
	
	// Calcular distancia
	float distance = sqrt(dx * dx + dy * dy);
	
	// Normalizar y aplicar velocidad
	if (distance > 0) {
		vx = (dx / distance) * speed;
		vy = (dy / distance) * speed;
	}
}
