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
	void UpMove(Enemy* enemy);
	void DownMove(Enemy* enemy);
	float PosYMovingT;
};
