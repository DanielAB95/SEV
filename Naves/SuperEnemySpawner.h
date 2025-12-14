#pragma once
#include "Actor.h"
#include "Game.h"
#include "Enemy.h"
#include "BasicEnemy.h"
#include "ChargeEnemy.h"
#include "ShooterEnemy.h"
#include <string>

class SuperEnemySpawner : public Actor {
public:
    SuperEnemySpawner(float x, float y, std::string enemyType, Game* game);
    SuperEnemySpawner(float x, float y, Game* game); // Spawner aleatorio
    
    void update();
    bool shouldSpawn();
    void resetTimer();
    Enemy* spawnSuperEnemy(); // Crear super enemigo
    
    std::string enemyType; // "B", "R", "T" o "RANDOM"
    int spawnInterval;
    int spawnTimer;
    bool active;
    bool hasSpawned; // Solo spawnea una vez
    
private:
    void upgradeToBoss(Enemy* enemy); // Convertir enemigo normal en boss
    Enemy* createRandomSuperEnemy(); // Crear super enemigo aleatorio
};