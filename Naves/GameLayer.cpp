#include "GameLayer.h"

GameLayer::GameLayer(Game* game)
	: Layer(game) {
	//llama al constructor del padre : Layer(renderer)
	init();
}

void GameLayer::init() {
	audioBackground = Audio::createAudio("res/musica_ambiente.mp3", true);
	audioBackground->play();

	points = 0;
	textPoints = new Text("hola", WIDTH * 0.92, 
		HEIGHT *0.05, game);
	textPoints->content = to_string(points);
	player = new Player(50 , HEIGHT/2, game);
	lifePoints = new Text("adios", WIDTH * 0.15,
		HEIGHT * 0.05, game);
	lifePoints->content = to_string(player->lives);
	textShoots = new Text("0", player->x, player->y + 20, game);
	background = new Background("res/fondo.png", WIDTH * 0.5, HEIGHT * 0.5, -1, game);
	backgroundPoints = new Actor("res/icono_puntos.png", WIDTH * 0.85,
		HEIGHT * 0.05, 24, 24, game);
	backgroundLives = new Actor("res/corazon.png", WIDTH * 0.08,
		HEIGHT * 0.06, 44, 36, game);
	projectiles.clear(); // Vaciar por si reiniciamos el juego
	powerUps.clear();
	enemies.clear(); // Vaciar por si reiniciamos el juego
	enemies.push_back(new BasicEnemy(300, 50, game));
	enemies.push_back(new BasicEnemy(450, 200, game));
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
	background->update();
	lifePoints->content = to_string(player->lives);
	// Generar enemigos
	newBasicEnemyTime--;
	if (newBasicEnemyTime <= 0) {
		int rX = (rand() % (600 - 500)) + 1 + 500;
		int rY = (rand() % (300 - 60)) + 1 + 60;
		enemies.push_back(new BasicEnemy(rX, rY, game));
		newBasicEnemyTime = 120;
	}
	newSMovingEnemyTime--;
	if (newSMovingEnemyTime <= 0) {
		int rX = (rand() % (600 - 500)) + 1 + 500;
		int rY = (rand() % (300 - 60)) + 1 + 60;
		enemies.push_back(new SMovingEnemy(rX, rY, game));
		newSMovingEnemyTime = 180;
	}

	//Generar PowerUps
	if (newLifePowerUpTime > 0)	newLifePowerUpTime--;
	if (newLifePowerUpTime <= 0 && player->lives < 3) {
		int rX = (rand() % (600 - 500)) + 1 + 500;
		int rY = (rand() % (300 - 60)) + 1 + 60;
		powerUps.push_back(new LifesPowerUp(rX, rY, game));
		newLifePowerUpTime = 1200;
	}
	if (newShootPowerUptime > 0) newShootPowerUptime--;
	if (newShootPowerUptime <= 0 && player->numberOfShoots < 10) {
		int rX = (rand() % (600 - 500)) + 1 + 500;
		int rY = (rand() % (300 - 60)) + 1 + 60;
		powerUps.push_back(new ShootPowerUp(rX, rY, game));
		newShootPowerUptime = 600;
	}

	player->update();
	textShoots->x = player->x;
	textShoots->y = player->y + 20;
	textShoots->content = to_string(player->numberOfShoots);
	for (auto const& enemy : enemies) {
		enemy->update();
	}
	for (auto const& projectile : projectiles) {
		projectile->update();
	}
	for (auto const& powerUp : powerUps) {
		powerUp->update();
	}

	// Colisiones , Enemy - Projectile, Player - PowerUp

	list<Enemy*> deleteEnemies;
	list<Projectile*> deleteProjectiles;
	list<PowerUp*> deletePowerUps;

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
				init();
				return;
			}
		}
	}

	for (auto const& powerUp : powerUps) {
		if (player->isOverlap(powerUp)) {
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
		if (projectile->isInRender() == false) {

			bool pInList = std::find(deleteProjectiles.begin(),
				deleteProjectiles.end(),
				projectile) != deleteProjectiles.end();

			if (!pInList) {
				deleteProjectiles.push_back(projectile);
			}
		}
	}

	for (auto const& powerUp : powerUps) {
		if (powerUp->isInRender() == false && powerUp->x <= 0) {

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
					}
					player->numberOfShoots++;
					points++;
					textPoints->content = to_string(points);
				}
			}
		}
	}

	for (auto const& delEnemy : deleteEnemies) {
		enemies.remove(delEnemy);
	}
	deleteEnemies.clear();

	for (auto const& delProjectile : deleteProjectiles) {
		projectiles.remove(delProjectile);
		delete delProjectile;
	}
	deleteProjectiles.clear();

	for (auto const& delPowerUp : deletePowerUps) {
		powerUps.remove(delPowerUp);
	}
	deletePowerUps.clear();

	std::cout << "update GameLayer" << endl;
}

void GameLayer::draw() {
	background->draw();

	for (auto const& projectile : projectiles) {
		projectile->draw();
	}

	player->draw();

	for (auto const& enemy : enemies) {
		enemy->draw();
	}

	for (auto const& powerUp : powerUps) {
		powerUp->draw();
	}
	textPoints->draw();
	lifePoints->draw();
	textShoots->draw();
	backgroundPoints->draw();
	backgroundLives->draw();

	SDL_RenderPresent(game->renderer); // Renderiza
}

