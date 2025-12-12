#include "GameLayer.h"

GameLayer::GameLayer(Game* game)
	: Layer(game) {
	//llama al constructor del padre : Layer(renderer)
	
	// Inicializar sistema de niveles
	currentLevel = 1;
	totalLevels = 5;
	levelTime = 0.0f;
	levelCompleted = false;
	
	init();
}

void GameLayer::init() {
	space = new Space(0);
	scrollX = 0;
	scrollY = 0;
	audioBackground = Audio::createAudio("res/musica_ambiente.mp3", true);
	audioBackground->play();

	points = 0;
	
	// HUD superior derecha - PUNTOS (estrella + número) - Ajustado para verse completo
	backgroundPoints = new Actor("res/icono_puntos.png", WIDTH * 0.88, HEIGHT * 0.06, 32, 32, game);
	textPoints = new Text("0", WIDTH * 0.95, HEIGHT * 0.06, game);
	
	// HUD superior izquierda - NIVEL Y TIEMPO - Separado del borde
	textLevel = new Text("NIVEL: 1/5", WIDTH * 0.15, HEIGHT * 0.06, game);
	textTime = new Text("TIEMPO: 30", WIDTH * 0.15, HEIGHT * 0.13, game);
	
	// HUD inferior izquierda - VIDAS (corazón + número) - Tamaño correcto 44x36
	backgroundLives = new Actor("res/corazon.png", WIDTH * 0.07, HEIGHT * 0.93, 44, 36, game);
	lifePoints = new Text("0", WIDTH * 0.14, HEIGHT * 0.93, game);
	
	// HUD inferior centro-izquierda - DINERO (moneda + número) - Mejor espaciado
	backgroundMoney = new Actor("res/icono_moneda.png", WIDTH * 0.26, HEIGHT * 0.93, 28, 28, game);
	textMoney = new Text("0", WIDTH * 0.33, HEIGHT * 0.93, game);
	
	// HUD inferior derecha - DISPAROS - Ajustado para no salirse
	textShoots = new Text("Disparos: 0", WIDTH * 0.78, HEIGHT * 0.93, game);

	background = new Background("res/fondo.png", WIDTH * 0.5, HEIGHT * 0.5, -1, game);
	
	projectiles.clear(); // Vaciar por si reiniciamos el juego
	powerUps.clear();
	enemies.clear(); // Vaciar por si reiniciamos el juego
	lifeSpawners.clear(); // Vaciar spawners
	enemySpawners.clear(); // Vaciar spawners de enemigos
	enemyProjectiles.clear(); // Vaciar proyectiles enemigos
	
	// Reiniciar el sistema de niveles
	currentLevel = 1;
	levelTime = 0.0f;
	levelCompleted = false;
	levelDuration = getLevelDuration(currentLevel);

	loadMap("res/0.txt");  // ← player se crea aquí

	// Actualizar los textos con los valores iniciales del jugador
	lifePoints->content = to_string(player->lives);
	textMoney->content = to_string(player->money);
	textShoots->content = "Disparos: " + to_string(player->numberOfShoots);
	textPoints->content = to_string(points);
	
	// Actualizar texto de nivel
	textLevel->content = "NIVEL: " + to_string(currentLevel) + "/" + to_string(totalLevels);
}

void GameLayer::processControls() {
	// obtener controles
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		keysToControls(event);
	}
	// procesar controles
	// Disparar
	if (controlShoot) {
		Projectile* newProjectile = player->shoot();
		if (newProjectile != NULL) {
			space->addDynamicActor(newProjectile);
			projectiles.push_back(newProjectile);
		}
	}
	// Eje X
	if (controlMoveX > 0) {
		player->moveX(1);
	}
	else if (controlMoveX < 0) {
		player->moveX(-1);
	}
	else {
		player->moveX(0);
	}

	// Eje Y
	if (controlMoveY > 0) {
		player->moveY(1);
	}
	else if (controlMoveY < 0) {
		player->moveY(-1);
	}
	else {
		player->moveY(0);
	}


}

void GameLayer::keysToControls(SDL_Event event) {
	if (event.type == SDL_QUIT) {
		game->loopActive = false;
	}
	if (event.type == SDL_KEYDOWN) {
		int code = event.key.keysym.sym;
		// Pulsada
		switch (code) {
		case SDLK_ESCAPE:
			game->loopActive = false;
			break;
		case SDLK_f:
			game->scale();
			break;
		case SDLK_d: // derecha
			controlMoveX = 1;
			break;
		case SDLK_a: // izquierda
			controlMoveX = -1;
			break;
		case SDLK_w: // arriba
			controlMoveY = -1;
			break;
		case SDLK_s: // abajo
			controlMoveY = 1;
			break;
		case SDLK_SPACE: // dispara
			controlShoot = true;
			break;
		}
	}
	if (event.type == SDL_KEYUP) {
		int code = event.key.keysym.sym;
		// Levantada
		switch (code) {
		case SDLK_d: // derecha
			if (controlMoveX == 1) {
				controlMoveX = 0;
			}
			break;
		case SDLK_a: // izquierda
			if (controlMoveX == -1) {
				controlMoveX = 0;
			}
			break;
		case SDLK_w: // arriba
			if (controlMoveY == -1) {
				controlMoveY = 0;
			}
			break;
		case SDLK_s: // abajo
			if (controlMoveY == 1) {
				controlMoveY = 0;
			}
			break;
		case SDLK_SPACE: // dispara
			controlShoot = false;
			break;
		}
	}
}

void GameLayer::update() {
	// Actualizar el tiempo del nivel
	levelTime += 0.033f; // Aproximadamente 30 FPS (1/30 = 0.033)
	
	// Actualizar texto de tiempo
	float timeRemaining = levelDuration - levelTime;
	if (timeRemaining < 0) timeRemaining = 0;
	textTime->content = "TIEMPO: " + to_string((int)timeRemaining);
	
	// Verificar si se completó el nivel (tiempo cumplido y jugador vivo)
	if (!levelCompleted && levelTime >= levelDuration) {
		levelCompleted = true;
		nextLevel();
		return; // Salir para evitar procesar el resto del frame
	}
	
	background->update();
	
	// Actualizar textos del HUD
	lifePoints->content = to_string(player->lives);
	textMoney->content = to_string(player->money);
	textShoots->content = "Disparos: " + to_string(player->numberOfShoots);

	player->update();
	
	// Calcular scroll para usarlo en la lógica de enemigos
	calculateScroll();
	
	// Actualizar enemigos según su tipo ANTES de space->update()
	for (auto const& enemy : enemies) {
		// Identificar tipo de enemigo y actualizar comportamiento
		BasicEnemy* basicEnemy = dynamic_cast<BasicEnemy*>(enemy);
		if (basicEnemy != nullptr) {
			basicEnemy->moveTowardsPlayer(player);
		}
		
		ChargeEnemy* chargeEnemy = dynamic_cast<ChargeEnemy*>(enemy);
		if (chargeEnemy != nullptr) {
			chargeEnemy->checkAndCharge(player);
		}
		
		ShooterEnemy* shooterEnemy = dynamic_cast<ShooterEnemy*>(enemy);
		if (shooterEnemy != nullptr) {
			// Usar la versión con scroll para detectar visibilidad
			shooterEnemy->maintainDistance(player, scrollX, scrollY);
			// Intentar disparar
			EnemyProjectile* enemyProj = shooterEnemy->shoot(player);
			if (enemyProj != nullptr) {
				enemyProjectiles.push_back(enemyProj);
				space->addDynamicActor(enemyProj);
			}
		}
		
		enemy->update();
	}
	
	// NUEVO: Procesar rebotes de proyectiles enemigos ANTES de space->update()
	// Los proyectiles enemigos NO deben ser bloqueados por Space, deben rebotar
	list<EnemyProjectile*> deleteEnemyProjectilesBounce;
	
	// Remover temporalmente proyectiles enemigos de Space para manejarlos manualmente
	for (auto const& enemyProj : enemyProjectiles) {
		space->removeDynamicActor(enemyProj);
	}
	
	// IMPORTANTE: Actualizar space DESPUÉS de establecer las velocidades
	space->update();
	
	// Ahora procesar proyectiles enemigos con rebotes
	for (auto const& enemyProj : enemyProjectiles) {
		// Guardar posición y velocidad anterior
		float oldX = enemyProj->x;
		float oldY = enemyProj->y;
		float oldVx = enemyProj->vx;
		float oldVy = enemyProj->vy;
		
		// Aplicar movimiento manualmente
		float newX = oldX + enemyProj->vx;
		float newY = oldY + enemyProj->vy;
		
		// Probar nueva posición
		enemyProj->x = newX;
		enemyProj->y = newY;
		
		bool collidesHorizontal = false;
		bool collidesVertical = false;
		
		// Verificar si hay colisión en la nueva posición
		if (space->checkCollisionDirection(enemyProj, collidesHorizontal, collidesVertical)) {
			// Si alcanzó el máximo de rebotes, marcarlo para eliminar
			if (enemyProj->currentBounces >= enemyProj->maxBounces) {
				enemyProj->x = oldX;
				enemyProj->y = oldY;
				deleteEnemyProjectilesBounce.push_back(enemyProj);
				cout << "Proyectil alcanzó máximo de rebotes (" << enemyProj->maxBounces << ") - Eliminando" << endl;
			} else {
				// Hacer rebotar según la dirección de colisión
				bool didBounce = false;
				
				if (collidesHorizontal) {
					enemyProj->bounce(true); // Rebote horizontal (invierte vx)
					didBounce = true;
				}
				if (collidesVertical) {
					enemyProj->bounce(false); // Rebote vertical (invierte vy)
					didBounce = true;
				}
				
				// Si rebotó en ambos ejes simultáneamente, el contador se incrementa dos veces
				// Compensar para que solo cuente como un rebote
				if (collidesHorizontal && collidesVertical && didBounce) {
					enemyProj->currentBounces--;
				}
				
				// Calcular nueva posición con velocidades invertidas
				// Si colisiona en X, mantener Y original + movimiento en Y
				// Si colisiona en Y, mantener X original + movimiento en X
				if (collidesHorizontal && !collidesVertical) {
					// Rebote horizontal: X vuelve a posición original, Y se mueve normalmente
					enemyProj->x = oldX;
					enemyProj->y = oldY + enemyProj->vy;
				} else if (collidesVertical && !collidesHorizontal) {
					// Rebote vertical: Y vuelve a posición original, X se mueve normalmente
					enemyProj->x = oldX + enemyProj->vx;
					enemyProj->y = oldY;
				} else {
					// Rebote en esquina: ambos ejes vuelven a posición original
					enemyProj->x = oldX;
					enemyProj->y = oldY;
				}
			}
		}
		// Si no hay colisión, la nueva posición ya está aplicada
	}
	
	// Eliminar proyectiles que alcanzaron el límite de rebotes
	for (auto const& delProj : deleteEnemyProjectilesBounce) {
		enemyProjectiles.remove(delProj);
		delete delProj;
	}
	
	for (auto const& projectile : projectiles) {
		projectile->update();
	}
	
	for (auto const& enemyProj : enemyProjectiles) {
		enemyProj->update();
	}
	
	for (auto const& powerUp : powerUps) {
		powerUp->update();
	}
	
	// Actualizar spawners y generar vidas
	for (auto const& spawner : lifeSpawners) {
		spawner->update();
		if (spawner->shouldSpawn()) {
			// Crear un power-up de vida en la posición del spawner
			LifesPowerUp* lifePowerUp = new LifesPowerUp(spawner->x, spawner->y, game);
			powerUps.push_back(lifePowerUp);
			space->addDynamicActor(lifePowerUp);
			spawner->resetTimer();
			cout << "Spawner generó un corazón en posición (" << spawner->x << ", " << spawner->y << ")" << endl;
		}
	}
	
	// Actualizar spawners de enemigos y generar enemigos
	for (auto const& spawner : enemySpawners) {
		spawner->update();
		if (spawner->shouldSpawn()) {
			Enemy* newEnemy = nullptr;
			
			// Crear el tipo de enemigo correspondiente
			if (spawner->enemyType == "B") {
				BasicEnemy* basicEnemy = new BasicEnemy(spawner->x, spawner->y, game);
				newEnemy = basicEnemy;
			}
			else if (spawner->enemyType == "R") {
				ChargeEnemy* chargeEnemy = new ChargeEnemy(spawner->x, spawner->y, game);
				newEnemy = chargeEnemy;
			}
			else if (spawner->enemyType == "T") {
				ShooterEnemy* shooterEnemy = new ShooterEnemy(spawner->x, spawner->y, game);
				newEnemy = shooterEnemy;
			}
			
			if (newEnemy != nullptr) {
				enemies.push_back(newEnemy);
				space->addDynamicActor(newEnemy);
				spawner->currentEnemies++;
				spawner->resetTimer();
				cout << "Spawner generó enemigo tipo " << spawner->enemyType 
				     << " en posición (" << spawner->x << ", " << spawner->y << ")" << endl;
			}
		}
	}

	// Colisiones , Enemy - Projectile, Player - PowerUp

	list<Enemy*> deleteEnemies;
	list<Projectile*> deleteProjectiles;
	list<PowerUp*> deletePowerUps;
	list<EnemyProjectile*> deleteEnemyProjectiles;

	// Colisiones
	for (auto const& enemy : enemies) {
		if (player->isOverlap(enemy)) {
			player->lives--;
			bool eInList = std::find(deleteEnemies.begin(),
				deleteEnemies.end(),
				enemy) != deleteEnemies.end();
			if (!eInList) {
				deleteEnemies.push_back(enemy);
			}
			if (player->lives == 0) {
				game->layer = game->gameOverLayer;
				game->layer->init();
				return;
			}
		}
	}
	
	// Colisión proyectiles enemigos con jugador
	for (auto const& enemyProj : enemyProjectiles) {
		if (player->isOverlap(enemyProj)) {
			player->lives--;
			bool pInList = std::find(deleteEnemyProjectiles.begin(),
				deleteEnemyProjectiles.end(),
				enemyProj) != deleteEnemyProjectiles.end();
			if (!pInList) {
				deleteEnemyProjectiles.push_back(enemyProj);
			}
			if (player->lives == 0) {
				game->layer = game->gameOverLayer;
				game->layer->init();
				return;
			}
		}
	}

	for (auto const& powerUp : powerUps) {
		if (player->isOverlap(powerUp)) {
			cout << "¡Colision con power-up detectada!" << endl;
			powerUp->effect(player);
			bool pInList = std::find(deletePowerUps.begin(),
				deletePowerUps.end(),
				powerUp) != deletePowerUps.end();
			if (!pInList) {
				deletePowerUps.push_back(powerUp);
			}
		}
	}

	for (auto const& projectile : projectiles) {
		if (projectile->isInRender(scrollX, scrollY) == false) {

			bool pInList = std::find(deleteProjectiles.begin(),
				deleteProjectiles.end(),
				projectile) != deleteProjectiles.end();

			if (!pInList) {
				deleteProjectiles.push_back(projectile);
			}
		}
	}
	
	// NOTA: Los proyectiles enemigos NO se eliminan cuando salen de pantalla
	// Solo se eliminan al golpear al jugador o al alcanzar el máximo de rebotes (6)
	// Ya no hay código aquí que los elimine por salir de pantalla

	for (auto const& powerUp : powerUps) {
		if (powerUp->isInRender(scrollX, scrollY) == false && powerUp->x <= 0) {

			bool pInList = std::find(deletePowerUps.begin(),
				deletePowerUps.end(),
				powerUp) != deletePowerUps.end();

			if (!pInList) {
				deletePowerUps.push_back(powerUp);
			}
		}
	}

	for (auto const& enemy : enemies) {
		for (auto const& projectile : projectiles) {
			if (enemy->isOverlap(projectile)) {
				enemy->lives--;

				bool pInList = std::find(deleteProjectiles.begin(),
					deleteProjectiles.end(),
					projectile) != deleteProjectiles.end();
				if (!pInList) {
					deleteProjectiles.push_back(projectile);
				}
				if (enemy->lives == 0) {
					bool eInList = std::find(deleteEnemies.begin(),
						deleteEnemies.end(),
						enemy) != deleteEnemies.end();
					if (!eInList) {
						deleteEnemies.push_back(enemy);
						// Crear moneda cuando muere el enemigo
						CoinPowerUp* coin = new CoinPowerUp(enemy->x, enemy->y, game);
						powerUps.push_back(coin);
						space->addDynamicActor(coin);
					}
					player->numberOfShoots++;
					points++;
					textPoints->content = to_string(points);
				}
			}
		}
	}

	for (auto const& delEnemy : deleteEnemies) {
		// Actualizar contadores de spawners
		for (auto const& spawner : enemySpawners) {
			if (spawner->currentEnemies > 0) {
				spawner->currentEnemies--;
			}
		}
		enemies.remove(delEnemy);
	}
	deleteEnemies.clear();

	for (auto const& delProjectile : deleteProjectiles) {
		projectiles.remove(delProjectile);
		space->removeDynamicActor(delProjectile);
		delete delProjectile;
	}
	deleteProjectiles.clear();

	for (auto const& delPowerUp : deletePowerUps) {
		powerUps.remove(delPowerUp);
		space->removeDynamicActor(delPowerUp);
		delete delPowerUp;
	}
	deletePowerUps.clear();
	
	// Eliminar proyectiles enemigos marcados
	for (auto const& delEnemyProj : deleteEnemyProjectiles) {
		enemyProjectiles.remove(delEnemyProj);
		delete delEnemyProj;
	}
	deleteEnemyProjectiles.clear();

	std::cout << "update GameLayer" << endl;
}

void GameLayer::draw() {
	calculateScroll();
	
	// 1. FONDO (más atrás)
	background->draw();

	// 2. TILES/SUELO
	for (auto const& tile : tiles) {
		tile->draw(scrollX, scrollY);
	}

	// 3. SPAWNERS (debajo de enemigos para que no los tapen)
	for (auto const& spawner : lifeSpawners) {
		spawner->draw(scrollX, scrollY);
	}
	for (auto const& spawner : enemySpawners) {
		spawner->draw(scrollX, scrollY);
	}

	// 4. POWER-UPS (en el suelo)
	for (auto const& powerUp : powerUps) {
		powerUp->draw(scrollX, scrollY);
	}

	// 5. PROYECTILES (sobre el suelo, debajo de personajes)
	for (auto const& projectile : projectiles) {
		projectile->draw(scrollX, scrollY);
	}
	for (auto const& enemyProj : enemyProjectiles) {
		enemyProj->draw(scrollX, scrollY);
	}

	// 6. ENEMIGOS (encima de spawners y proyectiles)
	for (auto const& enemy : enemies) {
		enemy->draw(scrollX, scrollY);
	}

	// 7. JUGADOR (encima de enemigos)
	player->draw(scrollX, scrollY);
	
	// 8. HUD (siempre visible encima de todo, sin scroll)
	// Los elementos del HUD se dibujan sin scroll para que permanezcan fijos en pantalla
	backgroundPoints->draw(0, 0);
	backgroundLives->draw(0, 0);
	backgroundMoney->draw(0, 0);
	textPoints->draw(0, 0);
	lifePoints->draw(0, 0);
	textShoots->draw(0, 0);
	textMoney->draw(0, 0);
	textLevel->draw(0, 0);
	textTime->draw(0, 0);

	SDL_RenderPresent(game->renderer); // Renderiza
}


void GameLayer::loadMap(string name) {
	char character;
	string line;
	ifstream streamFile(name.c_str());
	if (!streamFile.is_open()) {
		cout << "Falla abrir el fichero de mapa" << endl;
		return;
	}
	else {
		// Por línea
		for (int i = 0; getline(streamFile, line); i++) {
			istringstream streamLine(line);
			mapWidth = line.length() * 32; // Ancho del mapa en pixels
			// Por carácter (en cada línea)
			for (int j = 0; j<line.length(); j++) {
				streamLine >> character; // Leer character 
				cout << character;
				float x = 32 / 2 + j * 32; // x central
				float y = 32 + i * 32; // y suelo
				loadMapObject(character, x, y);
			}

			cout << character << endl;
		}
	}
	streamFile.close();
}


void GameLayer::loadMapObject(char character, float x, float y)
{
	switch (character) {
	case 'P': {
		player = new Player(x, y, game);
		// modificación para empezar a contar desde el suelo.
		player->y = player->y - player->height / 2;
		space->addDynamicActor(player);
		Tile* tile = new Tile("res/Tierra1.png", x, y, game);
		// modificación para empezar a contar desde el suelo.
		tile->y = tile->y - tile->height / 2;
		tiles.push_back(tile);
		// NO agregar al space - no debe colisionar
		break;
	}
	case '#': {
		static std::vector<const char*> names = {
		"res/Tierra1.png",
		"res/Tierra2.png",
		"res/Tierra3.png",
		"res/Tierra4.png"
		};

		static std::vector<double> weights = { 0.055, 0.005, 0.5, 0.44 };

		static std::random_device rd;
		static std::mt19937 gen(rd());
		static std::discrete_distribution<> dist(weights.begin(), weights.end());

		const char* selected = names[dist(gen)];
		Tile* tile = new Tile(selected, x, y, game);
		// modificación para empezar a contar desde el suelo.
		tile->y = tile->y - tile->height / 2;
		tiles.push_back(tile);
		// NO agregar al space - no debe colisionar
		break;
	}
	case 'B': {
		// BasicEnemy - Persigue al jugador directamente
		BasicEnemy* basicEnemy = new BasicEnemy(x, y, game);
		basicEnemy->y = basicEnemy->y - basicEnemy->height / 2;
		enemies.push_back(basicEnemy);
		space->addDynamicActor(basicEnemy);
		Tile* tile = new Tile("res/Tierra1.png", x, y, game);
		// modificación para empezar a contar desde el suelo.
		tile->y = tile->y - tile->height / 2;
		tiles.push_back(tile);
		// NO agregar al space - no debe colisionar
		break;
	}
	case 'R': {
		// ChargeEnemy - Embiste cuando está alineado (R = Ram/Ramming)
		ChargeEnemy* chargeEnemy = new ChargeEnemy(x, y, game);
		chargeEnemy->y = chargeEnemy->y - chargeEnemy->height / 2;
		enemies.push_back(chargeEnemy);
		space->addDynamicActor(chargeEnemy);
		Tile* tile = new Tile("res/Tierra1.png", x, y, game);
		// modificación para empezar a contar desde el suelo.
		tile->y = tile->y - tile->height / 2;
		tiles.push_back(tile);
		// NO agregar al space - no debe colisionar
		break;
	}
	case 'T': {
		// ShooterEnemy - Dispara desde la distancia (T = Turret/Tirador)
		ShooterEnemy* shooterEnemy = new ShooterEnemy(x, y, game);
		shooterEnemy->y = shooterEnemy->y - shooterEnemy->height / 2;
		enemies.push_back(shooterEnemy);
		space->addDynamicActor(shooterEnemy);
		Tile* tile = new Tile("res/Tierra1.png", x, y, game);
		// modificación para empezar a contar desde el suelo.
		tile->y = tile->y - tile->height / 2;
		tiles.push_back(tile);
		// NO agregar al space - no debe colisionar
		break;
	}
	case '0': {
		//"Límite del mapa"
		Tile* tile = new Tile("res/BordeTL.png", x, y, game);
		// modificación para empezar a contar desde el suelo.
		tile->y = tile->y - tile->height / 2;
		tiles.push_back(tile);
		space->addStaticActor(tile);
		break;
	}
	case '1': {
		//"Límite del mapa"
		Tile* tile = new Tile("res/BordeT.png", x, y, game);
		// modificación para empezar a contar desde el suelo.
		tile->y = tile->y - tile->height / 2;
		tiles.push_back(tile);
		space->addStaticActor(tile);
		break;
	}
	case '2': {
		//"Límite del mapa"
		Tile* tile = new Tile("res/BordeTR.png", x, y, game);
		// modificación para empezar a contar desde el suelo.
		tile->y = tile->y - tile->height / 2;
		tiles.push_back(tile);
		space->addStaticActor(tile);
		break;
	}
	case '3': {
		//"Límite del mapa"
		Tile* tile = new Tile("res/BordeL.png", x, y, game);
		// modificación para empezar a contar desde el suelo.
		tile->y = tile->y - tile->height / 2;
		tiles.push_back(tile);
		space->addStaticActor(tile);
		break;
	}
	case '4': {
		//"Límite del mapa"
		Tile* tile = new Tile("res/BordeR.png", x, y, game);
		// modificación para empezar a contar desde el suelo.
		tile->y = tile->y - tile->height / 2;
		tiles.push_back(tile);
		space->addStaticActor(tile);
		break;
	}
	case '5': {
		//"Límite del mapa"
		Tile* tile = new Tile("res/BordeBL.png", x, y, game);
		// modificación para empezar a contar desde el suelo.
		tile->y = tile->y - tile->height / 2;
		tiles.push_back(tile);
		space->addStaticActor(tile);
		break;
	}
	case '6': {
		//"Límite del mapa"
		Tile* tile = new Tile("res/BordeB.png", x, y, game);
		// modificación para empezar a contar desde el suelo.
		tile->y = tile->y - tile->height / 2;
		tiles.push_back(tile);
		space->addStaticActor(tile);
		break;
	}
	case '7': {
		//"Límite del mapa"
		Tile* tile = new Tile("res/BordeBR.png", x, y, game);
		// modificación para empezar a contar desde el suelo.
		tile->y = tile->y - tile->height / 2;
		tiles.push_back(tile);
		space->addStaticActor(tile);
		break;
	}
	case 'H': {
		// Power-up de vida (Heart)
		LifesPowerUp* lifePowerUp = new LifesPowerUp(x, y, game);
		lifePowerUp->y = lifePowerUp->y - lifePowerUp->height / 2;
		powerUps.push_back(lifePowerUp);
		space->addDynamicActor(lifePowerUp);
		break;
	}
	case 'S': {
		// Power-up de disparos (Shoots)
		ShootPowerUp* shootPowerUp = new ShootPowerUp(x, y, game);
		shootPowerUp->y = shootPowerUp->y - shootPowerUp->height / 2;
		powerUps.push_back(shootPowerUp);
		space->addDynamicActor(shootPowerUp);
		break;
	}
	case 'C': {
		// Power-up de moneda (Coin)
		CoinPowerUp* coin = new CoinPowerUp(x, y, game);
		coin->y = coin->y - coin->height / 2;
		powerUps.push_back(coin);
		space->addDynamicActor(coin);
		break;
	}
	case 'V': {
		// Spawner de vidas (V = Vida spawner)
		LifeSpawner* spawner = new LifeSpawner(x, y, game);
		spawner->y = spawner->y - spawner->height / 2;
		lifeSpawners.push_back(spawner);
		// NO añadir al space - los spawners no colisionan
		cout << "Spawner de vida creado en (" << x << ", " << y << ")" << endl;
		break;
	}
	case 'b': {
		// Spawner de BasicEnemy (minúscula 'b')
		EnemySpawner* spawner = new EnemySpawner(x, y, "B", game);
		spawner->y = spawner->y - spawner->height / 2;
		enemySpawners.push_back(spawner);
		cout << "Spawner de BasicEnemy creado en (" << x << ", " << y << ")" << endl;
		break;
	}
	case 'r': {
		// Spawner de ChargeEnemy (minúscula 'r')
		EnemySpawner* spawner = new EnemySpawner(x, y, "R", game);
		spawner->y = spawner->y - spawner->height / 2;
		enemySpawners.push_back(spawner);
		cout << "Spawner de ChargeEnemy creado en (" << x << ", " << y << ")" << endl;
		break;
	}
	case 't': {
		// Spawner de ShooterEnemy (minúscula 't')
		EnemySpawner* spawner = new EnemySpawner(x, y, "T", game);
		spawner->y = spawner->y - spawner->height / 2;
		enemySpawners.push_back(spawner);
		cout << "Spawner de ShooterEnemy creado en (" << x << ", " << y << ")" << endl;
		break;
	}
	}
}

void GameLayer::calculateScroll() {
	// Centrar el jugador en la pantalla
	// Para 1280x720: centro X = 640, centro Y = 360
	scrollX = player->x - WIDTH / 2;
	scrollY = player->y - HEIGHT / 2;
}

float GameLayer::getLevelDuration(int level) {
	switch(level) {
		case 1: return 30.0f;  // 30 segundos
		case 2: return 45.0f;  // 45 segundos
		case 3: return 60.0f;  // 60 segundos
		case 4: return 75.0f;  // 75 segundos
		case 5: return 90.0f;  // 90 segundos
		default: return 60.0f;
	}
}

void GameLayer::loadLevel(int level) {
	currentLevel = level;
	levelTime = 0.0f;
	levelCompleted = false;
	levelDuration = getLevelDuration(level);
	
	// Limpiar nivel anterior
	clearLevel();
	
	// Cargar el mapa correspondiente al nivel
	string mapName = "res/" + to_string(level - 1) + ".txt";
	loadMap(mapName);
	
	// Actualizar texto de nivel
	textLevel->content = "NIVEL: " + to_string(currentLevel) + "/" + to_string(totalLevels);
	
	cout << "Nivel " << currentLevel << " cargado. Duracion: " << levelDuration << " segundos." << endl;
}

void GameLayer::clearLevel() {
	// Limpiar enemigos
	for (auto const& enemy : enemies) {
		space->removeDynamicActor(enemy);
		delete enemy;
	}
	enemies.clear();
	
	// Limpiar proyectiles
	for (auto const& projectile : projectiles) {
		space->removeDynamicActor(projectile);
		delete projectile;
	}
	projectiles.clear();
	
	// Limpiar proyectiles enemigos
	for (auto const& enemyProj : enemyProjectiles) {
		space->removeDynamicActor(enemyProj);
		delete enemyProj;
	}
	enemyProjectiles.clear();
	
	// Limpiar power-ups
	for (auto const& powerUp : powerUps) {
		space->removeDynamicActor(powerUp);
		delete powerUp;
	}
	powerUps.clear();
	
	// Limpiar spawners
	for (auto const& spawner : lifeSpawners) {
		delete spawner;
	}
	lifeSpawners.clear();
	
	// Limpiar spawners de enemigos
	for (auto const& spawner : enemySpawners) {
		delete spawner;
	}
	enemySpawners.clear();
	
	// Limpiar tiles
	for (auto const& tile : tiles) {
		space->removeStaticActor(tile);
		delete tile;
	}
	tiles.clear();
}

void GameLayer::nextLevel() {
	if (currentLevel < totalLevels) {
		currentLevel++;
		loadLevel(currentLevel);
		cout << "¡Nivel completado! Pasando al nivel " << currentLevel << endl;
	} else {
		// Juego completado
		cout << "¡Has completado todos los niveles!" << endl;
		// Puedes reiniciar el juego o mostrar pantalla de victoria
		game->layer = game->inicioLayer;
	}
}
