#include "FollowJudgement.h"
#include"Player.h"
#include"Enemy.h"
#include"Collision.h"
bool FollowJudgement::Judgment(Enemy* enemy)
{

	if (enemy->GetHP() > 100&&((enemy->GetSearchFlag()==true)||enemy->GetRecvDamageJudg())||(Collision::GetLength(enemy->GetPosition(), Player::GetInstance()->GetPosition()) < 20&&enemy->GetState()!=enemy->NOW_ATTACK))//�ǐՂ��J�n����t���O
	{
		//�ǐՂ����Ă���t���O
		return true;
	}

	return false;
}