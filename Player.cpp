#include "Player.h"
#include "SimpleShotWeapon.h"
#include "MeleeSwipeWeapon.h"
#include "ShotgunWeapon.h"
#include "FlamethrowerWeapon.h"
#include "GrenadeWeapon.h"
#include "LaserBeamWeapon.h"
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

	aMoving = new Animation("res/astronauta.png", width, height,
		192, 32, 5, 6, game);
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
	// NO mover aquí - el movimiento lo maneja Space::update()
	// x = x + vx;
	// y = y + vy;
}

Projectile* Player::shoot(Enemy* target) {
	if (target == NULL) return NULL;

	// Obtener el arma actual
	Weapon* currentWeapon = getCurrentWeapon();
	if (currentWeapon == nullptr || !currentWeapon->unlocked) {
		// Si no hay arma o está bloqueada, no disparar
		return NULL;
	}

	// Verificar si el arma puede disparar (cooldown)
	if (!currentWeapon->canFire()) {
		return NULL;
	}

	// Verificar munición (si el arma usa munición)
	if (currentWeapon->maxAmmo > 0 && currentWeapon->ammo <= 0) {
		return NULL;
	}

	// El arma dispara usando su propia lógica
	currentWeapon->fire(this, target->x, target->y);

	// Consumir munición si el arma la usa
	if (currentWeapon->maxAmmo > 0) {
		currentWeapon->ammo--;
	}

	// Reproducir sonido de disparo
	audioShoot->play();

	// IMPORTANTE: Las armas manejan sus propios proyectiles internamente
	// No retornamos un proyectil aquí
	return NULL;
}


void Player::moveX(float axis) {
	vx = axis * moveSpeed;
}

void Player::moveY(float axis) {
	vy = axis * moveSpeed;
}


void Player::draw(float scrollX, float scrollY) {
	animation->draw(x - scrollX, y - scrollY);
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

	cout << "Sistema de armas inicializado: " << weapons.size() << " armas creadas" << endl;
}

Weapon* Player::getCurrentWeapon() {
	if (currentWeaponIndex >= 0 && currentWeaponIndex < weapons.size()) {
		return weapons[currentWeaponIndex];
	}
	return nullptr;
}

void Player::switchWeapon(int index) {
	if (index >= 0 && index < weapons.size()) {
		currentWeaponIndex = index;
		if (weapons[index]->unlocked) {
			cout << "Arma cambiada a: " << weapons[index]->getName() << " (DESBLOQUEADA)" << endl;
		}
		else {
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

std::vector<Projectile*> Player::getWeaponProjectiles() {
	std::vector<Projectile*> allProjectiles;

	// Recopilar proyectiles de todas las armas
	for (auto weapon : weapons) {
		// Cada arma debe tener una lista de proyectiles activos
		// (esto requiere modificar las clases de armas)
	}

	return allProjectiles;
}


