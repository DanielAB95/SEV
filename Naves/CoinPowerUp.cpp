#include "CoinPowerUp.h"
#include <iostream>
using namespace std;

CoinPowerUp::CoinPowerUp(float x, float y, Game* game)
	: PowerUp("res/icono_moneda.png", x, y, 32, 32, game) {
	
	vy = 0; // Sin movimiento vertical en vista top-down
	vx = 0; // Sin movimiento horizontal
	coinValue = 10; // Valor por defecto de la moneda
}

void CoinPowerUp::effect(Player* player) {
	player->money += coinValue;
	cout << "¡Moneda recogida! Dinero: " << player->money << endl;
}
