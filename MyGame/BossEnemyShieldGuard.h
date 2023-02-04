#pragma once
#include "BossEnemyState.h"
class BossEnemyShieldGuard :
    public BossEnemyState
{
public:
	void Initialize(Enemy* enemy) override;

	void Update(Enemy* enemy) override;

private:
	void Knock(Enemy* enemy);
	int timer;
	float knockpower;
};

