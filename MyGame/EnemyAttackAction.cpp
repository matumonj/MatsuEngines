#include "EnemyAttackAction.h"
#include "WalkAction.h"
#include "Enemy.h"
#include"Collision.h"
#include"mHelper.h"
using namespace DirectX;

EnemyActionBase::STATE EnemyAttackAction::Run(Enemy* enemy)
{
	const float DetectionRange = 5.0f;
	bool SearchPlayer = Collision::GetLength(enemy->GetPosition(), Player::GetInstance()->GetPosition()) < DetectionRange;
	enemy->Attack();
	//if (SearchPlayer) {
	return EnemyActionBase::STATE::COMPLETE;
	//}
}