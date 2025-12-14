#pragma once
#include "Weapon.h"
#include "Actor.h"
#include <list>

class Enemy;  // Forward declaration

class GrenadeProjectile : public Actor {
public:
	GrenadeProjectile(float targetX, float targetY, float startX, float startY, Game* game);
	
	void update();
	bool hasExploded();
	void explode() { exploded = true; explosionTime = 0; }  // Método para forzar explosión
	
	float vx, vy;
	float targetX, targetY;        // Posición objetivo para top-down
	float startX, startY;          // Posición inicial para calcular distancia
	float arrivalThreshold;        // Distancia mínima para considerar llegada
	float maxDistance;             // Distancia máxima antes de explotar
	int maxLifetime;               // Tiempo máximo de vida antes de explotar
	int currentLifetime;           // Tiempo actual de vida
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
	
	// Métodos adicionales
	void draw(float scrollX = 0, float scrollY = 0);
	void checkExplosionCollisions(std::list<Enemy*>* enemies);
};
