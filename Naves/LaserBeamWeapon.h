#pragma once
#include "Weapon.h"
#include "Actor.h"
#include <list>

class Enemy;  // Forward declaration

class LaserBeamWeapon : public Weapon {
public:
	LaserBeamWeapon(Game* game);
	~LaserBeamWeapon();
	
	void fire(Player* player, float targetX, float targetY) override;
	void update() override;
	bool canFire() override;
	std::string getName() override;
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
	float beamWidth;  // Ancho del rayo para colisiones
	
	// Métodos adicionales para láser
	void draw(float scrollX = 0, float scrollY = 0);
	void checkBeamCollisions(std::list<Enemy*>* enemies);
	float distancePointToLine(float px, float py, float x1, float y1, float x2, float y2);
};
