#include "ShopItem.h"
#include <iostream>
using namespace std;

ShopItem::ShopItem(ItemType type, int baseCost, float x, float y, Game* game) {
	this->type = type;
	this->baseCost = baseCost;
	this->cost = baseCost;
	this->x = x;
	this->y = y;
	this->game = game;
	this->level = 0;
	this->maxLevel = 10; // Máximo 10 niveles por mejora
	
	// Crear elementos visuales
	background = new Actor("res/Banner.png", x, y, 500, 90, game);
	icon = new Actor(getItemIcon().c_str(), x - 220, y, 50, 50, game);
	
	// Crear textos
	nameText = new Text(getItemName(), x - 150, y - 25, game);
	nameText->content = getItemName();
	
	descriptionText = new Text(getItemDescription(), x - 150, y + 5, game);
	descriptionText->content = getItemDescription();
	
	levelText = new Text("Nivel: 0/" + to_string(maxLevel), x - 150, y + 30, game);
	
	costText = new Text("Coste: " + to_string(cost) + " monedas", x + 60, y, game);
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
		case ItemType::DAMAGE_UPGRADE:
			return "Mejora de Dano";
		case ItemType::SPEED_UPGRADE:
			return "Mejora de Velocidad";
		case ItemType::MAX_AMMO_UPGRADE:
			return "Municion Maxima";
		default:
			return "Item Desconocido";
	}
}

std::string ShopItem::getItemDescription() {
	switch (type) {
		case ItemType::HEALTH_UPGRADE:
			return "+10 Vida Maxima";
		case ItemType::DAMAGE_UPGRADE:
			return "+5 Dano";
		case ItemType::SPEED_UPGRADE:
			return "+0.5 Velocidad";
		case ItemType::MAX_AMMO_UPGRADE:
			return "+10 Disparos Iniciales";
		default:
			return "";
	}
}

std::string ShopItem::getItemIcon() {
	switch (type) {
		case ItemType::HEALTH_UPGRADE:
			return "res/corazon.png";
		case ItemType::DAMAGE_UPGRADE:
			return "res/disparo_jugador.png";
		case ItemType::SPEED_UPGRADE:
			return "res/Astronauta.png"; // Usar astronauta como icono de velocidad
		case ItemType::MAX_AMMO_UPGRADE:
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
	levelText->content = "Nivel: " + to_string(level) + "/" + to_string(maxLevel);
	costText->content = "Coste: " + to_string(cost) + " monedas";
	
	// Cambiar color si está al máximo
	if (level >= maxLevel) {
		costText->content = "MAXIMO NIVEL";
	}
}

bool ShopItem::canAfford(Player* player) {
	return player->money >= cost && level < maxLevel;
}

bool ShopItem::purchase(Player* player) {
	if (!canAfford(player)) {
		cout << "No tienes suficiente dinero o nivel maximo alcanzado" << endl;
		return false;
	}
	
	// Restar dinero
	player->money -= cost;
	level++;
	
	// Aplicar mejora según el tipo
	switch (type) {
		case ItemType::HEALTH_UPGRADE:
			player->upgradeHealth();
			cout << "¡Vida mejorada! Nueva vida maxima: " << player->maxLives << endl;
			break;
		case ItemType::DAMAGE_UPGRADE:
			player->upgradeDamage();
			cout << "¡Dano mejorado! Nuevo dano: " << player->damage << endl;
			break;
		case ItemType::SPEED_UPGRADE:
			player->upgradeSpeed();
			cout << "¡Velocidad mejorada! Nueva velocidad: " << player->moveSpeed << endl;
			break;
		case ItemType::MAX_AMMO_UPGRADE:
			player->numberOfShoots += 10;
			cout << "¡Municion recargada! +10 disparos disponibles. Total: " << player->numberOfShoots << endl;
			break;
	}
	
	// Actualizar coste y textos
	updateCost();
	updateTexts();
	
	return true;
}

void ShopItem::draw(float offsetY) {
	float finalY = y + offsetY;
	
	// Dibujar fondo
	background->x = x;
	background->y = finalY;
	background->draw();
	
	// Dibujar icono
	icon->x = x - 220;
	icon->y = finalY;
	icon->draw();
	
	// Dibujar textos
	nameText->x = x - 150;
	nameText->y = finalY - 25;
	nameText->draw();
	
	descriptionText->x = x - 150;
	descriptionText->y = finalY + 5;
	descriptionText->draw();
	
	levelText->x = x - 150;
	levelText->y = finalY + 30;
	levelText->draw();
	
	costText->x = x + 60;
	costText->y = finalY;
	costText->draw();
}

void ShopItem::updatePosition(float newX, float newY) {
	this->x = newX;
	this->y = newY;
}
