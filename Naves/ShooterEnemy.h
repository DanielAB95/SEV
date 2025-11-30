#pragma once
#include "Enemy.h"
#include "Player.h"
#include "EnemyProjectile.h"

class ShooterEnemy : public Enemy
{
public:
	ShooterEnemy(float x, float y, Game* game);
	void update() override;
	void maintainDistance(Player* player); // Mantener distancia del jugador
	void maintainDistance(Player* player, float scrollX, float scrollY); // Versión con scroll
	EnemyProjectile* shoot(Player* player); // Disparar al jugador
	bool canShoot();
	void resetShootTimer();
	bool isVisibleOnScreen(float scrollX, float scrollY); // Verificar si está en pantalla
	
	float speed;
	float minDistance; // Distancia mínima al jugador
	float maxDistance; // Distancia máxima al jugador
	int shootCadence; // Cadencia de disparo
	int shootTimer; // Temporizador de disparo
};
