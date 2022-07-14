#ifndef PHYSICS_ACTION_H_
#define PHYSICS_ACTION_H_

#include "EnemyActionBase.h"
#include <stdio.h>
class Enemy;

// ï®óùçUåÇçsìÆ
class PhysicsAction : public EnemyActionBase
{
public:
	static PhysicsAction* GetInstance()
	{
		static PhysicsAction instance;
		return &instance;
	}

	virtual STATE Run(Enemy* enemy);
};

#endif
