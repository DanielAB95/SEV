#include "FlamethrowerWeapon.h"
#include "Player.h"
#include "Space.h"
#include <cmath>

FlamethrowerWeapon::FlamethrowerWeapon(Game* game) : Weapon(WeaponType::FLAMETHROWER, game) {
	cooldown = 0;
	currentCooldown = 0;
	ammo = 200;
	maxAmmo = 999;
	damage = 2;
	unlocked = false;
	
	flameActor = new Actor("res/flame.png", 0, 0, 100, 60, game);
	isFiring = false;
	fuelConsumption = 1;
	coneLength = 150.0f;
	coneWidth = 80.0f;
	damagePerFrame = 2;
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
	
	float dx = targetX - playerX;
	float dy = targetY - playerY;
	float angle = atan2(dy, dx);
	
	flameActor->x = playerX + cos(angle) * coneLength / 2;
	flameActor->y = playerY + sin(angle) * coneLength / 2;
}

void FlamethrowerWeapon::update() {
	if (ammo <= 0) {
		isFiring = false;
	}
}

bool FlamethrowerWeapon::canFire() {
	return ammo > 0;
}
