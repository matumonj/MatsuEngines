#pragma once
class Enemy;
class Player;

class GuardianState
{
public:
	virtual void Initialize(Enemy* enmey) = 0;

	virtual void Update(Enemy* enemy) = 0;
};
