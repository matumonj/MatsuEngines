
#include "ExecJudgmentBase.h"

class Enemy;

// –‚–@s“®”»’è
class FollowJudgement : public ExecJudgmentBase
{
public:
	static FollowJudgement* GetInstance()
	{
		static FollowJudgement m_Instance;
		return &m_Instance;
	}

	virtual bool Judgment(Enemy* enemy);
};
