#include "FollowJudgement.h"
#include"Player.h"
#include"Enemy.h"
#include"Collision.h"
bool FollowJudgement::Judgment(Enemy* enemy)
{

	if (enemy->GetSearchFlag()&& enemy->GetSearchPlayer() ==false)//�ǐՂ��J�n����t���O
	{
		enemy->SetSearchNow(true);//�ǐՂ����Ă���t���O
		return true;
	}

	return false;
}