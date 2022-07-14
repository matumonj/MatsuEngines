#ifndef MAGIC_ATTACK_JUDGMENT_H_
#define MAGIC_ATTACK_JUDGMENT_H_

#include "ExecJudgmentBase.h"

class Enemy;

// ñÇñ@çsìÆîªíË
class WalkJudgment : public ExecJudgmentBase
{
public:
	static WalkJudgment* GetInstance()
	{
		static WalkJudgment m_Instance;
		return &m_Instance;
	}

	virtual bool Judgment(Enemy* enemy);
};

#endif
