#pragma once
#include "EnemyState.h"
#include"Enemy.h"

class EnemyDeathState :
	public EnemyState
{
public:
	void Initialize(Enemy* enemy) override;

	void Update(Enemy* enemy) override;
private:
	int RespawnCount = 0;
};
