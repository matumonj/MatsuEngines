#include "FollowJudgement.h"
#include"Player.h"
#include"Enemy.h"
#include"Collision.h"
#include"PlayerControl.h"
bool FollowJudgement::Judgment(Enemy* enemy)
{
	bool GetLen = Collision::GetLength(enemy->GetPosition(), PlayerControl::GetInstance()->GetPlayer()->GetPosition()) < 20&&
		Collision::GetLength(enemy->GetPosition(), PlayerControl::GetInstance()->GetPlayer()->GetPosition()) > 10;
	if ( enemy->GetHP() > 100 && (GetLen&&!enemy->GetFolatc()&& enemy->GetState() == enemy->None))//追跡を開始するフラグ
	{
		//追跡をしているフラグ
		return true;
	}

	return false;
}