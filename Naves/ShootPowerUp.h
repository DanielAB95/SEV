#pragma once
#include "PowerUp.h"
class ShootPowerUp :
    public PowerUp
{
public:
    ShootPowerUp(float x, float y, Game* game);
    void effect(Player* player) override;
};

