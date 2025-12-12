#include "Projectile.h"

Projectile::Projectile(float xTarget, float yTarget, float x, float y, Game* game) :
    Actor("res/disparo_jugador.png", x, y, 18, 6, game)
{
    this->xTarget = xTarget;
    this->yTarget = yTarget;

    // Calculamos el vector hacia el objetivo
    float dx = xTarget - x;
    float dy = yTarget - y;
    float distance = sqrt(dx * dx + dy * dy);

    float speed = 9.0f; // velocidad del proyectil

    if (distance > 0) {
        vx = (dx / distance) * speed;
        vy = (dy / distance) * speed;
    }
    else {
        vx = speed;
        vy = 0;
    }
    
}


void Projectile::update() {
    // El movimiento lo maneja Space::update()
}
