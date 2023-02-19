#include "BossEnemyAttackCircle.h"
#include"BossEnemyFollow.h"
#include"FrontCircleAttack.h"
#include"BossEnemy.h"
#include "mHelper.h"

void BossEnemyAttackCircle::Initialize(Enemy* enmey)
{
}

void BossEnemyAttackCircle::Update(Enemy* enemy)
{
	enemy->SetAnimation(BossEnemy::NowAttackMotion::MAGIC, false, 1.f);
	enemy->SetRecvDamage2(false);
	if (Percent::GetParcent(static_cast<float>(enemy->GetMaxHP()), static_cast<float>(enemy->GetHP())) <= 90.0f)
	{
		enemy->SetAttack_End(enemy->CIRCLE_1, true);
	}
	if (Percent::GetParcent(static_cast<float>(enemy->GetMaxHP()), static_cast<float>(enemy->GetHP())) <= 80.0f)
	{
		enemy->SetAttack_End(enemy->CIRCLE_2, true);
	}
	if (enemy->GetAnimationTime() >= enemy->GetFbxTimeEnd() - 0.1f)
	{
		enemy->SetRecvDamage2(false);
		enemy->ChangeState_Boss(new BossEnemyFollow());
	}
}
