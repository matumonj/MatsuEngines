#include "BossEnemyAttackHalf.h"
#include"BossEnemyFollow.h"
#include"HalfAttack.h"
#include"mHelper.h"
#include"BossEnemy.h"
void BossEnemyAttackHalf::Initialize(Enemy* enmey)
{
}

void BossEnemyAttackHalf::Update(Enemy* enemy)
{
	enemy->SetRecvDamage2(false);
	enemy->SetAnimation(BossEnemy::NowAttackMotion::BJUMP,  false,1.0);
	//80%以下時の半面攻撃終了
	if (Percent::GetParcent(static_cast<float>(enemy->GetMaxHP()), static_cast<float>(enemy->GetHP())) <= 99.0f)
	{
		enemy->SetAttack_End(enemy->HALF_1, true);
	}

	//半面攻撃のフェーズが最後まで行ったらボスのstateを追跡に
	if (HalfAttack::GetInstance()->GetPhase() == HalfAttack::PHASEFOUR)
	{
		
		enemy->ChangeState_Boss(new BossEnemyFollow());
	}
}
