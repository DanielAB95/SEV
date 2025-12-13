#include "Player.h"

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
	if (shootTime == 0 && numberOfShoots > 0) {
		audioShoot->play();
		shootTime = shootCadence;
		numberOfShoots--;
		Projectile* projectile = new Projectile(target->x, target->y, x, y, game);
		projectile->damage = this->damage; // Asignar daño del jugador al proyectil
		return projectile;
	}
	else {
		return NULL;
	}
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



