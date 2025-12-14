#pragma once
#include "Weapon.h"
#include "Projectile.h"
#include <list>

class SimpleShotWeapon : public Weapon {
public:
	SimpleShotWeapon(Game* game);

	void fire(Player* player, float targetX, float targetY) override;
	void update() override;
	bool canFire() override;
	std::string getName() override { return "Disparo Simple"; }
	std::string getIconPath() override { return "res/NormalShot.png"; }

};
