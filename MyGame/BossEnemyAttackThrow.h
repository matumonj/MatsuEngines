#pragma once
#include "BossEnemyState.h"
class BossEnemyAttackThrow :
    public BossEnemyState
{
public:
	void Initialize(Enemy* enemy) override;

	void Update(Enemy* enemy) override;

};

