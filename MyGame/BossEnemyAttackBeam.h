#pragma once
#include "BossEnemyState.h"

class BossEnemyAttackBeam :
	public BossEnemyState
{
public:
	void Initialize(Enemy* enemy) override;

	void Update(Enemy* enemy) override;
};
