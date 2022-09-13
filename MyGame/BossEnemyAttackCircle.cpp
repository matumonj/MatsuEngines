#include "BossEnemyAttackCircle.h"
#include"BossEnemyFollow.h"
#include"CircleAttack.h"
#include"mHelper.h"
void BossEnemyAttackCircle::Initialize(Enemy* enmey)
{

}

void BossEnemyAttackCircle::Update(Enemy* enemy)
{
	CircleAttack::GetInstance()->SetDamageArea(CircleAttack::WEST, CircleAttack::NORTH);
	CircleAttack::GetInstance()->ActionJudg();
	if (Percent::GetParcent(enemy->GetMaxHP(), enemy->GetHP()) <= 40.0f) {
		enemy->SetAttack_End(enemy->CIRCLE_1,true);
	}
	if (CircleAttack::GetInstance()->GetFaseEnd() == CircleAttack::FASEFOUR) {
		enemy->ChangeState_Boss(new BossEnemyFollow());
	}
}