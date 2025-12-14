#include "MeleeSwipeWeapon.h"
#include "Player.h"
#include "Enemy.h"
#include <cmath>
#include <iostream>

MeleeSwipeWeapon::MeleeSwipeWeapon(Game* game) : Weapon(WeaponType::MELEE_SWIPE, game) {
	cooldown = 30;
	currentCooldown = 0;
	ammo = 999;
	maxAmmo = 999;
	damage = 15;
	unlocked = true; // Ya está desbloqueada para testing
	
	swipeActor = new Actor("res/Sword.png", 0, 0, 80, 80, game);
	isActive = false;
	activeTime = 0;
	maxActiveTime = 15;
	startAngle = -45.0f;
	endAngle = 45.0f;
	currentAngle = 0;
	knockbackForce = 50.0f;
	
	// Audio específico para arma melé
	audioFire = Audio::createAudio("res/SonidoEspada.wav", false); // Puedes usar cualquier sonido temporal
	audioFireCooldown = 0;
	
	std::cout << "MeleeSwipeWeapon creada y desbloqueada" << std::endl;
}

MeleeSwipeWeapon::~MeleeSwipeWeapon() {
	delete swipeActor;
}

void MeleeSwipeWeapon::fire(Player* player, float targetX, float targetY) {
	if (!canFire()) return;
	
	currentCooldown = cooldown;
	isActive = true;
	activeTime = 0;
	playerX = player->x;
	playerY = player->y;
	currentAngle = startAngle;
	
	// Limpiar lista de enemigos ya golpeados
	alreadyHit.clear();
	
	float dx = targetX - playerX;
	float dy = targetY - playerY;
	float baseAngle = atan2(dy, dx) * 180.0f / 3.14159f;
	startAngle = baseAngle - 45.0f;
	endAngle = baseAngle + 45.0f;
	
	// Reproducir sonido
	if (audioFireCooldown <= 0 && audioFire != nullptr) {
		audioFire->play();
		audioFireCooldown = 10;
	}
	
	std::cout << "¡Arma melé activada! Barrido de " << startAngle << " a " << endAngle << " grados" << std::endl;
	std::cout << "Posición jugador: (" << playerX << ", " << playerY << ")" << std::endl;
	std::cout << "Arma configurada con damage = " << damage << std::endl;
}

void MeleeSwipeWeapon::update() {
	if (currentCooldown > 0) {
		currentCooldown--;
	}
	
	if (audioFireCooldown > 0) {
		audioFireCooldown--;
	}
	
	if (isActive) {
		activeTime++;
		
		float progress = (float)activeTime / (float)maxActiveTime;
		currentAngle = startAngle + (endAngle - startAngle) * progress;
		
		float angleRad = currentAngle * 3.14159f / 180.0f;
		float distance = 50.0f;
		swipeActor->x = playerX + cos(angleRad) * distance;
		swipeActor->y = playerY + sin(angleRad) * distance;
		
		if (activeTime >= maxActiveTime) {
			isActive = false;
			activeTime = 0;
			alreadyHit.clear(); // Limpiar lista al finalizar
		}
	}
}

bool MeleeSwipeWeapon::canFire() {
	return currentCooldown == 0 && !isActive && unlocked;
}

void MeleeSwipeWeapon::draw(float scrollX, float scrollY) {
	if (isActive && swipeActor != nullptr) {
		swipeActor->draw(scrollX, scrollY);
	}
}

void MeleeSwipeWeapon::checkEnemyCollisions(std::list<Enemy*>* enemies) {
	if (!isActive || enemies == nullptr) return;
	
	for (auto const& enemy : *enemies) {
		// Verificar si ya golpeamos a este enemigo en este ataque
		bool alreadyHitThisEnemy = false;
		for (auto const& hitEnemy : alreadyHit) {
			if (hitEnemy == enemy) {
				alreadyHitThisEnemy = true;
				break;
			}
		}
		
		if (alreadyHitThisEnemy) continue;
		
		// Verificar colisión con el arma
		if (swipeActor->isOverlap(enemy)) {
			std::cout << "COLISIÓN DETECTADA! Enemigo antes: " << enemy->lives << " vidas" << std::endl;
			
			// Aplicar daño (arma + daño del jugador)
			int totalDamage = 20; // Daño balanceado para eliminar enemigo en 2 golpes aprox
			enemy->lives -= totalDamage;
			
			std::cout << "Daño aplicado: " << totalDamage << ", Enemigo después: " << enemy->lives << " vidas" << std::endl;
			
			// Aplicar knockback
			float dx = enemy->x - playerX;
			float dy = enemy->y - playerY;
			float distance = sqrt(dx * dx + dy * dy);
			
			if (distance > 0) { // Evitar división por cero
				float knockbackX = (dx / distance) * knockbackForce;
				float knockbackY = (dy / distance) * knockbackForce;
				
				enemy->x += knockbackX;
				enemy->y += knockbackY;
			}
			
			// Marcar como golpeado en este ataque
			alreadyHit.push_back(enemy);
			
			std::cout << "¡Arma melé golpeó a enemigo! Daño: " << totalDamage 
			          << ", Vidas restantes: " << enemy->lives 
			          << ", Posición enemigo: (" << enemy->x << ", " << enemy->y << ")"
			          << ", Posición arma: (" << swipeActor->x << ", " << swipeActor->y << ")" << std::endl;
		}
	}
}
