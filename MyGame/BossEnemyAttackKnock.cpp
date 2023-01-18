#include "BossEnemyAttackKnock.h"

#include "BossEnemy.h"
#include"BossEnemyFollow.h"
#include"KnockAttack.h"
#include"mHelper.h"

void BossEnemyAttackKnock::Initialize(Enemy* enmey)
{
}

void BossEnemyAttackKnock::Update(Enemy* enemy)
{
	enemy->SetRecvDamage2(false);
	KnockAttack::GetInstance()->SetAttackPhase(true);
	enemy->SetAnimation(BossEnemy::NowAttackMotion::BNORMAL, 1.f, false);

	if (Percent::GetParcent(static_cast<float>(enemy->GetMaxHP()), static_cast<float>(enemy->GetHP())) <= 90.0f)
	{
		enemy->SetAttack_End(enemy->KNOCK, true);
	}
	//enemy->SetMagicAttackTime(true);
	
	//if (KnockAttack::GetInstance()->GetPhase() == KnockAttack::PHASEFOUR)
	
		enemy->ChangeState_Boss(new BossEnemyFollow());
	//}
}
