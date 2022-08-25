#pragma once
class Enemy;
#include"PlayerControl.h"
// é¿çsîªíË
class ExecJudgmentBase
{
public:
	virtual bool Judgment(Enemy* enemy) = 0;
};
