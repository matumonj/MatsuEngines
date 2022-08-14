#include "EnemyAttackJudgement.h"

#include"Player.h"
#include"Enemy.h"
#include"Collision.h"
bool EnemyAttackJudgement::Judgment(Enemy* enemy)
{
	if ( Collision::GetLength(enemy->GetPosition(),Player::GetInstance()->GetPosition())<10&&enemy->GetAttackCoolTime()==0)
	{
		return true;
	}

	return false;
}