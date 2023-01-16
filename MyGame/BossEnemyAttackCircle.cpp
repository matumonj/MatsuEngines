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
	enemy->SetAnimation(BossEnemy::NowAttackMotion::MAGIC, 1.f, false);
	CircleAttack::GetInstance()->ActionJudg();

	if (Percent::GetParcent(static_cast<float>(enemy->GetMaxHP()), static_cast<float>(enemy->GetHP())) < 90.0f)
	{
		CircleAttack::GetInstance()->SetDamageArea(CircleAttack::WEST, CircleAttack::SOUTH);
		enemy->SetAttack_End(enemy->CIRCLE_1, true);
	}

	if (CircleAttack::GetInstance()->GetPhaseEnd() == CircleAttack::PHASEFOUR)
	{
		enemy->ChangeState_Boss(new BossEnemyFollow());
	}
}
