#pragma once
#include "Actor.h"
#include "Game.h"

class LifeSpawner : public Actor
{
public:
    LifeSpawner(float x, float y, Game* game);
    void update();
    bool shouldSpawn();
    void resetTimer();
    
    int spawnInterval; // Tiempo entre spawns (en frames)
    int spawnTimer;    // Temporizador actual
    bool active;       // Si el spawner está activo
};
