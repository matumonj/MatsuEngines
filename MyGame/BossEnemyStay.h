#pragma once
#include "BossEnemyState.h"
#include"Enemy.h"
class BossEnemyStay :
    public BossEnemyState
{

public:

	void Initialize(Enemy* enemy);

	void Update(Enemy* enemy);

};

