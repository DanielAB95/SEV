#include "InicioLayer.h"

InicioLayer::InicioLayer(Game* game) : Layer(game) {
	init();
}

void InicioLayer::init() {
	// Fondo normal, sin velocidad
	background = new Background("res/menu_fondo.png", WIDTH * 0.5, HEIGHT * 0.5, game); //TODO cambiar imagen
	button = new Actor("res/boton_jugar.png", WIDTH * 0.5, HEIGHT * 0.7, 232, 72, game); //TODO cambiar boton
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
		controlContinue = false;
	}
}

void InicioLayer::keysToControls(SDL_Event event) {
	if (event.type == SDL_KEYDOWN) {
		int code = event.key.keysym.sym;
		// Pulsada
		switch (code) {
		case SDLK_ESCAPE: // derecha
			game->loopActive = false;
			break;
		case SDLK_SPACE: // dispara
			controlContinue = true;
			break;
		}
	}
}

void InicioLayer::draw() {
	background->draw();
	button->draw();

	SDL_RenderPresent(game->renderer); // Renderiza NO PUEDE FALTAR
}

