#pragma once
class Enemy;

// ���s����
class ExecJudgmentBase
{
public:
	virtual bool Judgment(Enemy* enemy) = 0;
};
