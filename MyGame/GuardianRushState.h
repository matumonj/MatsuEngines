#pragma once
#include "GuardianState.h"

#include "Enemy.h"

class GuardianRushState :
	public GuardianState
{
public:
	void Initialize(Enemy* enemy) override;

	void Update(Enemy* enemy) override;
};
