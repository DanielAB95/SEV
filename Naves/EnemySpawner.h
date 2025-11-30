#pragma once
#include "Actor.h"
#include "Game.h"
#include <string>

class EnemySpawner : public Actor
{
public:
    EnemySpawner(float x, float y, std::string enemyType, Game* game);
    void update();
    bool shouldSpawn();
    void resetTimer();
    
    std::string enemyType;  // Tipo de enemigo a generar: "B", "R", "T"
    int spawnInterval;      // Tiempo entre spawns (en frames)
    int spawnTimer;         // Temporizador actual
    int maxEnemies;         // Máximo de enemigos activos de este spawner
    int currentEnemies;     // Enemigos activos actualmente
    bool active;            // Si el spawner está activo
};
