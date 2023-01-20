#include "BossEnemyAttackUlt.h"
#include "BossEnemy.h"
#include"BossEnemyFollow.h"
#include "UltAttack.h"
#include"KnockAttack.h"
#include"mHelper.h"

void BossEnemyAttackUlt::Initialize(Enemy* enmey)
{
}

void BossEnemyAttackUlt::Update(Enemy* enemy)
{
	enemy->SetRecvDamage2(false);
	UltAttack::GetIns()->SetAction(true);
	enemy->SetAnimation(BossEnemy::NowAttackMotion::BNORMAL, 1.f, false);

	if (Percent::GetParcent(static_cast<float>(enemy->GetMaxHP()), static_cast<float>(enemy->GetHP())) <= 50.0f)
	{
		enemy->SetAttack_End(enemy->ULT, true);
	}
	//enemy->SetMagicAttackTime(true);

	if (UltAttack::GetIns()->GetPhase() == UltAttack::END)
	{
		enemy->ChangeState_Boss(new BossEnemyFollow());
	}
}
