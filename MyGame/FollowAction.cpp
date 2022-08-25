#include "FollowAction.h"
#include "Enemy.h"
#include"Collision.h"
#include"mHelper.h"
#include"PlayerControl.h"
using namespace DirectX;

EnemyActionBase::STATE FollowAction::Run(Enemy* enemy)
{
	const float DetectionRange = 10.0f;
	bool SearchPlayer = Collision::GetLength(enemy->GetPosition(), PlayerControl::GetInstance()->GetPlayer()->GetPosition()) < DetectionRange;
	enemy->Follow();
	if (enemy->GetFolatc()|| enemy->GetEndSearch()||enemy->GetState()==enemy->NOW_ATTACK) {
		//enemy->SetSearchNow(false);
		return EnemyActionBase::STATE::COMPLETE;
	}
}