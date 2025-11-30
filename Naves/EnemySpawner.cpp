#include "EnemySpawner.h"

EnemySpawner::EnemySpawner(float x, float y, std::string enemyType, Game* game)
    : Actor("res/spawner_enemigo.png", x, y, 32, 32, game) {
    
    this->enemyType = enemyType;
    spawnInterval = 180; // 6 segundos aproximadamente (180 frames a 30 FPS)
    spawnTimer = 0;
    maxEnemies = 3; // Máximo 3 enemigos activos por spawner
    currentEnemies = 0;
    active = true;
}

void EnemySpawner::update() {
    if (!active) return;
    
    spawnTimer++;
}

bool EnemySpawner::shouldSpawn() {
    return active && (spawnTimer >= spawnInterval) && (currentEnemies < maxEnemies);
}

void EnemySpawner::resetTimer() {
    spawnTimer = 0;
}
