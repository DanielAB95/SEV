#include "SuperEnemySpawner.h"
#include <random>
#include <vector>
#include <iostream>

using namespace std;

SuperEnemySpawner::SuperEnemySpawner(float x, float y, std::string enemyType, Game* game)
    : Actor("res/Spawner.png", x, y, 32, 32, game) {
    
    this->enemyType = enemyType;
    spawnInterval = 150; // 5 segundos aprox (150 frames a 30 FPS)
    spawnTimer = 0;
    active = true;
    hasSpawned = false;
    
    cout << "SuperEnemySpawner creado - Tipo: " << enemyType << " en (" << x << ", " << y << ")" << endl;
}

SuperEnemySpawner::SuperEnemySpawner(float x, float y, Game* game)
    : Actor("res/Spawner.png", x, y, 32, 32, game) {
    
    this->enemyType = "RANDOM";
    spawnInterval = 150; // 5 segundos aprox
    spawnTimer = 0;
    active = true;
    hasSpawned = false;
    
    cout << "SuperEnemySpawner aleatorio creado en (" << x << ", " << y << ")" << endl;
}

void SuperEnemySpawner::update() {
    if (!active || hasSpawned) return;
    
    spawnTimer++;
}

bool SuperEnemySpawner::shouldSpawn() {
    return active && !hasSpawned && (spawnTimer >= spawnInterval);
}

void SuperEnemySpawner::resetTimer() {
    spawnTimer = 0;
}

Enemy* SuperEnemySpawner::spawnSuperEnemy() {
    if (hasSpawned) return nullptr;
    
    Enemy* superEnemy = nullptr;
    
    if (enemyType == "RANDOM") {
        superEnemy = createRandomSuperEnemy();
    } else if (enemyType == "B") {
        superEnemy = new BasicEnemy(x, y, game);
        cout << "Creando SUPER BasicEnemy..." << endl;
    } else if (enemyType == "R") {
        superEnemy = new ChargeEnemy(x, y, game);
        cout << "Creando SUPER ChargeEnemy..." << endl;
    } else if (enemyType == "T") {
        superEnemy = new ShooterEnemy(x, y, game);
        cout << "Creando SUPER ShooterEnemy..." << endl;
    }
    
    if (superEnemy != nullptr) {
        upgradeToBoss(superEnemy);
        hasSpawned = true;
        active = false; // Solo spawna una vez
        
        cout << "¡SUPER ENEMIGO CREADO! Tipo: " << enemyType 
             << " - Vidas: " << superEnemy->lives 
             << " - Tamaño: " << superEnemy->width << "x" << superEnemy->height << endl;
    }
    
    return superEnemy;
}

Enemy* SuperEnemySpawner::createRandomSuperEnemy() {
    static random_device rd;
    static mt19937 gen(rd());
    
    // PROBABILIDADES EQUIPROBABLES (33.33% cada uno)
    vector<int> weights = {
        33,  // BasicEnemy (33.33%)
        33,  // ChargeEnemy (33.33%) 
        34   // ShooterEnemy (33.34%) - Ligero redondeo
    };
    
    static discrete_distribution<> dist(weights.begin(), weights.end());
    
    int randomType = dist(gen);
    
    cout << "=== GENERANDO SUPER ENEMIGO ALEATORIO ===" << endl;
    cout << "Número generado: " << randomType << endl;
    
    switch (randomType) {
        case 0:
            cout << "Spawner aleatorio eligió: SUPER BasicEnemy (33.33%)" << endl;
            return new BasicEnemy(x, y, game);  // ¡USAR POSICIÓN DEL SPAWNER!
        case 1:
            cout << "Spawner aleatorio eligió: SUPER ChargeEnemy (33.33%)" << endl;
            return new ChargeEnemy(x, y, game); // ¡USAR POSICIÓN DEL SPAWNER!
        case 2:
            cout << "Spawner aleatorio eligió: SUPER ShooterEnemy (33.34%)" << endl;
            return new ShooterEnemy(x, y, game); // ¡USAR POSICIÓN DEL SPAWNER!
        default:
            return new BasicEnemy(x, y, game);   // ¡USAR POSICIÓN DEL SPAWNER!
    }
}

void SuperEnemySpawner::upgradeToBoss(Enemy* enemy) {
    if (enemy == nullptr) return;
    
    cout << "=== UPGRADING ENEMY TO BOSS ===" << endl;
    cout << "Stats ANTES - Vidas: " << enemy->lives << ", Daño: " << enemy->damage 
         << ", Velocidad: " << enemy->moveSpeed << ", Tamaño: " << enemy->width << "x" << enemy->height << endl;
    
    // STATS ESPECÍFICOS PARA CADA TIPO (como verdaderos bosses)
    BasicEnemy* basicEnemy = dynamic_cast<BasicEnemy*>(enemy);
    if (basicEnemy != nullptr) {
        // SUPER BasicEnemy: Tanque implacable
        enemy->maxLives = 600;  // Mucha resistencia
        enemy->lives = 600;
        enemy->damage = 15;     // Daño alto pero no instakill
        enemy->moveSpeed = 1.2f; // Velocidad moderada
        enemy->coinReward = 1800; // Recompensa alta
        
        // ¡ESCALAR ANIMACIÓN PARA HACERLO MÁS GRANDE VISUALMENTE!
        if (enemy->animation != nullptr) {
            enemy->animation->actorWidth = static_cast<int>(enemy->animation->actorWidth * 2.0f);
            enemy->animation->actorHeight = static_cast<int>(enemy->animation->actorHeight * 2.0f);
        }
        // También escalar las propiedades del actor para colisiones
        enemy->width = static_cast<int>(enemy->width * 2.0f);
        enemy->height = static_cast<int>(enemy->height * 2.0f);
        
        cout << "BasicEnemy → SUPER TANK BOSS (Persigue implacablemente)" << endl;
    }
    
    ChargeEnemy* chargeEnemy = dynamic_cast<ChargeEnemy*>(enemy);
    if (chargeEnemy != nullptr) {
        // SUPER ChargeEnemy: Ariete devastador
        enemy->maxLives = 500;   // Resistencia alta
        enemy->lives = 500;
        enemy->damage = 20;      // Daño alto en carga pero no instakill
        enemy->moveSpeed = 1.0f; // Velocidad base normal
        chargeEnemy->chargeSpeed *= 2.0f; // Carga 2x más rápida
        chargeEnemy->detectionRange *= 1.5f; // Detecta desde más lejos
        enemy->coinReward = 2000; // Recompensa muy alta
        
        // ¡ESCALAR ANIMACIÓN!
        if (enemy->animation != nullptr) {
            enemy->animation->actorWidth = static_cast<int>(enemy->animation->actorWidth * 2.0f);
            enemy->animation->actorHeight = static_cast<int>(enemy->animation->actorHeight * 2.0f);
        }
        enemy->width = static_cast<int>(enemy->width * 2.0f);
        enemy->height = static_cast<int>(enemy->height * 2.0f);
        
        cout << "ChargeEnemy → SUPER RAMMING BOSS (Carga devastadora 2x velocidad)" << endl;
    }
    
    ShooterEnemy* shooterEnemy = dynamic_cast<ShooterEnemy*>(enemy);
    if (shooterEnemy != nullptr) {
        // SUPER ShooterEnemy: Artillero letal
        enemy->maxLives = 700;   // Muy resistente
        enemy->lives = 700;
        enemy->damage = 10;      // Daño moderado en contacto
        enemy->moveSpeed = 0.8f; // Más lento pero letal
        shooterEnemy->shootCadence = 30; // ¡Dispara 3x más rápido!
        shooterEnemy->maxDistance *= 1.8f; // Rango mucho más largo
        shooterEnemy->minDistance *= 1.3f; // Mantiene distancia mayor
        enemy->coinReward = 2200; // Recompensa máxima
        
        // ¡ESCALAR ANIMACIÓN!
        if (enemy->animation != nullptr) {
            enemy->animation->actorWidth = static_cast<int>(enemy->animation->actorWidth * 2.0f);
            enemy->animation->actorHeight = static_cast<int>(enemy->animation->actorHeight * 2.0f);
        }
        enemy->width = static_cast<int>(enemy->width * 2.0f);
        enemy->height = static_cast<int>(enemy->height * 2.0f);
        
        cout << "ShooterEnemy → SUPER ARTILLERY BOSS (Dispara 3x más rápido, rango +80%)" << endl;
    }
    
    cout << "Stats DESPUÉS - Vidas: " << enemy->lives << ", Daño: " << enemy->damage 
         << ", Velocidad: " << enemy->moveSpeed << ", Tamaño Visual: " << (enemy->animation ? enemy->animation->actorWidth : 0) 
         << "x" << (enemy->animation ? enemy->animation->actorHeight : 0) << endl;
    cout << "Tamaño Colisión: " << enemy->width << "x" << enemy->height << endl;
    cout << "Recompensa: " << enemy->coinReward << " monedas" << endl;
    cout << "¡SUPER ENEMIGO LISTO! ¡Mantiene su comportamiento original!" << endl;
    cout << "===============================" << endl;
}