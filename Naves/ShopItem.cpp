#include "ShopItem.h"
#include <iostream>
using namespace std;

ShopItem::ShopItem(ItemType type, int baseCost, int max, float x, float y, Game* game, bool repeatable) {
	this->type = type;
	this->baseCost = baseCost;
	this->cost = baseCost;
	this->x = x;
	this->y = y;
	this->game = game;
	this->level = 0;
	this->maxLevel = max;
	this->repeat = repeatable;
	
	// Crear elementos visuales
	background = new Actor("res/Banner.png", x, y, 500, 90, game);
	icon = new Actor(getItemIcon().c_str(), x - 220, y, 50, 50, game);
	
	// Crear textos
	nameText = new Text(getItemName(), x - 150, y - 25, game);
	nameText->content = getItemName();
	
	descriptionText = new Text(getItemDescription(), x - 150, y + 5, game);
	descriptionText->content = getItemDescription();
	
	levelText = new Text("0/" + to_string(maxLevel), x - 150, y + 30, game);
	
	costText = new Text(to_string(cost) + " monedas", x + 60, y, game);
}

ShopItem::~ShopItem() {
	delete background;
	delete icon;
	delete nameText;
	delete costText;
	delete levelText;
	delete descriptionText;
}

std::string ShopItem::getItemName() {
	switch (type) {
		case ItemType::HEALTH_UPGRADE:
			return "Mejora de Vida";
		case ItemType::GUN_SLOT:
			return "Nueva Arma";
		case ItemType::SPEED_UPGRADE:
			return "Mejora de Velocidad";
		case ItemType::RELOAD_UPGRADE:
			return "Recargar Municion";
		default:
			return "Item Desconocido";
	}
}

std::string ShopItem::getItemDescription() {
	switch (type) {
		case ItemType::HEALTH_UPGRADE:
			return "+10 Vida Maxima";
		case ItemType::GUN_SLOT:
			return " ";
		case ItemType::SPEED_UPGRADE:
			return "+0.5 Velocidad";
		case ItemType::RELOAD_UPGRADE:
			return "Full munición en armas";
		default:
			return "";
	}
}

std::string ShopItem::getItemIcon() {
	switch (type) {
		case ItemType::HEALTH_UPGRADE:
			return "res/corazon.png";
		case ItemType::GUN_SLOT:
			return "res/disparo_jugador.png";
		case ItemType::SPEED_UPGRADE:
			return "res/Astronauta.png"; // Usar astronauta como icono de velocidad
		case ItemType::RELOAD_UPGRADE:
			return "res/recolectable.png"; // Usar recolectable como munición
		default:
			return "res/moneda.png";
	}
}

void ShopItem::updateCost() {
	// Coste incrementa en un 50% por nivel
	cost = baseCost + (level * baseCost / 2);
}

void ShopItem::updateTexts() {
	levelText->content = to_string(level) + "/" + to_string(maxLevel);
	costText->content = to_string(cost) + " monedas";
	
	// Cambiar color si está al máximo
	if (level >= maxLevel) {
		costText->content = "MÁXIMO";
	}
}

bool ShopItem::canAfford(Player* player) {
	bool affordable = player->money >= cost && level < maxLevel;
	bool available;
	if (repeat) {
		available = repeat;
	}
	else {
		available = !bought;
	}
	return affordable && available;
}

bool ShopItem::purchase(Player* player) {
	if (!canAfford(player)) {
		cout << "No tienes suficiente dinero o nivel maximo alcanzado" << endl;
		return false;
	}
	
	// Restar dinero
	player->money -= cost;
	level++;
	if (!repeat) {
		bought = true;
	}
	
	// Aplicar mejora según el tipo
	switch (type) {
		case ItemType::HEALTH_UPGRADE:
			player->upgradeHealth();
			cout << "¡Vida mejorada! Nueva vida maxima: " << player->maxLives << endl;
			break;
		case ItemType::GUN_SLOT:
			player->unlockNextWeapon();
			cout << "Nueva arma " << player->damage << endl;
			break;
		case ItemType::SPEED_UPGRADE:
			player->upgradeSpeed();
			cout << "¡Velocidad mejorada! Nueva velocidad: " << player->moveSpeed << endl;
			break;
		case ItemType::RELOAD_UPGRADE:
			player->reload();
			cout << "¡Municion recargada! Total: " << player->numberOfShoots << endl;
			break;
	}
	
	// Actualizar coste y textos
	updateCost();
	updateTexts();
	
	return true;
}

void ShopItem::draw(float offsetY) {
	
	// Dibujar fondo
	background->x = x;
	background->y = y;
	background->draw();
	
	// Dibujar icono
	icon->x = x - 200;
	icon->y = y;
	icon->draw();
	
	// Dibujar textos
	nameText->x = x;
	nameText->y = y - 27;
	nameText->draw();
	
	descriptionText->x = x;
	descriptionText->y = y;
	descriptionText->draw();
	
	levelText->x = x;
	levelText->y = y + 27;
	levelText->draw();
	
	costText->x = x + 160;
	costText->y = y + 27;
	costText->draw();
}

void ShopItem::updatePosition(float newX, float newY) {
	this->x = newX;
	this->y = newY;
}