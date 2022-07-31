#include "DeathAction.h"
#include "Enemy.h"
#include"Collision.h"
#include"mHelper.h"
using namespace DirectX;

EnemyActionBase::STATE DeathAction::Run(Enemy* enemy)
{
	enemy->Death();
	//if (enemy->GetDeath()) {
		return EnemyActionBase::STATE::COMPLETE;
	//}
}