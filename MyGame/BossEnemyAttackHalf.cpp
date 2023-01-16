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
	HalfAttack::GetInstance()->ActionJudg();
	enemy->SetAnimation(BossEnemy::NowAttackMotion::BROAR, 1.f, false);
	//80%�ȉ����̔��ʍU���I��
	if (Percent::GetParcent(static_cast<float>(enemy->GetMaxHP()), static_cast<float>(enemy->GetHP())) <= 30.0f)
	{
		enemy->SetAttack_End(enemy->HALF_1, true);
	}

	//���ʍU���̃t�F�[�Y���Ō�܂ōs������{�X��state��ǐՂ�
	if (HalfAttack::GetInstance()->GetPhaseEnd() == HalfAttack::PHASEFOUR)
	{
		
		enemy->ChangeState_Boss(new BossEnemyFollow());
	}
}
