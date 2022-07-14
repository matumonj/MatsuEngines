#pragma once
#include"EnemyState.h"
#include"Enemy.h"
class EnemyWalkState :public EnemyState 
{
public:
	void Initialize(Enemy*enemy);

	void Update(Enemy*enemy);
private:
	float tempx, tempz;
	int movement;

};

