#pragma once
#include "EnemyActionBase.h"

class DeathAction :public EnemyActionBase
{
public:
	static DeathAction* GetInstance()
	{
		static DeathAction instance;
		return &instance;
	}

	virtual STATE Run(Enemy* enemy);

};

