#pragma once
#include "Layer.h"
#include "Actor.h"
#include "Background.h"

class InicioLayer :
    public Layer
{
public:
	InicioLayer(Game* game);
	void init() override;
	void draw() override;
	void processControls() override;
	void keysToControls(SDL_Event event);
	bool controlContinue;
	Background* background;
	Actor* button;
	int screen = 0;
};

