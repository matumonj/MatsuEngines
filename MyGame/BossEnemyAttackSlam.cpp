#include "BossEnemyAttackSlam.h"
#include"BossEnemyFollow.h"
#include"FrontCircleAttack.h"
#include"mHelper.h"

void BossEnemyAttackSlam::Initialize(Enemy* enmey)
{
}

void BossEnemyAttackSlam::Update(Enemy* enemy)
{
	FrontCircleAttack::GetInstance()->ActionJudg();

	if (Percent::GetParcent(static_cast<float>(enemy->GetMaxHP()), static_cast<float>(enemy->GetHP())) <= 90.0f)
	{
		enemy->SetAttack_End(enemy->Slam, true);
	}
	if (FrontCircleAttack::GetInstance()->GetPhaseEnd() == FrontCircleAttack::PHASEFOUR)
	{
		enemy->ChangeState_Boss(new BossEnemyFollow());
	}
}
