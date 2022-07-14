#pragma once
#include"EnemyState.h"
class EnemyAttackState:public EnemyState
{
public:

	void Initialize(Enemy* enemy);

	void Update(Enemy* enemy);
};

