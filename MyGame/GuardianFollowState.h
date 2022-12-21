#pragma once
#include "GuardianState.h"

#include "Enemy.h"

class GuardianFollowState :
	public GuardianState
{
public:
	void Initialize(Enemy* enemy) override;

	void Update(Enemy* enemy) override;

private:
	int NormalAttackCount;
	float PosYMovingT;
	float RotY;
	void Follow(Enemy* enemy);
};
