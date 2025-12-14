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
	std::string getName() override;
	std::string getIconPath() override { return "res/flame.png"; }
	
	// Método para dibujar el arma cuando está activa
	void draw(float scrollX = 0, float scrollY = 0);
	
	// Método para verificar colisiones con enemigos
	void checkEnemyCollisions(std::list<Enemy*>* enemies);
	
	// Método para desactivar el lanzallamas externamente
	void stopFiring() { isFiring = false; }
	
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
	
	// Timer para controlar daño continuo
	int damageTimer;
	int damageInterval;
};
