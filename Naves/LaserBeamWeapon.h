#pragma once
#include "Weapon.h"
#include "Actor.h"

class LaserBeamWeapon : public Weapon {
public:
	LaserBeamWeapon(Game* game);
	~LaserBeamWeapon();
	
	void fire(Player* player, float targetX, float targetY) override;
	void update() override;
	bool canFire() override;
	std::string getName() override { return "Rayo Laser"; }
	std::string getIconPath() override { return "res/laser_icon.png"; }
	
	Actor* beamActor;
	bool isActive;
	int activeTime;
	int maxActiveTime;
	float startX;
	float startY;
	float endX;
	float endY;
	int beamDamage;
};
