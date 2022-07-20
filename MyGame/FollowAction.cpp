#include "FollowAction.h"
#include "Enemy.h"
#include"Collision.h"
#include"mHelper.h"
using namespace DirectX;

EnemyActionBase::STATE FollowAction::Run(Enemy* enemy)
{
	const float DetectionRange = 10.0f;
	bool SearchPlayer = Collision::GetLength(enemy->GetPosition(), Player::GetInstance()->GetPosition()) < DetectionRange;
	enemy->Follow();
	if (Collision::GetLength(Player::GetInstance()->GetPosition(),enemy->GetPosition())>30) {
		//enemy->SetSearchNow(false);
		return EnemyActionBase::STATE::COMPLETE;
	}
}