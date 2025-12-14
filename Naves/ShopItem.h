#pragma once
#include "Actor.h"
#include "Text.h"
#include "Player.h"
#include <string>

enum class ItemType {
	HEALTH_UPGRADE,
	GUN_SLOT,
	SPEED_UPGRADE,
	RELOAD_UPGRADE
};

class ShopItem {
public:
	ShopItem(ItemType type, int baseCost, int max, float x, float y, Game* game, bool repeatable = false);
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
	bool repeat;
	bool bought = false;
	
private:
	void updateCost();
	void updateTexts();
	std::string getItemName();
	std::string getItemDescription();
	std::string getItemIcon();
};
