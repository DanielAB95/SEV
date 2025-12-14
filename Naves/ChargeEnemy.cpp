#include "ChargeEnemy.h"
#include <cmath>
#include <iostream>
using namespace std;

ChargeEnemy::ChargeEnemy(float x, float y, Game* game)
	: Enemy(x, y, game) {
	
	maxLives = 25; // Menos vida pero más peligroso
	lives = 25;
	damage = 15; // Daño alto por embestida
	moveSpeed = 2.0f;
	coinReward = 100; // AÑADIDO: Enemigo cargador da 100 monedas
	normalSpeed = 2.0f;
	chargeSpeed = 3 * moveSpeed;
	detectionRange = 300.0f; // Rango de detección
	isCharging = false;
	patrolDirection = 0; // Se calculará según el borde más cercano
	patrolTimer = 0;
	chargeDirectionX = 0;
	chargeDirectionY = 0;
	
	// NUEVO: Inicializar variables de detección de atascamiento
	lastX = x;
	lastY = y;
	stuckTimer = 0.0f;
	stuckThreshold = 2.0f; // 2 segundos sin moverse = atascado
	stuckCheckFrames = 0;
	
	// NUEVO: Estados para movimiento por bordes
	isMovingToBorder = true; // Al inicio debe ir al borde más cercano
	targetBorderX = 0;
	targetBorderY = 0;
	currentCorner = 0; // Esquina actual (0=TL, 1=TR, 2=BR, 3=BL)
	
	// Definir límites del mapa (basado en GameLayer)
	MAP_MIN_X = 80.0f;
	MAP_MAX_X = 27 * 32 - 80.0f; // 784
	MAP_MIN_Y = 80.0f;
	MAP_MAX_Y = 27 * 32 - 80.0f; // 784
	
	// Calcular borde más cercano y dirección inicial
	findClosestBorderAndDirection();
	
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
	// Si aún se está moviendo al borde inicial
	if (isMovingToBorder) {
		moveToBorder();
		return;
	}
	
	// Patrullar pegado al borde en sentido horario
	patrolClockwise();
}

void ChargeEnemy::moveToBorder() {
	// Calcular dirección hacia el punto objetivo del borde
	float dx = targetBorderX - x;
	float dy = targetBorderY - y;
	float distance = sqrt(dx * dx + dy * dy);
	
	// Si llegamos al borde (dentro de un margen)
	if (distance < 15.0f) {
		isMovingToBorder = false;
		// Posicionarse exactamente en el borde
		x = targetBorderX;
		y = targetBorderY;
		vx = 0;
		vy = 0;
		cout << "ChargeEnemy llegó al borde en esquina " << currentCorner << endl;
		return;
	}
	
	// Normalizar y aplicar velocidad
	if (distance > 0) {
		vx = (dx / distance) * moveSpeed;
		vy = (dy / distance) * moveSpeed;
	}
}

void ChargeEnemy::patrolClockwise() {
	// Definir las 4 esquinas en sentido horario
	// 0=Top-Left, 1=Top-Right, 2=Bottom-Right, 3=Bottom-Left
	float corners[4][2] = {
		{MAP_MIN_X, MAP_MIN_Y},     // 0: Top-Left
		{MAP_MAX_X, MAP_MIN_Y},     // 1: Top-Right
		{MAP_MAX_X, MAP_MAX_Y},     // 2: Bottom-Right
		{MAP_MIN_X, MAP_MAX_Y}      // 3: Bottom-Left
	};
	
	// Calcular siguiente esquina en sentido horario
	int nextCorner = (currentCorner + 1) % 4;
	float targetX = corners[nextCorner][0];
	float targetY = corners[nextCorner][1];
	
	// Calcular dirección hacia la siguiente esquina
	float dx = targetX - x;
	float dy = targetY - y;
	float distance = sqrt(dx * dx + dy * dy);
	
	// Si llegamos a la esquina objetivo
	if (distance < 20.0f) {
		currentCorner = nextCorner;
		x = targetX; // Ajustar posición exacta
		y = targetY;
		cout << "ChargeEnemy llegó a esquina " << currentCorner << endl;
	}
	
	// Aplicar movimiento hacia la siguiente esquina
	if (distance > 0) {
		vx = (dx / distance) * moveSpeed;
		vy = (dy / distance) * moveSpeed;
	}
}

void ChargeEnemy::findClosestBorderAndDirection() {
	// Calcular distancias a cada borde
	float distToLeft = fabs(x - MAP_MIN_X);
	float distToRight = fabs(x - MAP_MAX_X);
	float distToTop = fabs(y - MAP_MIN_Y);
	float distToBottom = fabs(y - MAP_MAX_Y);
	
	// Encontrar el borde más cercano
	float minDist = distToLeft;
	int closestBorder = 0; // 0=izq, 1=der, 2=arriba, 3=abajo
	
	if (distToRight < minDist) { minDist = distToRight; closestBorder = 1; }
	if (distToTop < minDist) { minDist = distToTop; closestBorder = 2; }
	if (distToBottom < minDist) { minDist = distToBottom; closestBorder = 3; }
	
	// Determinar esquina objetivo según el borde más cercano
	switch (closestBorder) {
		case 0: // Borde izquierdo - ir a esquina superior izquierda
			targetBorderX = MAP_MIN_X;
			targetBorderY = MAP_MIN_Y;
			currentCorner = 0; // Top-Left
			break;
		case 1: // Borde derecho - ir a esquina superior derecha
			targetBorderX = MAP_MAX_X;
			targetBorderY = MAP_MIN_Y;
			currentCorner = 1; // Top-Right
			break;
		case 2: // Borde superior - ir a esquina superior izquierda
			targetBorderX = MAP_MIN_X;
			targetBorderY = MAP_MIN_Y;
			currentCorner = 0; // Top-Left
			break;
		case 3: // Borde inferior - ir a esquina inferior izquierda
			targetBorderX = MAP_MIN_X;
			targetBorderY = MAP_MAX_Y;
			currentCorner = 3; // Bottom-Left
			break;
	}
	
	cout << "ChargeEnemy spawneado en (" << x << ", " << y << ") - Borde más cercano: " << closestBorder 
	     << " - Objetivo: (" << targetBorderX << ", " << targetBorderY << ")" << endl;
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
		stuckTimer = 0.0f; // Resetear timer al salir de rango
		stuckCheckFrames = 0; // Resetear contador de frames
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
			float actualChargeSpeed = moveSpeed * 3.0f; // 3x la velocidad normal
			chargeDirectionX = (dx > 0) ? actualChargeSpeed : -actualChargeSpeed;
			chargeDirectionY = 0;
			cout << "¡ChargeEnemy embiste horizontalmente!" << endl;
		} else if (alignedY) {
			// Embestir verticalmente
			float actualChargeSpeed = moveSpeed * 3.0f; // 3x la velocidad normal
			chargeDirectionX = 0;
			chargeDirectionY = (dy > 0) ? actualChargeSpeed : -actualChargeSpeed;
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
			stuckTimer = 0.0f; // Resetear timer
		}
		
		// NUEVO: Verificar si está atascado cada 15 frames (aproximadamente cada 0.5 segundos)
		stuckCheckFrames++;
		if (stuckCheckFrames >= 15) {
			stuckCheckFrames = 0;
			
			// Calcular distancia desde la última verificación
			float moveDistance = sqrt((x - lastX) * (x - lastX) + (y - lastY) * (y - lastY));
			
			// Si se movió menos de 5 píxeles en 0.5 segundos, está atascado
			if (moveDistance < 5.0f) {
				stuckTimer += 0.5f; // Aproximadamente 0.5 segundos
				if (stuckTimer >= stuckThreshold) {
					cout << "ChargeEnemy detectado atascado - Reseteando a patrulla" << endl;
					resetToPatrol();
					return;
				}
			} else {
				// Se está moviendo, resetear timer
				stuckTimer = 0.0f;
			}
			
			// Actualizar última posición conocida
			lastX = x;
			lastY = y;
		}
	} else {
		// Si no está embestiendo, patrullar
		patrolBorder();
	}
}

void ChargeEnemy::resetToPatrol() {
	// Resetear todas las variables de embestida
	isCharging = false;
	chargeDirectionX = 0;
	chargeDirectionY = 0;
	stuckTimer = 0.0f;
	stuckCheckFrames = 0;
	vx = 0;
	vy = 0;
	
	// Buscar el borde más cercano y resetear patrullaje
	findClosestBorderAndDirection();
	isMovingToBorder = true; // Forzar que vaya al borde más cercano
	
	cout << "ChargeEnemy reseteado - volviendo al modo patrulla hacia borde más cercano" << endl;
}