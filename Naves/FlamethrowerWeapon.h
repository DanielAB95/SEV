#pragma once
#include "Weapon.h"
#include "Actor.h"
#include <list>

class Enemy;

class FlamethrowerWeapon : public Weapon {
public:
	FlamethrowerWeapon(Game* game);
	~FlamethrowerWeapon();
	
	void fire(Player* player, float targetX, float targetY) override;
	void update() override;
	bool canFire() override;
	std::string getName() override { return "Lanzallamas"; }
	std::string getIconPath() override { return "res/flamethrower_icon.png"; }
	
	Actor* flameActor;
	bool isFiring;
	int fuelConsumption;
	float playerX;
	float playerY;
	float targetX;
	float targetY;
	float coneLength;
	float coneWidth;
	int damagePerFrame;
};
