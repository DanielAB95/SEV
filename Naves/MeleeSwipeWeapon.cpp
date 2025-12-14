#include "MeleeSwipeWeapon.h"
#include "Player.h"
#include "Enemy.h"
#include <cmath>

MeleeSwipeWeapon::MeleeSwipeWeapon(Game* game) : Weapon(WeaponType::MELEE_SWIPE, game) {
	cooldown = 30;
	currentCooldown = 0;
	ammo = 999;
	maxAmmo = 999;
	damage = 25;
	unlocked = false;
	
	swipeActor = new Actor("res/melee_swipe.png", 0, 0, 80, 80, game);
	isActive = false;
	activeTime = 0;
	maxActiveTime = 15;
	startAngle = -45.0f;
	endAngle = 45.0f;
	currentAngle = 0;
	knockbackForce = 100.0f;
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
	
	float dx = targetX - playerX;
	float dy = targetY - playerY;
	float baseAngle = atan2(dy, dx) * 180.0f / 3.14159f;
	startAngle = baseAngle - 45.0f;
	endAngle = baseAngle + 45.0f;
}

void MeleeSwipeWeapon::update() {
	if (currentCooldown > 0) {
		currentCooldown--;
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
		}
	}
}

bool MeleeSwipeWeapon::canFire() {
	return currentCooldown == 0 && !isActive;
}
