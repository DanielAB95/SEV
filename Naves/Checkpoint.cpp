#include "Checkpoint.h"

Checkpoint::Checkpoint(float x, float y, Game* game)
	: Actor("res/checkpoint.png", x, y, 32, 32, game) {
	active = false;
	used = false;
}

void Checkpoint::update() {
	// Animación visual si está activo
	if (active) {
		// Efecto visual simple
	}
}

void Checkpoint::activate() {
	active = true;
}