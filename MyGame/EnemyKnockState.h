#pragma once
#include "EnemyState.h"
#include"Enemy.h"

class EnemyKnockState :
	public EnemyState
{
private:
public:
	void Initialize(Enemy* enemy) override;

	void Update(Enemy* enemy) override;
};
