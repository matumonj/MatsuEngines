#pragma once
#include "ExecJudgmentBase.h"
class Enemy;
class DeathJudgment : public ExecJudgmentBase
{
public:

	static DeathJudgment* GetInstance()
	{
		static DeathJudgment m_Instance;
		return &m_Instance;
	}

	virtual bool Judgment(Enemy* enemy);
};

