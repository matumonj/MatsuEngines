#include "FollowJudgement.h"
#include"Player.h"
#include"Enemy.h"
#include"Collision.h"
bool FollowJudgement::Judgment(Enemy* enemy)
{

	if ( enemy->GetSearchPlayer() ==false&&enemy->GetSearchFlag()==true)//�ǐՂ��J�n����t���O
	{
		//�ǐՂ����Ă���t���O
		return true;
	}

	return false;
}