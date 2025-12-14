#include "InicioLayer.h"

InicioLayer::InicioLayer(Game* game) : Layer(game) {
	init();
}

void InicioLayer::init() {
	// Fondo normal, sin velocidad
	background = new Background("res/Inicio1.png", WIDTH * 0.5, HEIGHT * 0.5, game);
	screen = 0;
	controlContinue = false;
}

void InicioLayer::processControls() {
	// obtener controles
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		keysToControls(event);
	}
	//procesar controles, solo tiene uno
	if (controlContinue) {
		// Cambia la capa
		game->layer = game->gameLayer;
		game->layer->init();
	}
}

void InicioLayer::keysToControls(SDL_Event event) {
	if (event.type == SDL_KEYDOWN) {
		int code = event.key.keysym.sym;
		// Pulsada
		switch (code) {
		case SDLK_ESCAPE:
			game->loopActive = false;
			break;
		case SDLK_SPACE:
			if (screen > 0) {
				controlContinue = true;
			}
			screen++;
			background = new Background("res/Inicio2.png", WIDTH * 0.5, HEIGHT * 0.5, game);
			break;
		}
	}
}

void InicioLayer::draw() {
	background->draw();

	SDL_RenderPresent(game->renderer); // Renderiza NO PUEDE FALTAR
}

void InicioLayer::resetToStart() {
	// Reiniciar el InicioLayer al estado inicial
	screen = 0;
	controlContinue = false;
	background = new Background("res/Inicio1.png", WIDTH * 0.5, HEIGHT * 0.5, game);
}

