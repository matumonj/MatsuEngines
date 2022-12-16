#include "BossEnemyAttackHalf.h"
#include"BossEnemyFollow.h"
#include"HalfAttack.h"
#include"mHelper.h"

void BossEnemyAttackHalf::Initialize(Enemy* enmey)
{
}

void BossEnemyAttackHalf::Update(Enemy* enemy)
{
	enemy->SetRecvDamage2(false);
	HalfAttack::GetInstance()->ActionJudg();
	enemy->SetRoarMotion(true);
	//80%以下時の半面攻撃終了
	if (Percent::GetParcent(enemy->GetMaxHP(), enemy->GetHP()) <= 30.0f)
	{
		enemy->SetAttack_End(enemy->HALF_1, true);
	}

	//半面攻撃のフェーズが最後まで行ったらボスのstateを追跡に
	if (HalfAttack::GetInstance()->GetPhaseEnd() == HalfAttack::PHASEFOUR)
	{
		enemy->ChangeState_Boss(new BossEnemyFollow());
	}
}
