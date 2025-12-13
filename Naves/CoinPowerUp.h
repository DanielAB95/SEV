#pragma once
#include "PowerUp.h"

class CoinPowerUp : public PowerUp
{
public:
    CoinPowerUp(float x, float y, Game* game);
    CoinPowerUp(float x, float y, int value, Game* game);
    void effect(Player* player) override;
    int coinValue;
};
