#pragma once
class Enemy;
#include"PlayerControl.h"
// ���s����
class ExecJudgmentBase
{
public:
	virtual bool Judgment(Enemy* enemy) = 0;
};
