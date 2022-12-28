#pragma once
#include "GuardianState.h"
#include "Enemy.h"

class GuardianAppearState :
	public GuardianState
{
public:
	void Initialize(Enemy* enemy) override;

	void Update(Enemy* enemy) override;

private:
	float EaseTime;
	bool DownF;
	bool UpF;
	bool SmokeF;
	void DownMove(Enemy* enemy);
};
