#pragma once

#include "Layer.h"
#include "Player.h"
#include "Background.h"
#include "Enemy.h"
#include "BasicEnemy.h"
#include "SMovingEnemy.h"
#include "Projectile.h"
#include "Text.h"
#include "Audio.h"
#include "PowerUp.h"
#include "LifesPowerUp.h"
#include "ShootPowerUp.h"
#include "Tile.h"
#include <list>
#include <fstream> // Leer ficheros
#include <sstream> // Leer líneas / String
#include "Space.h" // importar

class GameLayer : public Layer
{
public:
	GameLayer(Game* game);
	void init() override;
	void processControls() override;
	void update() override;
	void draw() override;
	void keysToControls(SDL_Event event);
	void loadMap(string name);
	void loadMapObject(char character, float x, float y);
	void calculateScroll();
	Space* space;
	float scrollX;
	float scrollY;

	int mapWidth;
	list<Tile*> tiles;

	Audio* audioBackground;
	Text* textPoints;
	Text* lifePoints;
	Text* textShoots;
	int points;
	int newBasicEnemyTime = 0;
	int newSMovingEnemyTime = 0;
	int newLifePowerUpTime = 0;
	int newShootPowerUptime = 0;

	Player* player;
	Background* background;
	Actor* backgroundPoints;
	Actor* backgroundLives;

	bool controlShoot = false;
	int controlMoveY = 0;
	int controlMoveX = 0;

	list<Enemy*> enemies;
	list<Projectile*> projectiles;
	list<PowerUp*> powerUps;
};


