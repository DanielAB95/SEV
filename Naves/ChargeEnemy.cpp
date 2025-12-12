#include "ChargeEnemy.h"
#include <cmath>
#include <iostream>
using namespace std;

ChargeEnemy::ChargeEnemy(float x, float y, Game* game)
	: Enemy(x, y, game) {
	
	lives = 2;
	normalSpeed = 1.0f;
	chargeSpeed = 5.0f;
	detectionRange = 300.0f; // Rango de detección
	isCharging = false;
	patrolDirection = 0; // Empieza moviéndose a la derecha
	patrolTimer = 0;
	chargeDirectionX = 0;
	chargeDirectionY = 0;
	
	// Sprite específico para ChargeEnemy
	texture = game->getTexture("res/AlienCarguero.png");
	aMoving = new Animation("res/AlienCarguero.png", width, height,
		128, 32, 6, 3, game);
	animation = aMoving;
}

void ChargeEnemy::update() {
	// El movimiento se actualiza desde GameLayer
}

void ChargeEnemy::patrolBorder() {
	// Patrullar en forma rectangular por el borde
	patrolTimer++;
	
	// Cambiar dirección cada cierto tiempo
	if (patrolTimer >= 60) { // Cambiar cada 2 segundos aprox
		patrolDirection = (patrolDirection + 1) % 4;
		patrolTimer = 0;
	}
	
	// Establecer velocidad según dirección
	switch (patrolDirection) {
		case 0: // Derecha
			vx = normalSpeed;
			vy = 0;
			break;
		case 1: // Abajo
			vx = 0;
			vy = normalSpeed;
			break;
		case 2: // Izquierda
			vx = -normalSpeed;
			vy = 0;
			break;
		case 3: // Arriba
			vx = 0;
			vy = -normalSpeed;
			break;
	}
}

void ChargeEnemy::checkAndCharge(Player* player) {
	if (player == nullptr) return;
	
	// Calcular distancia al jugador
	float dx = player->x - x;
	float dy = player->y - y;
	float distance = sqrt(dx * dx + dy * dy);
	
	// Si está fuera de rango, patrullar
	if (distance > detectionRange) {
		isCharging = false;
		patrolBorder();
		return;
	}
	
	// Verificar si está alineado en el eje X o Y (con cierto margen)
	float alignmentMargin = 50.0f;
	bool alignedX = fabs(dy) < alignmentMargin;
	bool alignedY = fabs(dx) < alignmentMargin;
	
	if ((alignedX || alignedY) && !isCharging) {
		// Iniciar embestida
		isCharging = true;
		
		if (alignedX) {
			// Embestir horizontalmente
			chargeDirectionX = (dx > 0) ? chargeSpeed : -chargeSpeed;
			chargeDirectionY = 0;
			cout << "¡ChargeEnemy embiste horizontalmente!" << endl;
		} else if (alignedY) {
			// Embestir verticalmente
			chargeDirectionX = 0;
			chargeDirectionY = (dy > 0) ? chargeSpeed : -chargeSpeed;
			cout << "¡ChargeEnemy embiste verticalmente!" << endl;
		}
	}
	
	if (isCharging) {
		// Mantener la velocidad de embestida
		vx = chargeDirectionX;
		vy = chargeDirectionY;
		
		// Detener embestida si se aleja mucho o choca
		if (distance > detectionRange * 1.5f) {
			isCharging = false;
		}
	} else {
		// Si no está embestiendo, patrullar
		patrolBorder();
	}
}
