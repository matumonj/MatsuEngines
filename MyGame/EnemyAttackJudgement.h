#pragma once
#include "ExecJudgmentBase.h"
class Enemy;
class EnemyAttackJudgement : public ExecJudgmentBase
{
public:
	static EnemyAttackJudgement* GetInstance()
	{
		static EnemyAttackJudgement m_Instance;
		return &m_Instance;
	}

	virtual bool Judgment(Enemy* enemy);
};

