#pragma once
#include "BossEnemyState.h"

class BossEnemyAttackUlt :
	public BossEnemyState
{
public:
	void Initialize(Enemy* enemy) override;

	void Update(Enemy* enemy) override;
};
