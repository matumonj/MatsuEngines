#pragma once
#include "GuardianState.h"

#include "Enemy.h"

class GuardianBomState :
	public GuardianState
{
public:
	void Initialize(Enemy* enemy) override;

	void Update(Enemy* enemy) override;

private:
	float PosYMovingT;
};
