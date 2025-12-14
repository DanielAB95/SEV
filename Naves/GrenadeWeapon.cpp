#include "GrenadeWeapon.h"
#include "Player.h"
#include <cmath>

GrenadeProjectile::GrenadeProjectile(float targetX, float targetY, float startX, float startY, Game* game)
	: Actor("res/grenade.png", startX, startY, 20, 20, game) {
	
	float dx = targetX - startX;
	float dy = targetY - startY;
	float distance = sqrt(dx * dx + dy * dy);
	
	float time = distance / 8.0f;
	
	vx = dx / time;
	vy = (dy / time) - (0.5f * 0.5f * time);
	
	gravity = 0.5f;
	explosionRadius = 80;
	damage = 30;
	exploded = false;
	explosionTime = 0;
	maxExplosionTime = 10;
}

void GrenadeProjectile::update() {
	if (!exploded) {
		x += vx;
		y += vy;
		vy += gravity;
	} else {
		explosionTime++;
	}
}

bool GrenadeProjectile::hasExploded() {
	return exploded && explosionTime >= maxExplosionTime;
}

GrenadeWeapon::GrenadeWeapon(Game* game) : Weapon(WeaponType::GRENADE, game) {
	cooldown = 60;
	currentCooldown = 0;
	ammo = 10;
	maxAmmo = 999;
	damage = 30;
	unlocked = false;
	explosionRadius = 80;
}

void GrenadeWeapon::fire(Player* player, float targetX, float targetY) {
	if (!canFire()) return;
	
	currentCooldown = cooldown;
	ammo--;
	
	GrenadeProjectile* grenade = new GrenadeProjectile(targetX, targetY, player->x, player->y, game);
	grenade->damage = player->damage + damage;
	activeGrenades.push_back(grenade);
}

void GrenadeWeapon::update() {
	if (currentCooldown > 0) {
		currentCooldown--;
	}
	
	for (auto it = activeGrenades.begin(); it != activeGrenades.end(); ) {
		(*it)->update();
		
		if ((*it)->hasExploded()) {
			delete (*it);
			it = activeGrenades.erase(it);
		} else {
			++it;
		}
	}
}

bool GrenadeWeapon::canFire() {
	return currentCooldown == 0 && ammo > 0;
}

std::string GrenadeWeapon::getName() {
	return "Granada";
}

std::string GrenadeWeapon::getIconPath() {
	return "res/recolectable.png";
}
