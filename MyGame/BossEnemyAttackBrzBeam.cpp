#include "BossEnemyAttackBrzBeam.h"
#include "BossEnemy.h"
#include"BossEnemyFollow.h"
#include "BronzeAttack.h"
#include"KnockAttack.h"
#include"mHelper.h"

void BossEnemyAttackBrzBeam::Initialize(Enemy* enmey)
{
}

void BossEnemyAttackBrzBeam::Update(Enemy* enemy)
{
	enemy->SetRecvDamage2(false);
	BronzeAttack::GetIns()->SetAttackPhase(true);
	enemy->SetAnimation(BossEnemy::NowAttackMotion::BNORMAL, 1.f, false);

	if (Percent::GetParcent(static_cast<float>(enemy->GetMaxHP()), static_cast<float>(enemy->GetHP())) <= 90.0f)
	{
		enemy->SetAttack_End(enemy->BRONZEATTACK, true);
	}
	//enemy->SetMagicAttackTime(true);

	if (BronzeAttack::GetIns()->GetPhase() ==BronzeAttack::END)
	{
	enemy->ChangeState_Boss(new BossEnemyFollow());
	}
}
