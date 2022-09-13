#pragma once
#include"Enemy.h"
#include"EnemyState.h"
class EnemyStayState :public EnemyState
{
public:

	void Initialize(Enemy* enemy);

	void Update(Enemy* enemy);

private:
	//向き変える前の向き
	float BeforeRot;
	float AfterRot;
	float RotTime = 0;
	int StayCount = 0;
};

