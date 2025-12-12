#include "GameOverLayer.h"

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
		// Cambia la capa
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

