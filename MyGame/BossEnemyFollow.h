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


private:
	bool ActionCount = true, ActionCount2 = true;
	int Evaprobability;
	float RotY = 0;
	bool BlurF;


	void ActionSequence(Enemy* enemy, float percent, BossAttackActionManager* action, int actionnum,
	                    BossEnemyState* state);

private:

	enum AttackInvoPer
	{
		CIRCLE_FIR,
		CIRCLE_SEC,
		RUSH_FIR,
		ULY_FIR,
		MAGIC_FIR,
		MAGIC_SEC,
		SUMMON_FIR
	};
	std::array<float,7>_attackInvoPer;
	
};
