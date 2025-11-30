#pragma once
#include "Enemy.h"
#include "Player.h"

class ChargeEnemy : public Enemy
{
public:
	ChargeEnemy(float x, float y, Game* game);
	void update() override;
	void patrolBorder(); // Patrullar por el borde
	void checkAndCharge(Player* player); // Verificar si debe embestir
	
	float normalSpeed;
	float chargeSpeed;
	float detectionRange;
	bool isCharging;
	float chargeDirectionX;
	float chargeDirectionY;
	int patrolDirection; // 0=derecha, 1=abajo, 2=izquierda, 3=arriba
	float patrolTimer;
};
