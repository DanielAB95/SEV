#pragma once
#include "Weapon.h"
#include "Actor.h"
#include <list>

class Enemy;

class MeleeSwipeWeapon : public Weapon {
public:
	MeleeSwipeWeapon(Game* game);
	~MeleeSwipeWeapon();
	
	void fire(Player* player, float targetX, float targetY) override;
	void update() override;
	bool canFire() override;
	std::string getName() override { return "Barrido Melee"; }
	std::string getIconPath() override { return "res/melee_icon.png"; }
	
	// Método para dibujar el arma cuando está activa
	void draw(float scrollX = 0, float scrollY = 0);
	
	// Método para verificar colisiones con enemigos
	void checkEnemyCollisions(std::list<Enemy*>* enemies);
	
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
	
	// Lista para evitar dañar al mismo enemigo múltiples veces
	std::list<Enemy*> alreadyHit;
};
