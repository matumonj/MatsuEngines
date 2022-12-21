#pragma once
#include "GuardianState.h"

#include "Enemy.h"

class GuardianNAttackState :
	public GuardianState
{
public:
	void Initialize(Enemy* enemy) override;

	void Update(Enemy* enemy) override;

private:
	float PosYMovingT;
};
