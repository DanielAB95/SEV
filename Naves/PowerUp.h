#pragma once
#include "Actor.h"
#include "Player.h"
class PowerUp :
    public Actor
{
public:
    PowerUp(string filename, float x, float y, int width, int height, Game* game);
    void update();
    virtual void effect(Player* player);
};

