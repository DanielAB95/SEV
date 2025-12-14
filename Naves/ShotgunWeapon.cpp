#include "ShotgunWeapon.h"
#include "Player.h"
#include "Space.h"
#include <cmath>

ShotgunWeapon::ShotgunWeapon(Game* game) : Weapon(WeaponType::SHOTGUN, game) {
	cooldown = 40;
	currentCooldown = 0;
	ammo = 15;
	maxAmmo = 999;
	damage = 8;
	unlocked = false;
	projectileList = nullptr;
	pelletsPerShot = 3;
	spreadAngle = 15.0f;
}

void ShotgunWeapon::fire(Player* player, float targetX, float targetY) {
	if (!canFire() || projectileList == nullptr || space == nullptr) return;
	
	currentCooldown = cooldown;
	ammo--;
	
	float dx = targetX - player->x;
	float dy = targetY - player->y;
	float baseAngle = atan2(dy, dx);
	
	for (int i = 0; i < pelletsPerShot; i++) {
		float offset = (i - 1) * (spreadAngle * 3.14159f / 180.0f);
		float angle = baseAngle + offset;
		
		float newTargetX = player->x + cos(angle) * 500.0f;
		float newTargetY = player->y + sin(angle) * 500.0f;
		
		Projectile* projectile = new Projectile(newTargetX, newTargetY, player->x, player->y, game);
		projectile->damage = player->damage + damage;
		
		if (projectileList != nullptr) {
			projectileList->push_back(projectile);
		}
	}
}

void ShotgunWeapon::update() {
	if (currentCooldown > 0) {
		currentCooldown--;
	}
}

bool ShotgunWeapon::canFire() {
	return currentCooldown == 0 && ammo > 0;
}

std::string ShotgunWeapon::getName() {
	return "Escopeta";
}

std::string ShotgunWeapon::getIconPath() {
	return "res/disparo_jugador.png"; // Usar icono temporal
}
