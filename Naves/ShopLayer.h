#pragma once
#include "Layer.h"
#include "ShopItem.h"
#include "Player.h"
#include "Background.h"
#include "Text.h"
#include <vector>

class ShopLayer : public Layer {
public:
	ShopLayer(Game* game);
	~ShopLayer();
	
	void init() override;
	void processControls() override;
	void update() override;
	void draw() override;
	void keysToControls(SDL_Event event);
	
	void setPlayer(Player* player);
	void setNextLevel(int level); // Nuevo: establecer el siguiente nivel a cargar
	void resetBoughts();
	
	
private:
	Player* player;
	Background* background;
	Text* titleText;
	Text* moneyText;
	Text* instructionsText;
	Text* exitText;
	Actor* selector; // Indicador de selección
	
	std::vector<ShopItem*> shopItems;
	int selectedItemIndex;
	
	bool controlUp;
	bool controlDown;
	bool controlBuy;
	bool controlExit;
	
	// Variables para manejar transición entre niveles
	int nextLevelToLoad;
	bool shouldLoadNextLevel;
	
	void updateMoneyDisplay();
	void selectNextItem();
	void selectPreviousItem();
	void buySelectedItem();
};
