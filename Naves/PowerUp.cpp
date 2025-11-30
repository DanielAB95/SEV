#include "PowerUp.h"

PowerUp::PowerUp(string filename, float x, float y, int width, int height, Game* game) 
	: Actor (filename, x, y, width, height, game) {
	
	vx = 0;  // Sin movimiento horizontal
	vy = 0;  // Sin velocidad vertical inicial
}

void PowerUp::update() {
	// El movimiento vertical (gravedad) lo maneja Space::update()
	// Aquí solo actualizamos la lógica específica si es necesaria
}

void PowerUp::effect(Player* player) {

}



