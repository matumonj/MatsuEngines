#pragma once
class Enemy;

// Às”»’è
class ExecJudgmentBase
{
public:
	virtual bool Judgment(Enemy* enemy) = 0;
};
