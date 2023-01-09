#include "BossEnemyAttackBeam.h"
#include"BossEnemyFollow.h"
#include"AltAttack.h"
#include"mHelper.h"

void BossEnemyAttackBeam::Initialize(Enemy* enmey)
{
}

void BossEnemyAttackBeam::Update(Enemy* enemy)
{
	enemy->SetRecvDamage2(false);
	AltAttack::GetInstance()->ActionJudg();

	if (Percent::GetParcent(static_cast<float>(enemy->GetMaxHP()), static_cast<float>(enemy->GetHP())) <= 70.0f)
	{
		enemy->SetAttack_End(enemy->Beam, true);
	}
	//if (AltAttack::GetInstance()->GetPhaseEnd() == AltAttack::PHASEFOUR)
	{
		enemy->ChangeState_Boss(new BossEnemyFollow());
	}
}
