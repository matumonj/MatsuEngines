#pragma once
#include "GuardianState.h"

#include "Enemy.h"
class GuardianDeathState :
    public GuardianState
{
public:
	void Initialize(Enemy* enemy) override;

	void Update(Enemy* enemy) override;
};

