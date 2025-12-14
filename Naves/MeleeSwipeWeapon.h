#pragma once
#include "Weapon.h"
#include "Actor.h"

class MeleeSwipeWeapon : public Weapon {
public:
	MeleeSwipeWeapon(Game* game);
	~MeleeSwipeWeapon();
	
	void fire(Player* player, float targetX, float targetY) override;
	void update() override;
	bool canFire() override;
	std::string getName() override { return "Barrido Melee"; }
	std::string getIconPath() override { return "res/melee_icon.png"; }
	
	Actor* swipeActor;
	bool isActive;
	int activeTime;
	int maxActiveTime;
	float startAngle;
	float endAngle;
	float currentAngle;
	float playerX;
	float playerY;
	float knockbackForce;
};
