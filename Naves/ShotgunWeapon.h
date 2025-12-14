#pragma once
#include "Weapon.h"
#include "Projectile.h"
#include <list>

class ShotgunWeapon : public Weapon {
public:
	ShotgunWeapon(Game* game);
	
	void fire(Player* player, float targetX, float targetY) override;
	void update() override;
	bool canFire() override;
	std::string getName() override;
	std::string getIconPath() override;
	
	int pelletsPerShot;
	float spreadAngle;
};
