#include "BossEnemyAttackCircle.h"
#include"BossEnemyFollow.h"
#include"CircleAttack.h"
#include"mHelper.h"
void BossEnemyAttackCircle::Initialize(Enemy* enmey)
{

}

void BossEnemyAttackCircle::Update(Enemy* enemy)
{
	CircleAttack::GetInstance()->ActionJudg();

	if (Percent::GetParcent(enemy->GetMaxHP(), enemy->GetHP()) <= 80.0f) {
		CircleAttack::GetInstance()->SetDamageArea(CircleAttack::NORTH, CircleAttack::SOUTH);
		enemy->SetAttack_End(enemy->CIRCLE_1,true);
	}
	if (Percent::GetParcent(enemy->GetMaxHP(), enemy->GetHP()) <= 70.0f) {
		CircleAttack::GetInstance()->SetDamageArea(CircleAttack::WEST, CircleAttack::SOUTH);
		enemy->SetAttack_End(enemy->CIRCLE_2, true);
	}

	if (CircleAttack::GetInstance()->GetFaseEnd() == CircleAttack::FASEFOUR) {
		enemy->ChangeState_Boss(new BossEnemyFollow());
	}
}