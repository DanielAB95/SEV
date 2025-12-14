#pragma once

#include "Actor.h"
#include "Projectile.h"
#include "Audio.h"
#include "Animation.h"
#include "Weapon.h"
#include <vector>
#include <list>

class Space;

class Player : public Actor
{
public:
Player(float x, float y, Game* game);
~Player();
	void shoot(Enemy* target); // Cambiar a void porque las armas manejan los proyectiles internamente
	void update();
	void moveX(float axis);
	void moveY(float axis);
	void draw(float scrollX = 0, float scrollY = 0) override;
	
	// Configurar referencias para que las armas puedan crear proyectiles
	void setWeaponReferences(std::list<Projectile*>* projectileList, Space* space);
	
	Animation* aMoving;
	Animation* animation;
	Audio* audioShoot;
	int shootCadence = 22;
	int shootTime = 0;
	int numberOfShoots = 20;
	int lives;
	int money; // Dinero recolectado
	
	// STATS DEL JUGADOR
	int maxLives;
	int damage;
	float moveSpeed;
	
	// SISTEMA DE ARMAS
	std::vector<Weapon*> weapons;
	int currentWeaponIndex; // Índice del arma actual (público para HUD)
	Weapon* getCurrentWeapon();
	void switchWeapon(int index);
	void unlockWeapon(WeaponType type);
	void initWeapons();
	
	// Métodos para mejorar stats
	void upgradeHealth();
	void upgradeDamage();
	void upgradeSpeed();
};

