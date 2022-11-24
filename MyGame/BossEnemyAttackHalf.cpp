#include "BossEnemyAttackHalf.h"
#include"BossEnemyFollow.h"
#include"HalfAttack.h"
#include"mHelper.h"

void BossEnemyAttackHalf::Initialize(Enemy* enmey)
{
}

void BossEnemyAttackHalf::Update(Enemy* enemy)
{
	HalfAttack::GetInstance()->ActionJudg();
	enemy->SetRoarMotion(true);
	//80%ΘΊΜΌΚUIΉ
	if (Percent::GetParcent(enemy->GetMaxHP(), enemy->GetHP()) <= 60.0f)
	{
		enemy->SetAttack_End(enemy->HALF_1, true);
	}
	//40%ΘΊΜΌΚUIΉ
	if (Percent::GetParcent(enemy->GetMaxHP(), enemy->GetHP()) < 40.0f)
	{
		enemy->SetAttack_End(enemy->HALF_2, true);
	}
	//ΌΚUΜtF[YͺΕγάΕsΑ½η{XΜstateπΗΥΙ
	if (HalfAttack::GetInstance()->GetFaseEnd() == HalfAttack::FASEFOUR)
	{
		enemy->ChangeState_Boss(new BossEnemyFollow());
	}
}
