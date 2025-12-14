#include "GrenadeWeapon.h"
#include "Player.h"
#include "Enemy.h"
#include <cmath>
#include <iostream>

GrenadeProjectile::GrenadeProjectile(float targetX, float targetY, float startX, float startY, Game* game)
	: Actor("res/grenade.png", startX, startY, 20, 20, game) {
	
	// Guardar posición objetivo y inicial para verificar llegada
	this->targetX = targetX;
	this->targetY = targetY;
	this->startX = startX;
	this->startY = startY;
	
	// Calcular dirección y velocidad hacia el objetivo (top-down)
	float dx = targetX - startX;
	float dy = targetY - startY;
	float distance = sqrt(dx * dx + dy * dy);
	
	// Velocidad constante hacia el objetivo (sin gravedad, es top-down)
	float speed = 6.0f; // Velocidad de la granada
	if (distance > 0) {
		vx = (dx / distance) * speed;
		vy = (dy / distance) * speed;
	} else {
		vx = 0;
		vy = 0;
	}
	
	// Configurar propiedades de explosión
	explosionRadius = 80;
	damage = 30;
	exploded = false;
	explosionTime = 0;
	maxExplosionTime = 10;
	
	// Distancia mínima para considerar que llegó al objetivo
	arrivalThreshold = 15.0f;
	
	// Configurar límites de seguridad para evitar granadas infinitas
	maxDistance = 400.0f;            // Distancia máxima antes de auto-explosión
	maxLifetime = 120;               // 2 segundos máximo (60fps * 2)
	currentLifetime = 0;
	
	std::cout << "Granada lanzada hacia (" << targetX << ", " << targetY << ") desde (" << startX << ", " << startY 
	          << ") - Límites: " << maxDistance << "px, " << maxLifetime << " frames" << std::endl;
}

void GrenadeProjectile::update() {
	if (!exploded) {
		// Incrementar tiempo de vida
		currentLifetime++;
		
		// Mover hacia el objetivo (sin gravedad)
		x += vx;
		y += vy;
		
		// 1. Verificar si ha llegado al objetivo
		float distanceToTarget = sqrt((x - targetX) * (x - targetX) + (y - targetY) * (y - targetY));
		if (distanceToTarget <= arrivalThreshold) {
			exploded = true;
			explosionTime = 0;
			// Posicionar exactamente en el objetivo
			x = targetX;
			y = targetY;
			std::cout << "¡Granada explotó en posición objetivo (" << x << ", " << y << ")!" << std::endl;
		}
		
		// 2. Verificar límite de distancia máxima
		else {
			float distanceFromStart = sqrt((x - startX) * (x - startX) + (y - startY) * (y - startY));
			if (distanceFromStart >= maxDistance) {
				exploded = true;
				explosionTime = 0;
				std::cout << "¡Granada explotó por alcanzar distancia máxima (" << distanceFromStart << "px)!" << std::endl;
			}
			
			// 3. Verificar límite de tiempo de vida
			else if (currentLifetime >= maxLifetime) {
				exploded = true;
				explosionTime = 0;
				std::cout << "¡Granada explotó por timeout (" << currentLifetime << " frames)!" << std::endl;
			}
		}
	} else {
		explosionTime++;
	}
}

bool GrenadeProjectile::hasExploded() {
	return exploded && explosionTime >= maxExplosionTime;
}

GrenadeWeapon::GrenadeWeapon(Game* game) : Weapon(WeaponType::GRENADE, game) {
	cooldown = 60;
	currentCooldown = 0;
	ammo = 10;
	maxAmmo = 999;
	damage = 30;
	unlocked = true; // Desbloqueado para testing
	explosionRadius = 80;
	
	std::cout << "GrenadeWeapon creada y desbloqueada" << std::endl;
}

void GrenadeWeapon::fire(Player* player, float targetX, float targetY) {
	if (!canFire()) return;
	
	currentCooldown = cooldown;
	ammo--;
	
	GrenadeProjectile* grenade = new GrenadeProjectile(targetX, targetY, player->x, player->y, game);
	grenade->damage = player->damage + damage;
	activeGrenades.push_back(grenade);
	
	std::cout << "Granada disparada. Munición restante: " << ammo << std::endl;
}

void GrenadeWeapon::update() {
	if (currentCooldown > 0) {
		currentCooldown--;
	}
	
	for (auto it = activeGrenades.begin(); it != activeGrenades.end(); ) {
		(*it)->update();
		
		if ((*it)->hasExploded()) {
			delete (*it);
			it = activeGrenades.erase(it);
		} else {
			++it;
		}
	}
}

bool GrenadeWeapon::canFire() {
	return currentCooldown == 0 && ammo > 0;
}

std::string GrenadeWeapon::getName() {
	return "Granada (" + std::to_string(ammo) + ")";
}

std::string GrenadeWeapon::getIconPath() {
	return "res/recolectable.png";
}

void GrenadeWeapon::draw(float scrollX, float scrollY) {
	// Dibujar todas las granadas activas
	for (std::list<GrenadeProjectile*>::const_iterator it = activeGrenades.begin(); it != activeGrenades.end(); ++it) {
		(*it)->draw(scrollX, scrollY);
	}
}

void GrenadeWeapon::checkExplosionCollisions(std::list<Enemy*>* enemies) {
	if (enemies == nullptr) return;
	
	for (std::list<GrenadeProjectile*>::iterator it = activeGrenades.begin(); it != activeGrenades.end(); ++it) {
		GrenadeProjectile* grenade = *it;
		if (grenade->exploded && grenade->explosionTime < grenade->maxExplosionTime) {
			// Verificar colisiones en el radio de explosión
			for (std::list<Enemy*>::iterator enemyIt = enemies->begin(); enemyIt != enemies->end(); ++enemyIt) {
				Enemy* enemy = *enemyIt;
				float dx = enemy->x - grenade->x;
				float dy = enemy->y - grenade->y;
				float distance = sqrt(dx * dx + dy * dy);
				
				if (distance <= grenade->explosionRadius) {
					// Aplicar daño basado en distancia (más cerca = más daño)
					float damageFactor = 1.0f - (distance / grenade->explosionRadius);
					int explosionDamage = (int)(grenade->damage * damageFactor);
					enemy->lives -= explosionDamage;
					
					std::cout << "¡Explosión dañó enemigo! Distancia: " << distance 
					          << ", Factor: " << damageFactor << ", Daño: " << explosionDamage
					          << ", Vidas restantes: " << enemy->lives << std::endl;
				}
			}
		}
	}
}