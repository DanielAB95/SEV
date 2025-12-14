#include "SimpleShotWeapon.h"
#include "Player.h"
#include "Space.h"
#include <iostream>

SimpleShotWeapon::SimpleShotWeapon(Game* game) : Weapon(WeaponType::SIMPLE_SHOT, game) {
	cooldown = 22;
	currentCooldown = 0;
	ammo = 999; // Munición infinita para arma básica
	maxAmmo = 999;
	damage = 10;
	unlocked = true;
	
	// Audio específico para esta arma
	audioFire = Audio::createAudio("res/efecto_disparo.wav", false);
	audioFireCooldown = 0;
	
	std::cout << "SimpleShotWeapon creada con cooldown: " << cooldown << std::endl;
}

void SimpleShotWeapon::fire(Player* player, float targetX, float targetY) {
	std::cout << "SimpleShotWeapon::fire() - Verificando condiciones..." << std::endl;
	std::cout << "canFire(): " << canFire() << ", projectileList: " << (projectileList != nullptr) << ", space: " << (space != nullptr) << std::endl;
	
	if (!canFire()) {
		std::cout << "No puede disparar - cooldown: " << currentCooldown << std::endl;
		return;
	}
	
	if (projectileList == nullptr) {
		std::cout << "ERROR: projectileList es null!" << std::endl;
		return;
	}
	
	if (space == nullptr) {
		std::cout << "ERROR: space es null!" << std::endl;
		return;
	}
	
	currentCooldown = cooldown;
	
	// Crear proyectil	
	Projectile* projectile = new Projectile(targetX, targetY, player->x, player->y, game);
	projectile->damage = player->damage + damage;  // Combinar daño del jugador y del arma
	
	// Agregar a la lista de proyectiles de GameLayer
	projectileList->push_back(projectile);
	
	// Agregar al sistema de física
	space->addDynamicActor(projectile);
	
	// Reproducir sonido solo si no está en cooldown
	if (audioFireCooldown <= 0 && audioFire != nullptr) {
		audioFire->play();
		audioFireCooldown = 10; // Cooldown de sonido
	}
	
	std::cout << "¡Proyectil disparado! Posición: (" << player->x << ", " << player->y << ") -> (" << targetX << ", " << targetY << ")" << std::endl;
}

void SimpleShotWeapon::update() {
	if (currentCooldown > 0) {
		currentCooldown--;
	}
	if (audioFireCooldown > 0) {
		audioFireCooldown--;
	}
}

bool SimpleShotWeapon::canFire() {
	return currentCooldown == 0 && unlocked;
}