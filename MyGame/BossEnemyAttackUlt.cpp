#include "BossEnemyAttackUlt.h"
#include "BossEnemy.h"
#include"BossEnemyFollow.h"
#include "CameraControl.h"
#include "UltAttack.h"
#include"KnockAttack.h"
#include"mHelper.h"

void BossEnemyAttackUlt::Initialize(Enemy* enmey)
{
}

void BossEnemyAttackUlt::Update(Enemy* enemy)
{
	enemy->SetRecvDamage2(false);
	enemy->SetAnimation(BossEnemy::NowAttackMotion::BROAR, false, 1.f);
	
	if (Percent::GetParcent(static_cast<float>(enemy->GetMaxHP()), static_cast<float>(enemy->GetHP())) <= 90.0f)
	{
		enemy->SetAttack_End(enemy->ULT, true);
	}
	CameraControl::GetIns()->ShakeCamera();

	if (UltAttack::GetIns()->GetPhase() == UltAttack::PHASE_FOUR)
	{
		enemy->ChangeState_Boss(new BossEnemyFollow());
	}
}
