#include "FollowJudgement.h"
#include"Player.h"
#include"Enemy.h"
#include"Collision.h"
bool FollowJudgement::Judgment(Enemy* enemy)
{

	if ( (enemy->GetSearchPlayer_Distance() ==true&&enemy->GetSearchFlag()==true)||enemy->GetRecvDamageJudg())//�ǐՂ��J�n����t���O
	{
		//�ǐՂ����Ă���t���O
		return true;
	}

	return false;
}