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
	
	// NUEVO: Métodos para el movimiento mejorado por bordes
	void moveToBorder(); // Moverse hacia el borde más cercano
	void patrolClockwise(); // Patrullar en sentido horario
	void findClosestBorderAndDirection(); // Calcular borde más cercano
	void resetToPatrol(); // Resetear a modo patrulla cuando se atasca
	
	float normalSpeed;
	float chargeSpeed;
	float detectionRange;
	bool isCharging;
	float chargeDirectionX;
	float chargeDirectionY;
	int patrolDirection; // 0=derecha, 1=abajo, 2=izquierda, 3=arriba
	float patrolTimer;
	
	// NUEVO: Variables para movimiento por bordes
	bool isMovingToBorder; // Si está yendo al borde inicial
	float targetBorderX, targetBorderY; // Punto objetivo en el borde
	int currentCorner; // Esquina actual (0=TL, 1=TR, 2=BR, 3=BL)
	float MAP_MIN_X, MAP_MAX_X, MAP_MIN_Y, MAP_MAX_Y; // Límites del mapa
	
	// NUEVO: Variables para detectar cuando se queda atascado
	float lastX, lastY; // Posición anterior para detectar movimiento
	float stuckTimer; // Tiempo que lleva sin moverse
	float stuckThreshold; // Tiempo máximo antes de resetear (en segundos)
	int stuckCheckFrames; // Contador de frames para verificar cada cierto tiempo
};