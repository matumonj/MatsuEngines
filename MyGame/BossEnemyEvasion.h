#pragma once
#include"BossEnemyState.h"
#include"Enemy.h"

class BossEnemyEvasion : public BossEnemyState
{
public:
	void Initialize(Enemy* enemy) override;

	void Update(Enemy* enemy) override;
};

class BossEnemyFalter : public BossEnemyState
{
public:
	void Initialize(Enemy* enemy) override;

	void Update(Enemy* enemy) override;
};
