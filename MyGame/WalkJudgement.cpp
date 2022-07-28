#include "WalkJudgement.h"
#include"Player.h"
#include"Enemy.h"
#include"Collision.h"
bool WalkJudgment::Judgment(Enemy* enemy)
{
	if(Collision::GetLength(Player::GetInstance()->GetPosition(),enemy->GetPosition())>0||enemy->getendsearch()||enemy->GetTime()==0)
	{
		return true;
	}

	return false;
}