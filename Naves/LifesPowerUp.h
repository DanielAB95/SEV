#pragma once
#include "PowerUp.h"
class LifesPowerUp :
    public PowerUp
{
public:
    LifesPowerUp(float x, float y, Game* game);
    void effect(Player* player) override;
};

