#pragma once
#include "EnemyState.h"

class EnemyReturnCreatePos :
	public EnemyState
{
public:
	void Initialize(Enemy* enemy) override;

	void Update(Enemy* enemy) override;

private:
	float RotY;
};
