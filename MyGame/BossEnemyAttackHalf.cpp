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
	//80%�ȉ����̔��ʍU���I��
	if (Percent::GetParcent(enemy->GetMaxHP(), enemy->GetHP()) <= 60.0f) {
		enemy->SetAttack_End(enemy->HALF_1,true);
	}
	//40%�ȉ����̔��ʍU���I��
	if (Percent::GetParcent(enemy->GetMaxHP(), enemy->GetHP()) < 20.0f) {
		enemy->SetAttack_End(enemy->HALF_2, true);
	}
	//���ʍU���̃t�F�[�Y���Ō�܂ōs������{�X��state��ǐՂ�
	if (HalfAttack::GetInstance()->GetFaseEnd()==HalfAttack::FASEFOUR) {
		enemy->ChangeState_Boss(new BossEnemyFollow());
	}
}