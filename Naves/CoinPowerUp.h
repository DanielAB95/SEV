#pragma once
#include "PowerUp.h"

class CoinPowerUp : public PowerUp
{
public:
    CoinPowerUp(float x, float y, Game* game);
    void effect(Player* player) override;
    int coinValue;
};
