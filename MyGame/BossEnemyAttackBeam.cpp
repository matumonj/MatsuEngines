#include "BossEnemyAttackBeam.h"
#include"BossEnemyFollow.h"
#include"AltAttack.h"
#include"mHelper.h"

void BossEnemyAttackBeam::Initialize(Enemy* enmey)
{
}

void BossEnemyAttackBeam::Update(Enemy* enemy)
{
	AltAttack::GetInstance()->ActionJudg();

	if (Percent::GetParcent(enemy->GetMaxHP(), enemy->GetHP()) <= 70.0f)
	{
		enemy->SetAttack_End(enemy->Beam, true);
	}
	if (AltAttack::GetInstance()->GetFaseEnd() == AltAttack::FASEFOUR)
	{
		enemy->ChangeState_Boss(new BossEnemyFollow());
	}
}
