#include "EnemyAttackAction.h"
#include "WalkAction.h"
#include "Enemy.h"
#include"Collision.h"
#include"mHelper.h"
using namespace DirectX;

EnemyActionBase::STATE EnemyAttackAction::Run(Enemy* enemy)
{
	enemy->Attack();
	
	//if(enemy->AttackEnd()) {
	return EnemyActionBase::STATE::COMPLETE;
	//}
}