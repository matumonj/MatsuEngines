#include "BossEnemyAttackKnock.h"
#include"BossEnemyFollow.h"
#include"KnockAttack.h"
#include"mHelper.h"
void BossEnemyAttackKnock::Initialize(Enemy* enmey)
{

}

void BossEnemyAttackKnock::Update(Enemy* enemy)
{
	KnockAttack::GetInstance()->ActionJudg();
	if (Percent::GetParcent(enemy->GetMaxHP(), enemy->GetHP()) <= 30.0f) {
		enemy->SetAttack_End(enemy->KNOCK, true);
	}
	if (KnockAttack::GetInstance()->GetFase() == KnockAttack::FASETHREE) {
		enemy->ChangeState_Boss(new BossEnemyFollow());
	}
}