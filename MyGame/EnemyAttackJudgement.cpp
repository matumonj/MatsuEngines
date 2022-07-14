#include "EnemyAttackJudgement.h"

#include"Player.h"
#include"Enemy.h"
#include"Collision.h"
bool EnemyAttackJudgement::Judgment(Enemy* enemy)
{
	if (enemy->GetSearchPlayer()==true&&enemy->AttackCoolTime()==0)
	{
		return true;
	}

	return false;
}