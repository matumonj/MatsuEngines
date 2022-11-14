#pragma once
#include"BossEnemyState.h"

class BossEnemyAttackSlam :
	public BossEnemyState
{
public:
	void Initialize(Enemy* enemy) override;

	void Update(Enemy* enemy) override;
};
