#include "ShopLayer.h"
#include "GameLayer.h"
#include <iostream>
using namespace std;

ShopLayer::ShopLayer(Game* game) : Layer(game) {
	player = nullptr;
	selectedItemIndex = 0;
	controlUp = false;
	controlDown = false;
	controlBuy = false;
	controlExit = false;
	
	// Inicializar variables de transición de nivel
	nextLevelToLoad = -1;
	shouldLoadNextLevel = false;
	
	init();
}

ShopLayer::~ShopLayer() {
	delete background;
	delete titleText;
	delete moneyText;
	delete instructionsText;
	delete exitText;
	delete selector;
	
	for (auto item : shopItems) {
		delete item;
	}
	shopItems.clear();
}

void ShopLayer::init() {
	// Fondo de la tienda - usar fondo existente
	background = new Background("res/Tienda.png", WIDTH * 0.5, HEIGHT * 0.5, -1, game);
	
	// Título
	titleText = new Text("=== TIENDA DE MEJORAS ===", WIDTH * 0.5 - 150, HEIGHT * 0.1, game);
	
	// Dinero disponible
	moneyText = new Text("Dinero: 0 monedas", WIDTH * 0.5 - 100, HEIGHT * 0.17, game);
	
	// Instrucciones
	instructionsText = new Text("W/S: Navegar | ESPACIO: Comprar | ESC: Continuar", 
	                            WIDTH * 0.5 - 260, HEIGHT * 0.9, game);
	
	// Texto de salida
	exitText = new Text("Presiona ESC para continuar al siguiente nivel", 
	                    WIDTH * 0.5 - 220, HEIGHT * 0.95, game);
	
	// Crear selector visual
	selector = new Actor("res/AlienSelector.png", 0, 0, 64, 64, game);
	
	// Limpiar items existentes
	for (auto item : shopItems) {
		delete item;
	}
	shopItems.clear();
	
	// Crear items de la tienda
	float startY = HEIGHT * 0.30;
	float itemSpacing = 105;
	float centerX = WIDTH * 0.5;
	
	shopItems.push_back(new ShopItem(ItemType::HEALTH_UPGRADE, 50, centerX, startY, game));
	shopItems.push_back(new ShopItem(ItemType::DAMAGE_UPGRADE, 75, centerX, startY + itemSpacing, game));
	shopItems.push_back(new ShopItem(ItemType::SPEED_UPGRADE, 60, centerX, startY + itemSpacing * 2, game));
	shopItems.push_back(new ShopItem(ItemType::MAX_AMMO_UPGRADE, 40, centerX, startY + itemSpacing * 3, game));
	
	selectedItemIndex = 0;
}

void ShopLayer::setPlayer(Player* player) {
	if (player != nullptr) {
		this->player = player;
		updateMoneyDisplay();
		cout << "ShopLayer: Player asignado correctamente" << endl;
	} else {
		cout << "WARNING: Intentando asignar player nullptr a ShopLayer" << endl;
	}
}

void ShopLayer::setNextLevel(int level) {
	nextLevelToLoad = level;
	shouldLoadNextLevel = true;
	cout << "Tienda configurada para cargar nivel " << level << " al salir" << endl;
}

void ShopLayer::updateMoneyDisplay() {
	if (player != nullptr) {
		moneyText->content = "Dinero: " + to_string(player->money) + " monedas";
	}
}

void ShopLayer::selectNextItem() {
	selectedItemIndex++;
	if (selectedItemIndex >= shopItems.size()) {
		selectedItemIndex = 0;
	}
	cout << "Item seleccionado: " << selectedItemIndex << endl;
}

void ShopLayer::selectPreviousItem() {
	selectedItemIndex--;
	if (selectedItemIndex < 0) {
		selectedItemIndex = shopItems.size() - 1;
	}
	cout << "Item seleccionado: " << selectedItemIndex << endl;
}

void ShopLayer::buySelectedItem() {
	if (player == nullptr || selectedItemIndex < 0 || selectedItemIndex >= shopItems.size()) {
		return;
	}
	
	ShopItem* item = shopItems[selectedItemIndex];
	
	if (item->purchase(player)) {
		cout << "¡Compra exitosa! Dinero restante: " << player->money << endl;
		updateMoneyDisplay();
	} else {
		cout << "¡Compra fallida! No tienes suficiente dinero o nivel maximo." << endl;
	}
}

void ShopLayer::processControls() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		keysToControls(event);
	}
	
	// Navegar hacia abajo
	if (controlDown) {
		selectNextItem();
		controlDown = false; // Reset para evitar repetición
	}
	
	// Navegar hacia arriba
	if (controlUp) {
		selectPreviousItem();
		controlUp = false; // Reset para evitar repetición
	}
	
	// Comprar item
	if (controlBuy) {
		buySelectedItem();
		controlBuy = false; // Reset para evitar repetición
	}
	
	// Salir de la tienda
	if (controlExit) {
		// Verificar si debe cargar el siguiente nivel
		if (shouldLoadNextLevel && nextLevelToLoad > 0) {
			cout << "Saliendo de tienda y cargando nivel " << nextLevelToLoad << endl;
			// IMPORTANTE: Guardar el estado del jugador DESPUÉS de las compras
			GameLayer* gameLayer = dynamic_cast<GameLayer*>(game->gameLayer);
			if (gameLayer != nullptr && player != nullptr) {
				// Guardar estado actualizado con las mejoras de la tienda
				gameLayer->savedPlayerMoney = player->money;
				gameLayer->savedPlayerLives = player->lives;
				gameLayer->savedPlayerMaxLives = player->maxLives;
				gameLayer->savedPlayerDamage = player->damage;
				gameLayer->savedPlayerShoots = player->numberOfShoots;
				gameLayer->savedPlayerMoveSpeed = player->moveSpeed;
				gameLayer->hasPlayerData = true;
				cout << "Estado post-tienda guardado - Dinero: " << player->money << ", Vidas: " << player->lives << ", DISPAROS: " << player->numberOfShoots << endl;
				
				// Ahora cargar el siguiente nivel
				game->layer = game->gameLayer;
				gameLayer->loadLevel(nextLevelToLoad);
			}
			// Reset de variables
			nextLevelToLoad = -1;
			shouldLoadNextLevel = false;
		} else {
			// Volver al juego normal
			game->layer = game->gameLayer;
		}
		controlExit = false;
	}
}

void ShopLayer::keysToControls(SDL_Event event) {
	if (event.type == SDL_QUIT) {
		game->loopActive = false;
	}
	
	if (event.type == SDL_KEYDOWN) {
		int code = event.key.keysym.sym;
		
		switch (code) {
			case SDLK_ESCAPE:
				controlExit = true;
				break;
			case SDLK_w:
			case SDLK_UP:
				controlUp = true;
				break;
			case SDLK_s:
			case SDLK_DOWN:
				controlDown = true;
				break;
			case SDLK_SPACE:
			case SDLK_RETURN:
				controlBuy = true;
				break;
		}
	}
	
	if (event.type == SDL_KEYUP) {
		int code = event.key.keysym.sym;
		
		switch (code) {
			case SDLK_ESCAPE:
				controlExit = false;
				break;
			case SDLK_w:
			case SDLK_UP:
				controlUp = false;
				break;
			case SDLK_s:
			case SDLK_DOWN:
				controlDown = false;
				break;
			case SDLK_SPACE:
			case SDLK_RETURN:
				controlBuy = false;
				break;
		}
	}
}

void ShopLayer::update() {
	updateMoneyDisplay();
}

void ShopLayer::draw() {
	// Dibujar fondo
	background->draw();
	
	// Dibujar título y textos informativos
	titleText->draw();
	moneyText->draw();
	instructionsText->draw();
	exitText->draw();
	
	// Dibujar items de la tienda
	for (int i = 0; i < shopItems.size(); i++) {
		ShopItem* item = shopItems[i];
		item->draw(0);
		
		// Dibujar selector en el item seleccionado
		if (i == selectedItemIndex) {
			selector->x = item->x - 290;
			selector->y = item->y;
			selector->draw();
		}
	}
	
	// Renderizar todo
	SDL_RenderPresent(game->renderer);
}

