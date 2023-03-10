#include "BossEnemyAttackSlam.h"

#include "BossEnemy.h"
#include"BossEnemyFollow.h"
#include"FrontCircleAttack.h"
#include"mHelper.h"

void BossEnemyAttackSlam::Initialize(Enemy* enmey)
{
}

void BossEnemyAttackSlam::Update(Enemy* enemy)
{
	enemy->SetAnimation(BossEnemy::NowAttackMotion::MAGIC, false, 1.f);
	enemy->SetRecvDamage2(false);
	if (Percent::GetParcent(static_cast<float>(enemy->GetMaxHP()), static_cast<float>(enemy->GetHP())) <= 80.0f)
	{
		enemy->SetAttack_End(enemy->CIRCLE_1, true);
	}
	if (Percent::GetParcent(static_cast<float>(enemy->GetMaxHP()), static_cast<float>(enemy->GetHP())) <= 20.0f)
	{
		enemy->SetAttack_End(enemy->CIRCLE_2, true);
	}
	if (enemy->GetAnimationTime() >= enemy->GetFbxTimeEnd() - 0.1f)
	{
		enemy->SetRecvDamage2(false);
		enemy->ChangeState_Boss(new BossEnemyFollow());
	}
}
