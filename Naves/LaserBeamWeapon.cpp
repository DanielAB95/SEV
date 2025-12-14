#include "LaserBeamWeapon.h"
#include "Player.h"
#include "Enemy.h"
#include <cmath>
#include <iostream>

LaserBeamWeapon::LaserBeamWeapon(Game* game) : Weapon(WeaponType::LASER_BEAM, game) {
	cooldown = 90;
	currentCooldown = 0;
	ammo = 10;
	maxAmmo = 10;
	damage = 30;
	unlocked = false; // Desbloqueado para testing
	beamWidth = 20.0f; // Ancho del rayo para detección de colisiones
	
	beamActor = new Actor("res/LaserShot.png", 0, 0, 800, 8, game);
	isActive = false;
	activeTime = 0;
	maxActiveTime = 20;
	beamDamage = 15;
	
	std::cout << "LaserBeamWeapon creada y desbloqueada" << std::endl;
}

LaserBeamWeapon::~LaserBeamWeapon() {
	delete beamActor;
}

void LaserBeamWeapon::fire(Player* player, float targetX, float targetY) {
	if (!canFire()) return;
	
	currentCooldown = cooldown;
	ammo--;
	isActive = true;
	activeTime = 0;
	
	startX = player->x;
	startY = player->y;
	
	// Calcular dirección del rayo
	float dx = targetX - startX;
	float dy = targetY - startY;
	float length = sqrt(dx * dx + dy * dy);
	
	// El rayo va hasta 600px en esa dirección (más razonable que 1000)
	float laserRange = 600.0f;
	if (length > 0) {
		endX = startX + (dx / length) * laserRange;
		endY = startY + (dy / length) * laserRange;
	} else {
		endX = startX + laserRange;
		endY = startY;
	}
	
	// Configurar el actor para que represente el láser
	// Calcular longitud y posición del centro
	float laserLength = sqrt((endX - startX) * (endX - startX) + (endY - startY) * (endY - startY));
	beamActor->width = (int)laserLength;
	beamActor->height = 8; // Grosor del rayo visual
	
	// Posicionar en el centro del rayo
	beamActor->x = (startX + endX) / 2;
	beamActor->y = (startY + endY) / 2;
	
	std::cout << "Láser disparado desde (" << startX << ", " << startY << ") hasta (" 
	          << endX << ", " << endY << ") - Longitud: " << laserLength << std::endl;
}

void LaserBeamWeapon::update() {
	if (currentCooldown > 0) {
		currentCooldown--;
	}
	
	if (isActive) {
		activeTime++;
		if (activeTime >= maxActiveTime) {
			isActive = false;
			activeTime = 0;
		}
	}
}

bool LaserBeamWeapon::canFire() {
	return currentCooldown == 0 && ammo > 0 && !isActive;
}

void LaserBeamWeapon::draw(float scrollX, float scrollY) {
	// Solo dibujar si el láser está activo
	if (isActive && beamActor != nullptr) {
		// Dibujar una línea simple usando múltiples rectángulos pequeños
		float dx = endX - startX;
		float dy = endY - startY;
		float distance = sqrt(dx * dx + dy * dy);
		
		if (distance > 0) {
			int segments = (int)(distance / 10); // Segmentos cada 10 píxeles
			if (segments < 1) segments = 1;
			
			float stepX = dx / segments;
			float stepY = dy / segments;
			
			// Crear múltiples pequeños actores para simular una línea
			for (int i = 0; i < segments; i++) {
				float x = startX + (stepX * i);
				float y = startY + (stepY * i);
				
				// Crear un pequeño actor temporal para cada segmento
				Actor segmentActor("res/LaserShot.png", x, y, 36, 12, game);
				segmentActor.draw(scrollX, scrollY);
			}
		}
	}
}

std::string LaserBeamWeapon::getName() {
	return "Rayo Laser (" + std::to_string(ammo) + ")";
}

void LaserBeamWeapon::checkBeamCollisions(std::list<Enemy*>* enemies) {
	if (!isActive || enemies == nullptr) return;
	
	std::cout << "Verificando colisiones rayo láser. Enemigos: " << enemies->size() << std::endl;
	
	// Verificar colisión con cada enemigo usando distancia punto-línea
	for (std::list<Enemy*>::iterator enemyIt = enemies->begin(); enemyIt != enemies->end(); ++enemyIt) {
		Enemy* enemy = *enemyIt;
		
		// Calcular la distancia del enemigo a la línea del láser
		float distanceToBeam = distancePointToLine(enemy->x, enemy->y, startX, startY, endX, endY);
		
		if (distanceToBeam <= beamWidth / 2.0f) {
			// Verificar que esté dentro del rango del láser
			float enemyDistance = sqrt((enemy->x - startX) * (enemy->x - startX) + 
			                          (enemy->y - startY) * (enemy->y - startY));
			float beamLength = sqrt((endX - startX) * (endX - startX) + 
			                       (endY - startY) * (endY - startY));
			
			if (enemyDistance <= beamLength) {
				std::cout << "COLISIÓN LÁSER! Enemigo antes: " << enemy->lives << " vidas" << std::endl;
				
				// Aplicar daño del láser (alto daño por ser láser)
				int totalDamage = beamDamage + 20; // Daño significativo
				enemy->lives -= totalDamage;
				
				std::cout << "Láser daño aplicado: " << totalDamage << ", Enemigo después: " << enemy->lives << " vidas" << std::endl;
				
				std::cout << "¡Rayo láser atravesó enemigo! Daño: " << totalDamage 
				          << ", Vidas restantes: " << enemy->lives 
				          << ", Distancia a línea: " << distanceToBeam << std::endl;
			}
		}
	}
}

float LaserBeamWeapon::distancePointToLine(float px, float py, float x1, float y1, float x2, float y2) {
	// Calcular la distancia de un punto a una línea usando la fórmula estándar
	float dx = x2 - x1;
	float dy = y2 - y1;
	
	if (dx == 0 && dy == 0) {
		// La línea es un punto
		return sqrt((px - x1) * (px - x1) + (py - y1) * (py - y1));
	}
	
	// Usar la fórmula de distancia punto-línea
	float t = ((px - x1) * dx + (py - y1) * dy) / (dx * dx + dy * dy);
	t = fmax(0, fmin(1, t)); // Clamp t entre 0 y 1
	
	float projX = x1 + t * dx;
	float projY = y1 + t * dy;
	
	return sqrt((px - projX) * (px - projX) + (py - projY) * (py - projY));
}