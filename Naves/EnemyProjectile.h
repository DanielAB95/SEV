#pragma once
#include "Actor.h"

class EnemyProjectile : public Actor
{
public:
	EnemyProjectile(float x, float y, float targetX, float targetY, Game* game);
	void update();
	void bounce(bool horizontal); // Rebotar (horizontal: true para paredes izq/der, false para arriba/abajo)
	bool checkWallCollision(); // Detectar colisión con paredes y rebotar si es necesario
	float speed;
	int maxBounces; // Número máximo de rebotes
	int currentBounces; // Rebotes actuales
};
