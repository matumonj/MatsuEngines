#pragma once
#include "BossEnemyState.h"
#include"Enemy.h"
class BossEnemyDeath :
    public BossEnemyState
{
	// DirectX::���ȗ�
	using XMVECTOR = DirectX::XMVECTOR;

public:
	void Initialize(Enemy* enemy)override;

	void Update(Enemy* enemy)override;

private:
	bool ActionCount = true, ActionCount2 = true;
	float RotY;
};
