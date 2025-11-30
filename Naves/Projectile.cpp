#include "Projectile.h"

Projectile::Projectile(float x, float y, Game* game) :
	Actor("res/disparo_jugador.png", x, y, 18, 6, game) {
	vx = 9;
}

void Projectile::update() {
	// El movimiento lo maneja Space::update() mediante vx
	// No actualizar x aquí para evitar doble movimiento
}
