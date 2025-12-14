#include "FlamethrowerWeapon.h"
#include "Player.h"
#include "Enemy.h"
#include "Space.h"
#include <cmath>
#include <iostream>

FlamethrowerWeapon::FlamethrowerWeapon(Game* game) : Weapon(WeaponType::FLAMETHROWER, game) {
	cooldown = 0;
	currentCooldown = 0;
	ammo = 300;
	maxAmmo = 300;
	damage = 1;
	unlocked = false; // Desbloqueado para testing
	
	flameActor = new Actor("res/Flames.png", 0, 0, 100, 60, game);
	isFiring = false;
	fuelConsumption = 1;
	coneLength = 150.0f;
	coneWidth = 80.0f;
	damagePerFrame = 1;
	damageTimer = 0;
	damageInterval = 8; // Daño cada 8 frames (50% más lento que antes)
	
	// Audio específico para el lanzallamas
	audioFire = Audio::createAudio("res/flamethrower.wav", false);
	audioFireCooldown = 0;
	
	std::cout << "FlamethrowerWeapon creada y desbloqueada" << std::endl;
}

FlamethrowerWeapon::~FlamethrowerWeapon() {
	delete flameActor;
}

void FlamethrowerWeapon::fire(Player* player, float targetX, float targetY) {
	if (!canFire()) return;
	
	ammo -= fuelConsumption;
	if (ammo < 0) ammo = 0;
	
	isFiring = true;
	playerX = player->x;
	playerY = player->y;
	this->targetX = targetX;
	this->targetY = targetY;
	
	std::cout << "Lanzallamas disparando - ammo restante: " << ammo << ", isFiring: " << isFiring << std::endl;
	
	float dx = targetX - playerX;
	float dy = targetY - playerY;
	float angle = atan2(dy, dx);
	
	flameActor->x = playerX + cos(angle) * coneLength / 2;
	flameActor->y = playerY + sin(angle) * coneLength / 2;
}

void FlamethrowerWeapon::update() {
	if (audioFireCooldown > 0) {
		audioFireCooldown--;
	}
	
	if (ammo <= 0) {
		std::cout << "Lanzallamas sin combustible, desactivando..." << std::endl;
		isFiring = false;
	}
	
	// Actualizar timer de daño
	if (isFiring) {
		damageTimer++;
		if (damageTimer >= damageInterval) {
			damageTimer = 0;
		}
	} else {
		damageTimer = 0;
	}
}

bool FlamethrowerWeapon::canFire() {
	return ammo > 0 && unlocked;
}

std::string FlamethrowerWeapon::getName() {
	return "Lanzallamas (" + std::to_string(ammo) + ")";  // Mostrar combustible restante
}

void FlamethrowerWeapon::draw(float scrollX, float scrollY) {
	// Solo dibujar si está disparando Y tiene combustible
	if (isFiring && flameActor != nullptr && ammo > 0) {
		flameActor->draw(scrollX, scrollY);
	}
}

void FlamethrowerWeapon::checkEnemyCollisions(std::list<Enemy*>* enemies) {
if (!isFiring || enemies == nullptr) return;
	
std::cout << "Verificando colisiones lanzallamas. Enemigos: " << enemies->size() 
		<< ", damageTimer: " << damageTimer << ", damageInterval: " << damageInterval << std::endl;
	
	for (auto const& enemy : *enemies) {
		// Verificar si el enemigo está dentro del cono de la llama
		float dx = enemy->x - playerX;
		float dy = enemy->y - playerY;
		float distance = sqrt(dx * dx + dy * dy);
		
		// Verificar distancia
		if (distance > coneLength) continue;
		
		// Verificar ángulo del cono
		float enemyAngle = atan2(dy, dx);
		float targetAngle = atan2(targetY - playerY, targetX - playerX);
		float angleDiff = abs(enemyAngle - targetAngle);
		
		// Normalizar diferencia de ángulo
		if (angleDiff > 3.14159f) angleDiff = 2 * 3.14159f - angleDiff;
		
		// Verificar si está dentro del cono (ángulo de apertura de ~45 grados)
		if (angleDiff <= 0.39f) { // ~45 grados en radianes
			std::cout << "COLISIÓN LANZALLAMAS! Enemigo antes: " << enemy->lives << " vidas" << std::endl;
			
			// Aplicar daño por frame (arma + daño del jugador)
			int totalDamage = 1; // Daño continuo balanceado
			enemy->lives -= totalDamage;
			
			std::cout << "Lanzallamas daño aplicado: " << totalDamage << ", Enemigo después: " << enemy->lives << " vidas" << std::endl;
			
			std::cout << "¡Lanzallamas quemando enemigo! Daño: " << totalDamage 
			          << ", Vidas restantes: " << enemy->lives 
			          << ", Distancia: " << distance
			          << ", Ángulo diff: " << angleDiff << std::endl;
		}
	}
}
