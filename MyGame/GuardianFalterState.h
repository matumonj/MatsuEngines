#pragma once
#include "GuardianState.h"

#include "Enemy.h"

class GuardianFalterState :
	public GuardianState
{
public:
	void Initialize(Enemy* enemy) override;

	void Update(Enemy* enemy) override;
private:
	void Falter(Enemy* enemy);

	float shakex;
	float shakey;
	float shake;
	int shaketime;

};
