#include "ShotgunWeapon.h"
#include "Player.h"
#include "Space.h"
#include <cmath>
#include <iostream>

ShotgunWeapon::ShotgunWeapon(Game* game) : Weapon(WeaponType::SHOTGUN, game) {
	cooldown = 40;
	currentCooldown = 0;
	ammo = 50;
	maxAmmo = 50;
	damage = 8;
	unlocked = false;
	pelletsPerShot = 3;
	spreadAngle = 15.0f;
	
	// Audio específico para esta arma - puede ser diferente
	audioFire = Audio::createAudio("res/SonidoEscopeta.wav", false); // Puedes cambiar por "res/shotgun_fire.wav"
	audioFireCooldown = 0;
	
	std::cout << "ShotgunWeapon creada" << std::endl;
}

void ShotgunWeapon::fire(Player* player, float targetX, float targetY) {
	if (!canFire() || projectileList == nullptr || space == nullptr) {
		return;
	}
	
	currentCooldown = cooldown;
	ammo--;
	
	float dx = targetX - player->x;
	float dy = targetY - player->y;
	float baseAngle = atan2(dy, dx);
	
	// ¡IMPORTANTE! Un solo sonido para toda la ráfaga de proyectiles
	if (audioFireCooldown <= 0) {
		audioFire->play();
		audioFireCooldown = 15; // Cooldown más largo para escopeta
	}
	
	std::cout << "Disparando " << pelletsPerShot << " proyectiles de escopeta" << std::endl;
	
	for (int i = 0; i < pelletsPerShot; i++) {
		float offset = (i - 1) * (spreadAngle * 3.14159f / 180.0f);
		float angle = baseAngle + offset;
		
	float newTargetX = player->x + cos(angle) * 500.0f;
		float newTargetY = player->y + sin(angle) * 500.0f;
		
		Projectile* projectile = new Projectile(newTargetX, newTargetY, player->x, player->y, game);
		projectile->damage = player->damage + damage;
		
		projectileList->push_back(projectile);
		space->addDynamicActor(projectile);
	}
}

void ShotgunWeapon::update() {
	if (currentCooldown > 0) {
		currentCooldown--;
	}
	if (audioFireCooldown > 0) {
		audioFireCooldown--;
	}
}

bool ShotgunWeapon::canFire() {
	return currentCooldown == 0 && ammo > 0 && unlocked;
}

std::string ShotgunWeapon::getName() {
	return "Escopeta (" + std::to_string(ammo) + ")";  // Mostrar munición restante
}

std::string ShotgunWeapon::getIconPath() {
	return "res/ShotGunShot.png"; // Usar icono temporal
}