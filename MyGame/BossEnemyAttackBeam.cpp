#include "BossEnemyAttackBeam.h"
#include"BossEnemyFollow.h"
#include"RushAttack.h"
#include "BossEnemy.h"
#include"mHelper.h"

void BossEnemyAttackBeam::Initialize(Enemy* enmey)
{
}

void BossEnemyAttackBeam::Update(Enemy* enemy)
{
	enemy->SetRecvDamage2(false);
	RushAttack::GetInstance()->ActionJudg();
	enemy->SetAnimation(BossEnemy::NowAttackMotion::BWALK, false, 1.f);

	if (Percent::GetParcent(static_cast<float>(enemy->GetMaxHP()), static_cast<float>(enemy->GetHP())) <= 70.0f)
	{
		enemy->SetAttack_End(enemy->Beam, true);
	}
	if (RushAttack::GetInstance()->GetPhaseEnd() == RushAttack::PHASEFOUR)
	{
		enemy->ChangeState_Boss(new BossEnemyFollow());
	}
}
