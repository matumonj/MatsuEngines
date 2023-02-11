#include "BossEnemyAttackCircle.h"
#include"BossEnemyFollow.h"
#include "CameraControl.h"
#include"CircleAttack.h"
#include"mHelper.h"
#include"BossEnemy.h"

void BossEnemyAttackCircle::Initialize(Enemy* enmey)
{
}

void BossEnemyAttackCircle::Update(Enemy* enemy)
{
	enemy->SetRecvDamage2(false);
	enemy->SetAnimation(BossEnemy::NowAttackMotion::MAGIC, false, 1.f);
	CircleAttack::GetIns()->SetAttackPhase(true);
	if (Percent::GetParcent(static_cast<float>(enemy->GetMaxHP()), static_cast<float>(enemy->GetHP())) < 90.0f)
	{
		CircleAttack::GetIns()->SetDamageArea(CircleAttack::WEST, CircleAttack::SOUTH);
		enemy->SetAttack_End(enemy->CIRCLE_1, true);
	}

	if (enemy->GetAnimationTime() >= enemy->GetFbxTimeEnd() - 0.1)

	{
		enemy->ChangeState_Boss(new BossEnemyFollow());
	}
}
