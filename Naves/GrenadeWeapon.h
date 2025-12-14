#pragma once
#include "Weapon.h"
#include "Actor.h"
#include <list>

class GrenadeProjectile : public Actor {
public:
	GrenadeProjectile(float targetX, float targetY, float startX, float startY, Game* game);
	
	void update();
	bool hasExploded();
	
	float vx, vy;
	float gravity;
	int explosionRadius;
	int damage;
	bool exploded;
	int explosionTime;
	int maxExplosionTime;
};

class GrenadeWeapon : public Weapon {
public:
	GrenadeWeapon(Game* game);
	
	void fire(Player* player, float targetX, float targetY) override;
	void update() override;
	bool canFire() override;
	std::string getName() override;
	std::string getIconPath() override;
	
	std::list<GrenadeProjectile*> activeGrenades;
	int explosionRadius;
};
