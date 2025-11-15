#include "Player.h"

Player::Player(float x, float y, Game* game)
	: Actor("res/jugador.png", x, y, 50, 57, game) {

	audioShoot = Audio::createAudio("res/efecto_disparo.wav", false);
	lives = 3;
}

void Player::update() {
	if (shootTime > 0) {
		shootTime--;
	}
	x = x + vx;
	y = y + vy;
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


