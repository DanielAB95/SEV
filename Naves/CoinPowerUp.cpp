#include "CoinPowerUp.h"
#include <iostream>
using namespace std;

CoinPowerUp::CoinPowerUp(float x, float y, Game* game)
	: PowerUp("res/moneda.png", x, y, 32, 32, game) {
	
	vy = 0; // Sin movimiento vertical en vista top-down
	vx = 0; // Sin movimiento horizontal
	coinValue = 80; // AUMENTADO: Valor de la moneda (era 35)
}

CoinPowerUp::CoinPowerUp(float x, float y, int value, Game* game)
	: PowerUp("res/moneda.png", x, y, 32, 32, game) {
	
	vy = 0;
	vx = 0;
	coinValue = value;
}

void CoinPowerUp::effect(Player* player) {
	player->money += coinValue;
	cout << "¡Moneda recogida! Dinero: " << player->money << endl;
}

