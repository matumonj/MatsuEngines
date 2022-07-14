#include "WalkJudgement.h"
#include"Player.h"
#include"Enemy.h"
#include"Collision.h"
bool WalkJudgment::Judgment(Enemy* enemy)
{
	if(Collision::GetLength(Player::GetInstance()->GetPosition(),enemy->GetPosition())>1&& enemy->GetSearchPlayer() == false)
	{
		return true;
	}

	return false;
}