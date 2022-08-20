#include "EnemyAttackJudgement.h"

#include"Player.h"
#include"Enemy.h"
#include"Collision.h"
bool EnemyAttackJudgement::Judgment(Enemy* enemy)
{
	bool GetLen = Collision::GetLength(enemy->GetPosition(), Player::GetInstance()->GetPosition()) < 10;
	if (enemy->GetHP() > 100&&(enemy->GetFolatc()) &&enemy->GetAttackCoolTime()==0)
	{
		return true;
	}

	return false;
}