#include "GameOverLayer.h"
#include "GameLayer.h" // NUEVO: Para acceder al método de reinicio
#include "InicioLayer.h" // NUEVO: Para reiniciar la capa de inicio

GameOverLayer::GameOverLayer(Game* game) : Layer(game) {
	init();
}

void GameOverLayer::init() {
	// Fondo normal, sin velocidad
	background = new Background("res/GameOver.png", WIDTH * 0.5, HEIGHT * 0.5, game);
	controlContinue = false;
}

void GameOverLayer::processControls() {
	// obtener controles
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		keysToControls(event);
	}
	//procesar controles, solo tiene uno
	if (controlContinue) {
		// NUEVO: Reiniciar GameLayer completamente antes de cambiar capas
		GameLayer* gameLayer = dynamic_cast<GameLayer*>(game->gameLayer);
		if (gameLayer != nullptr) {
			gameLayer->resetGameToInitialState();
		}
		
		// NUEVO: Reiniciar también el InicioLayer
		InicioLayer* inicioLayer = dynamic_cast<InicioLayer*>(game->inicioLayer);
		if (inicioLayer != nullptr) {
			inicioLayer->resetToStart();
		}
		
		// Cambiar a la capa de inicio
		game->layer = game->inicioLayer;
		game->layer->init();
	}
}

void GameOverLayer::keysToControls(SDL_Event event) {
	if (event.type == SDL_KEYDOWN) {
		int code = event.key.keysym.sym;
		// Pulsada
		switch (code) {
		case SDLK_ESCAPE:
			game->loopActive = false;
			break;
		case SDLK_SPACE:
			controlContinue = true;
			break;
		}
	}
}

void GameOverLayer::draw() {
	background->draw();

	SDL_RenderPresent(game->renderer); // Renderiza NO PUEDE FALTAR
}

