#pragma once
#include "BossEnemyState.h"

class BossEnemyAttackCross :
    public BossEnemyState
{
public:
	void Initialize(Enemy* enemy) override;

	void Update(Enemy* enemy) override;

private:
	double AnimationSpeed = 0.f;
};

