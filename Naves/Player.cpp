#include "Player.h"
#include "SimpleShotWeapon.h"
#include "MeleeSwipeWeapon.h"
#include "ShotgunWeapon.h"
#include "FlamethrowerWeapon.h"
#include "GrenadeWeapon.h"
#include "LaserBeamWeapon.h"
#include "Space.h"
#include <iostream>

using namespace std;

Player::Player(float x, float y, Game* game)
	: Actor("res/astronauta.png", x, y, 32, 32, game) {

	audioShoot = Audio::createAudio("res/efecto_disparo.wav", false);
	lives = 30;
	money = 0; // Inicializar dinero
	
	// INICIALIZAR STATS
	maxLives = 30;
	damage = 10;
	moveSpeed = 3.0f;

	aMoving = new Animation("res/astronauta.png", static_cast<float>(width), static_cast<float>(height),
		192.0f, 32.0f, 5, 6, game);
	animation = aMoving;
	
	// Inicializar sistema de armas
	initWeapons();
	currentWeaponIndex = 0;
}

void Player::update() {
	if (shootTime > 0) {
		shootTime--;
	}
	animation->update();
	
	// Actualizar todas las armas (para cooldowns)
	for (auto weapon : weapons) {
		weapon->update();
	}
}

void Player::shoot(Enemy* target) {
	if (target == NULL) return;
	
	// Obtener arma actual
	Weapon* currentWeapon = getCurrentWeapon();
	if (currentWeapon == nullptr || !currentWeapon->unlocked) {
		return; // No se puede disparar con arma bloqueada
	}
	
	// El arma maneja el disparo internamente (incluyendo el audio)
	currentWeapon->fire(this, target->x, target->y);
	// ¡Ya no reproducir audio aquí!
}

void Player::setWeaponReferences(std::list<Projectile*>* projectileList, Space* space) {
	// Configurar referencias en todas las armas
	for (size_t i = 0; i < weapons.size(); i++) {
		weapons[i]->setProjectileReferences(projectileList, space);
		cout << "Arma " << i << " (" << weapons[i]->getName() << ") configurada con referencias" << endl;
	}
	cout << "Referencias de proyectiles configuradas en todas las armas" << endl;
}

void Player::moveX(float axis) {
	vx = axis * moveSpeed;
}

void Player::moveY(float axis) {
	vy = axis * moveSpeed;
}

void Player::draw(float scrollX, float scrollY) {
	animation->draw(x - scrollX, y- scrollY);
}

void Player::upgradeHealth() {
	maxLives += 10; // +10 HP por mejora
	lives = maxLives; // Restaurar vida completa al mejorar
}

void Player::upgradeDamage() {
	damage += 5; // +5 daño por mejora (50% del daño base)
}

void Player::upgradeSpeed() {
	moveSpeed += 0.5f;
}

Player::~Player() {
	for (auto weapon : weapons) {
		delete weapon;
	}
	weapons.clear();
}

void Player::initWeapons() {
	weapons.push_back(new SimpleShotWeapon(game));
	weapons.push_back(new MeleeSwipeWeapon(game));
	weapons.push_back(new ShotgunWeapon(game));
	weapons.push_back(new FlamethrowerWeapon(game));
	weapons.push_back(new GrenadeWeapon(game));
	weapons.push_back(new LaserBeamWeapon(game));
	
	// TEMPORAL: Desbloquear algunas armas para testing
	weapons[1]->unlocked = true; // MeleeSwipeWeapon
	weapons[3]->unlocked = true; // FlamethrowerWeapon
	
	cout << "Sistema de armas inicializado: " << weapons.size() << " armas creadas" << endl;
	cout << "Armas desbloqueadas: SimpleShotWeapon, MeleeSwipeWeapon, ShotgunWeapon, FlamethrowerWeapon" << endl;
}

Weapon* Player::getCurrentWeapon() {
	if (currentWeaponIndex >= 0 && static_cast<size_t>(currentWeaponIndex) < weapons.size()) {
		return weapons[currentWeaponIndex];
	}
	return nullptr;
}

void Player::switchWeapon(int index) {
	if (index >= 0 && static_cast<size_t>(index) < weapons.size()) {
		currentWeaponIndex = index;
		if (weapons[index]->unlocked) {
			cout << "Arma cambiada a: " << weapons[index]->getName() << " (DESBLOQUEADA)" << endl;
		} else {
			cout << "Arma seleccionada: " << weapons[index]->getName() << " (BLOQUEADA - Visible pero no usable)" << endl;
		}
	}
}

void Player::unlockWeapon(WeaponType type) {
	for (auto weapon : weapons) {
		if (weapon->type == type) {
			weapon->unlocked = true;
			cout << "Arma desbloqueada: " << weapon->getName() << endl;
			break;
		}
	}
}












