#include "BossEnemyAttackSlam.h"
#include"BossEnemyFollow.h"
#include"FrontCircleAttack.h"
#include"mHelper.h"
void BossEnemyAttackSlam::Initialize(Enemy* enmey)
{

}

void BossEnemyAttackSlam::Update(Enemy* enemy)
{
	FrontCircleAttack::GetInstance()->ActionJudg();

	if (Percent::GetParcent(enemy->GetMaxHP(), enemy->GetHP()) <= 90.0f) {
		enemy->SetAttack_End(enemy->Slam, true);
	}
	if (FrontCircleAttack::GetInstance()->GetFaseEnd() == FrontCircleAttack::FASEFOUR) {
		enemy->ChangeState_Boss(new BossEnemyFollow());
	}
}