#include "SimpleShotWeapon.h"
#include "Player.h"
#include "Space.h"

SimpleShotWeapon::SimpleShotWeapon(Game* game) : Weapon(WeaponType::SIMPLE_SHOT, game) {
	cooldown = 22;
	currentCooldown = 0;
	ammo = 999; // Munición infinita para arma básica
	maxAmmo = 999;
	damage = 10;
	unlocked = true;
}

void SimpleShotWeapon::fire(Player* player, float targetX, float targetY) {
	if (!canFire() || projectileList == nullptr || space == nullptr) return;
	
	currentCooldown = cooldown;
	
	// Crear proyectil
	Projectile* projectile = new Projectile(targetX, targetY, player->x, player->y, game);
	projectile->damage = player->damage;
	
	// Agregar a la lista de proyectiles de GameLayer
	projectileList->push_back(projectile);
	
	// Agregar al sistema de física
	space->addDynamicActor(projectile);
}

void SimpleShotWeapon::update() {
	if (currentCooldown > 0) {
		currentCooldown--;
	}
}

bool SimpleShotWeapon::canFire() {
	return currentCooldown == 0;
}