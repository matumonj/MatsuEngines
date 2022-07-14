#include "FollowJudgement.h"
#include"Player.h"
#include"Enemy.h"
#include"Collision.h"
bool FollowJudgement::Judgment(Enemy* enemy)
{

	if (enemy->GetSearchFlag()&& enemy->GetSearchPlayer() ==false)//追跡を開始するフラグ
	{
		enemy->SetSearchNow(true);//追跡をしているフラグ
		return true;
	}

	return false;
}