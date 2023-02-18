#pragma once
#include "BossAttackActionManager.h"
#include"BossEnemyState.h"
#include"Enemy.h"

class BossEnemyFollow : public BossEnemyState
{
private:
	// DirectX::Çè»ó™
	using XMVECTOR = DirectX::XMVECTOR;

public:
	void Initialize(Enemy* enemy) override;

	void Update(Enemy* enemy) override;

	void AttackSelect(Enemy* enemy, bool judg, int index);

	void AttackType(Enemy* enemy, int num);

	void AttackStart(Enemy* enemy, int num);
private:
	bool ActionCount = true, ActionCount2 = true;
	int Evaprobability;
	float RotY = 0;

	Enemy* enemys;
	void ActionSequence(Enemy* enemy, float percent, BossAttackActionManager* action, int actionnum, BossEnemyState* state);
};
