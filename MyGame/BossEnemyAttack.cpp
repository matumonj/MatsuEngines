#include "BossEnemyAttack.h"
#include"BossEnemyFollow.h"
#include"BossEnemyStay.h"
#include"CameraControl.h"
#include"PlayerControl.h"
#include"BossEnemy.h"
#include "BossEnemyAttackCircle.h"
#include "BossEnemyDeath.h"
#include "BossEnemyEvasion.h"
#include "BossEnemyShieldGuard.h"
#include "UI.h"

void BossEnemyAttack::Initialize(Enemy* enmey)
{
}

void BossEnemyAttack::Update(Enemy* enemy)
{
	int Evaprobability;
	//...Damage2()�[���Q�Ƃ��̃}�W�b�N�i���o�[�悭�Ȃ�
	if (enemy->GetRecvDamage2())
	{
		//��_���[�W��
		//�����𐶐����l�����ȏ�Ȃ狯�ݏ��
		Evaprobability = rand() % 100 + 1;
		
		 if(Evaprobability > 75)
		{
			enemy->ChangeState_Boss(new BossEnemyShieldGuard());
		}else if (Evaprobability > 40)
		{
			PlayerControl::GetIns()->GetPlayer()->SetKnockF(true);

			UI::GetIns()->SetRadBlur(true);
			enemy->ChangeState_Boss(new BossEnemyAttackCircle());

			//enemy->ChangeState_Boss(new BossEnemyFalter());
		}
		else
		{
			enemy->ChangeState_Boss(new BossEnemyFollow());
		}
	}
	enemy->SetGuardAction(false);
	//�����_���ōU�����@��I�o
	//�����_��������������p�̊֐�������ق����B�B
	const int SideNum = 30;
	const int HeightNum = 60;
	if (randmotion < HeightNum)
	{
		//�c�U��
		enemy->SetAnimation(BossEnemy::NowAttackMotion::BNORMAL, false, 1.0);
	}
	else if (randmotion < SideNum)
	{
		//��U��
		enemy->SetAnimation(BossEnemy::NowAttackMotion::SWING, false, 1.0);
	}
	else
	{
		//�R���{�U��
		enemy->SetAnimation(BossEnemy::NowAttackMotion::BNORMAL2, false, 1.0);
	}

	if (CameraControl::GetIns()->GetCameraState() == CameraControl::BOSSCUTSCENE)
	{
		enemy->ChangeState_Boss(new BossEnemyStay());
	}
	else
	{
		if (enemy->GetAnimationTime() >= enemy->GetFbxTimeEnd() - 0.1)
		{
			//���ȏ㗣�ꂽ���Ԃ�ǐՂ�
			if (Collision::GetLength(enemy->GetPosition(), PlayerControl::GetIns()->GetPlayer()->GetPosition()) > 17.f)
			{
				randmotion = 0;
				enemy->ChangeState_Boss(new BossEnemyFollow());
			}
			//����ȊO�͑ҋ@
			else
			{
				randmotion = 0;
				enemy->ChangeState_Boss(new BossEnemyStay());
			}
		}
		else
		{
			//0�̂Ƃ����������𐶐�����悤�Ɂ@���������͕ς�����
			if (randmotion == 0)
			{
				randmotion = rand() % 100 + 1;
			}
		}
	}

	//���S
	if (enemy->GetHP() <= 0.f)
	{
		enemy->ChangeState_Boss(new BossEnemyDeath());
	}
}
