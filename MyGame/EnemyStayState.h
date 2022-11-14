#pragma once
#include"Enemy.h"
#include"EnemyState.h"

class EnemyStayState : public EnemyState
{
public:
	void Initialize(Enemy* enemy) override;

	void Update(Enemy* enemy) override;

private:
	//Œü‚«•Ï‚¦‚é‘O‚ÌŒü‚«
	float BeforeRot;
	float AfterRot;
	float RotTime = 0;
	int StayCount = 0;
};
