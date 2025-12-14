#include "Weapon.h"
#include "Projectile.h"
#include "Space.h"

Weapon::Weapon(WeaponType type, Game* game) {
	this->type = type;
	this->game = game;
	this->cooldown = 0;
	this->currentCooldown = 0;
	this->ammo = 0;
	this->maxAmmo = 0;
	this->damage = 0;
	this->unlocked = false;
	this->projectileList = nullptr;
	this->space = nullptr;
}

void Weapon::setProjectileReferences(std::list<Projectile*>* projectileList, Space* space) {
	this->projectileList = projectileList;
	this->space = space;
}
