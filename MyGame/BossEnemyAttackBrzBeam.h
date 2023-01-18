#pragma once
#include"BossEnemyState.h"
#include"Enemy.h"

class BossEnemyAttackBrzBeam:public BossEnemyState
{
public:
	void Initialize(Enemy* enemy) override;

	void Update(Enemy* enemy) override;
};