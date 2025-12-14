#pragma once

#include "Layer.h"
#include "Player.h"
#include "Background.h"
#include "Enemy.h"
#include "BasicEnemy.h"
#include "ChargeEnemy.h"
#include "ShooterEnemy.h"
#include "SMovingEnemy.h"
#include "Projectile.h"
#include "EnemyProjectile.h"
#include "Text.h"
#include "Audio.h"
#include "PowerUp.h"
#include "LifesPowerUp.h"
#include "ShootPowerUp.h"
#include "CoinPowerUp.h"
#include "LifeSpawner.h"
#include "EnemySpawner.h"
#include "Tile.h"
#include <list>
#include <fstream> // Leer ficheros
#include <sstream> // Leer líneas / String
#include <vector>
#include <random>
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
	
	// Sistema de niveles y tiempo
	int currentLevel;
	int totalLevels;
	float levelTime;
	float levelDuration;
	bool levelCompleted;
	float shopHintTimer; // Temporizador para mostrar el hint de tienda
	
	// Variables para conservar estado del jugador entre niveles
	int savedPlayerMoney;
	int savedPlayerLives;
	int savedPlayerMaxLives;
	int savedPlayerDamage;
	int savedPlayerShoots;
	float savedPlayerMoveSpeed;
	std::vector<Weapon*> savedPlayerWeapons;
	bool hasPlayerData;
	void loadLevel(int level);
	void nextLevel();
	float getLevelDuration(int level);
	void clearLevel();

	Enemy* closestEnemy();
	
	Space* space;
	float scrollX;
	float scrollY;

	int mapWidth;
	list<Tile*> tiles;

	Audio* audioBackground;
	Text* textPoints;
	Text* lifePoints;
	Text* textShoots;
	Text* textWeapon;
	Actor* weaponIcon;
	// Sistema de armas visual - HUD
	std::vector<Actor*> weaponIcons; // Iconos de todas las armas
	std::vector<Text*> weaponNumbers; // Números de las armas (1-6)
	Actor* weaponSelector; // Indicador visual del arma seleccionada
	// Textos para el sistema de niveles
	Text* textLevel;
	Text* textTime;
	Text* textMoney; // Texto para mostrar el dinero
	Text* shopHint; // Texto para indicar cómo abrir la tienda
	int points;
	int newBasicEnemyTime = 0;
	int newSMovingEnemyTime = 0;
	int newLifePowerUpTime = 0;
	int newShootPowerUptime = 0;

	Player* player;
	Background* background;
	Actor* backgroundPoints;
	Actor* backgroundLives;
	Actor* backgroundMoney; // Icono para el dinero

	bool controlShoot = false;
	int controlMoveY = 0;
	int controlMoveX = 0;

	list<Enemy*> enemies;
	list<Projectile*> projectiles;
	list<PowerUp*> powerUps;
	list<LifeSpawner*> lifeSpawners; // Lista de spawners de vida
	list<EnemySpawner*> enemySpawners; // Lista de spawners de enemigos
	list<EnemyProjectile*> enemyProjectiles; // Lista de proyectiles enemigos
};





























