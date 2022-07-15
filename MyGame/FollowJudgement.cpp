#include "FollowJudgement.h"
#include"Player.h"
#include"Enemy.h"
#include"Collision.h"
bool FollowJudgement::Judgment(Enemy* enemy)
{

	if ( enemy->GetSearchPlayer() ==false&&enemy->GetSearchFlag()==true)//追跡を開始するフラグ
	{
		//追跡をしているフラグ
		return true;
	}

	return false;
}