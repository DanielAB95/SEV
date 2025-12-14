#include "LaserBeamWeapon.h"
#include "Player.h"
#include <cmath>

LaserBeamWeapon::LaserBeamWeapon(Game* game) : Weapon(WeaponType::LASER_BEAM, game) {
	cooldown = 90;
	currentCooldown = 0;
	ammo = 5;
	maxAmmo = 999;
	damage = 15;
	unlocked = false;
	
	beamActor = new Actor("res/laser_beam.png", 0, 0, 800, 10, game);
	isActive = false;
	activeTime = 0;
	maxActiveTime = 20;
	beamDamage = 15;
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
	
	float dx = targetX - startX;
	float dy = targetY - startY;
	float length = sqrt(dx * dx + dy * dy);
	
	endX = startX + (dx / length) * 1000.0f;
	endY = startY + (dy / length) * 1000.0f;
	
	beamActor->x = (startX + endX) / 2;
	beamActor->y = (startY + endY) / 2;
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
