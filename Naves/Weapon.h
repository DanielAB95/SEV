#pragma once
#include "Game.h"
#include "Actor.h"
#include "Audio.h"
#include <string>
#include <list>

class Enemy;
class Player;
class Projectile;
class Space;

enum class WeaponType {
	SIMPLE_SHOT,
	MELEE_SWIPE,
	SHOTGUN,
	FLAMETHROWER,
	GRENADE,
	LASER_BEAM
};

class Weapon {
public:
	Weapon(WeaponType type, Game* game);
	virtual ~Weapon() {}

	virtual void fire(Player* player, float targetX, float targetY) = 0;
	virtual void update() = 0;
	virtual bool canFire() = 0;
	virtual std::string getName() = 0;
	virtual std::string getIconPath() = 0;

	// Método para configurar las referencias necesarias para crear proyectiles
	void setProjectileReferences(std::list<Projectile*>* projectileList, Space* space);

	WeaponType type;
	Game* game;

	int cooldown;
	int currentCooldown;
	int ammo;
	int maxAmmo;
	int damage;
	bool unlocked;
	
	// Audio para cada arma
	Audio* audioFire;
	int audioFireCooldown;

	void reload();
	void unlock();

protected:
	// Referencias compartidas para crear y gestionar proyectiles
	std::list<Projectile*>* projectileList;
	Space* space;
};