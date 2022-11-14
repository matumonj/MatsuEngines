#pragma once
#include "EnemyState.h"
#include"Enemy.h"

class EnemyRespawnState :
	public EnemyState
{
public:
	void Initialize(Enemy* enemy) override;

	void Update(Enemy* enemy) override;
};
