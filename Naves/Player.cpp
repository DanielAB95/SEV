#include "Player.h"

Player::Player(float x, float y, Game* game)
	: Actor("res/astronauta.png", x, y, 32, 32, game) {

	audioShoot = Audio::createAudio("res/efecto_disparo.wav", false);
	lives = 3;
	money = 0; // Inicializar dinero

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

Projectile* Player::shoot() {
	if (shootTime == 0 && numberOfShoots > 0) {
		audioShoot->play();
		shootTime = shootCadence;
		numberOfShoots--;
		return new Projectile(x, y, game);
	}
	else {
		return NULL;
	}
}


void Player::moveX(float axis) {
	vx = axis * 3;
}

void Player::moveY(float axis) {
	vy = axis * 3;
}


void Player::draw(float scrollX, float scrollY) {
	animation->draw(x - scrollX, y- scrollY);
}



