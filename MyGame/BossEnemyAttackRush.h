#pragma once
#include "BossEnemyState.h"

class BossEnemyAttackRush :
	public BossEnemyState
{
public:
	void Initialize(Enemy* enemy) override;

	void Update(Enemy* enemy) override;


};
