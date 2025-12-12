#include "LifeSpawner.h"

LifeSpawner::LifeSpawner(float x, float y, Game* game)
    : Actor("res/SpawnerVidas.png", x, y, 32, 32, game) {
    
    spawnInterval = 300; // 10 segundos aproximadamente (300 frames a 30 FPS)
    spawnTimer = 0;
    active = true;
}

void LifeSpawner::update() {
    if (!active) return;
    
    spawnTimer++;
}

bool LifeSpawner::shouldSpawn() {
    return active && (spawnTimer >= spawnInterval);
}

void LifeSpawner::resetTimer() {
    spawnTimer = 0;
}
