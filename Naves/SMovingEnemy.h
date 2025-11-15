#pragma once
#include "Enemy.h"
class SMovingEnemy :
    public Enemy
{
public:
    SMovingEnemy(float x, float y, Game* game);
    void update() override;
};

