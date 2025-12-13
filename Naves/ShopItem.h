#pragma once
#include "Actor.h"
#include "Text.h"
#include "Player.h"
#include <string>

enum class ItemType {
	HEALTH_UPGRADE,
	DAMAGE_UPGRADE,
	SPEED_UPGRADE,
	MAX_AMMO_UPGRADE
};

class ShopItem {
public:
	ShopItem(ItemType type, int baseCost, float x, float y, Game* game);
	~ShopItem();
	
	bool canAfford(Player* player);
	bool purchase(Player* player);
	void draw(float offsetY = 0);
	void updatePosition(float x, float y);
	
	ItemType type;
	int cost;
	int baseCost;
	int level;
	int maxLevel;
	
	std::string name;
	std::string description;
	
	Actor* icon;
	Actor* background;
	Text* nameText;
	Text* costText;
	Text* levelText;
	Text* descriptionText;
	
	float x, y;
	Game* game;
	
private:
	void updateCost();
	void updateTexts();
	std::string getItemName();
	std::string getItemDescription();
	std::string getItemIcon();
};
