#include "BossEnemyFollow.h"
#include "EnemyFollowState.h"
#include"PlayerControl.h"
#include"CameraControl.h"
#include"BossEnemyAttack.h"
#include"BossEnemyAttackCircle.h"
#include"BossEnemyAttackknock.h"
#include"BossEnemyAttackhalf.h"
#include"mHelper.h"
#include"AltAttack.h"
#include"FrontCircleAttack.h"
#include"HalfAttack.h"
#include"CircleAttack.h"
#include"BossEnemyDeath.h"
#include"KnockAttack.h"
#include"BossEnemyAttackSlam.h"
#include"BossEnemyAttackBeam.h"
#include"BossEnemyEvasion.h"
#include"Feed.h"
#include"BossEnemy.h"
#include "BossEnemyAttackBrzBeam.h"
#include "BossEnemyAttackUlt.h"
#include "BronzeAttack.h"
#include "UltAttack.h"

void BossEnemyFollow::Initialize(Enemy* enmey)
{
}

#include"CustomButton.h"

void BossEnemyFollow::Update(Enemy* enemy)
{
	if (Feed::GetInstance()->GetAlpha() > 0.0f)
	{
		return;
	}
	enemy->SetAnimation(BossEnemy::NowAttackMotion::BWALK , 1.f, true);
	//�ǐՏ�������//////////


	//�G���v�G���C���[�̕���������
	XMVECTOR positionA = {
		PlayerControl::GetInstance()->GetPlayer()->GetPosition().x,
		PlayerControl::GetInstance()->GetPlayer()->GetPosition().y,
		PlayerControl::GetInstance()->GetPlayer()->GetPosition().z
	};
	XMVECTOR positionB = {enemy->GetPosition().x, enemy->GetPosition().y, enemy->GetPosition().z};
	//�v���C���[�ƓG�̃x�N�g���̒���(��)�����߂�
	XMVECTOR SubVector = XMVectorSubtract(positionB, positionA); // positionA - positionB;

	//�p�x�̎擾 �v���C���[���G�̍��G�ʒu�ɓ�������������v���C���[�̕���
	RotY = atan2f(SubVector.m128_f32[0], SubVector.m128_f32[2]);
	//�ړ��x�N�g����y������̊p�x�ŉ�]
	XMVECTOR move = {0.0f, 0.0f, 0.1f, 0.0f};

	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(enemy->GetRotation().y + enemy->GetRotRadians()));

	move = XMVector3TransformNormal(move, matRot);
	enemy->SetRotation({
		enemy->GetRotation().x,
		RotY * 60.0f ,
		enemy->GetRotation().z
	});
	if (Collision::GetLength(enemy->GetPosition(), PlayerControl::GetInstance()->GetPlayer()->GetPosition()) > 15.f)
	{
		enemy->SetPosition({
				enemy->GetPosition().x + move.m128_f32[0] * 3,
				enemy->GetPosition().y,
				enemy->GetPosition().z + move.m128_f32[2] * 3
			}
		);
	}

	//////////////////////////////////////////////////////////////

	//���ʂ̍U��
	if (Collision::GetLength(enemy->GetPosition(), PlayerControl::GetInstance()->GetPlayer()->GetPosition()) < 17.f)
	{
		if (enemy->GetCoolTime() == 0) {
			enemy->ChangeState_Boss(new BossEnemyAttack());
		}
	}
	if (enemy->GetRecvDamage2())
	{
		Evaprobability = rand() % 100 + 1;
		if (Evaprobability > 60)
		{
			//enemy->ChangeState_Boss(new BossEnemyFalter());
		}
		else
		{
			enemy->SetRecvDamage2(false);
		}
	}
	if (Collision::GetLength(enemy->GetPosition(), PlayerControl::GetInstance()->GetPlayer()->GetPosition()) < 17.f)
	{
		if (CustomButton::GetInstance()->GetAttackAction()) {
			Evaprobability = rand() % 100 + 1;
			if (Evaprobability <= 30) {
			//	enemy->ChangeState_Boss(new BossEnemyEvasion());
			}
		}
	}

	/*2�����F�̗͐ݒ�(�ݒ�l�ȉ��Ȃ�����R�����̍U����)*/
	AttackSelect(
		enemy, Percent::GetParcent(static_cast<float>(enemy->GetMaxHP()), static_cast<float>(enemy->GetHP())) <= 50.0f,
		enemy->ULT);
	AttackSelect(
		enemy, Percent::GetParcent(static_cast<float>(enemy->GetMaxHP()), static_cast<float>(enemy->GetHP())) <= 90.0f,
		enemy->CIRCLE_1);
	AttackSelect(
		enemy, Percent::GetParcent(static_cast<float>(enemy->GetMaxHP()), static_cast<float>(enemy->GetHP())) <= 70.0f,
		enemy->Beam);
	AttackSelect(
		enemy, Percent::GetParcent(static_cast<float>(enemy->GetMaxHP()), static_cast<float>(enemy->GetHP())) <= 60.0f,
		enemy->KNOCK);

	//���S
	if (enemy->GetHP() <= 0.f)
	{
		enemy->ChangeState_Boss(new BossEnemyDeath());
	}

	//
	if (Percent::GetParcent(static_cast<float>(enemy->GetMaxHP()), static_cast<float>(enemy->GetHP())) <= 70.0f)
	{
		if (AltAttack::GetInstance()->GetPhaseEnd() != AltAttack::PHASEFOUR)
		{
			if (enemy->GetAttack_End(enemy->Beam) == false) {
				AltAttack::GetInstance()->SetAttackPhase(true);
				enemy->ChangeState_Boss(new BossEnemyAttackBeam());
			}
		}
	}
	//
	if (Percent::GetParcent(static_cast<float>(enemy->GetMaxHP()), static_cast<float>(enemy->GetHP())) <= 60.0f)
	{
		if (KnockAttack::GetInstance()->GetPhase() != KnockAttack::PHASEFOUR)
		{
			if (enemy->GetAttack_End(enemy->KNOCK) == false) {
				KnockAttack::GetInstance()->SetAttackPhase(true);
				enemy->ChangeState_Boss(new BossEnemyAttackKnock());
			}
		}
	}

	if (Percent::GetParcent(static_cast<float>(enemy->GetMaxHP()), static_cast<float>(enemy->GetHP())) <= 50.0f)
	{
		if (UltAttack::GetIns()->GetPhase() != UltAttack::END)
		{
			if (enemy->GetAttack_End(enemy->ULT) == false) {
				UltAttack::GetIns()->SetAction(true);
				enemy->ChangeState_Boss(new BossEnemyAttackUlt());
			}
		}
	}

	if (Percent::GetParcent(static_cast<float>(enemy->GetMaxHP()), static_cast<float>(enemy->GetHP())) <= 90.0f)
	{
		if (BronzeAttack::GetIns()->GetPhase() != BronzeAttack::END)
		{
			if (enemy->GetAttack_End(enemy->BRONZEATTACK) == false) {
				BronzeAttack::GetIns()->SetAction(true);
				enemy->ChangeState_Boss(new BossEnemyAttackBrzBeam());
			}
		}
	}
	//KnockAttack::GetInstance()->SetAttackPhase(false);
}

void BossEnemyFollow::AttackSelect(Enemy* enemy, bool judg, int num)
{
	if (judg)
	{
		enemy->SetRecvDamage2(false);
		//AttackStart(enemy, num);
	
			AttackType(enemy, num);
		}
}

void BossEnemyFollow::AttackStart(Enemy* enemy, int num)
{
	switch (num)
	{
	case enemy->CIRCLE_1:
		enemy->SetRecvDamage2(false);
		
		break;

	case enemy->KNOCK:
		enemy->SetRecvDamage2(false);
		if (enemy->GetAttack_End(enemy->KNOCK) == false)
		{
			KnockAttack::GetInstance()->SetAttackPhase(true);
		}
			break;

	case enemy->ULT:
		UltAttack::GetIns()->SetAction(true);
		break;
	case enemy->Beam:
		enemy->SetRecvDamage2(false);
		AltAttack::GetInstance()->SetAttackPhase(true);
		break;
	case enemy->Slam:
		FrontCircleAttack::GetInstance()->SetAttackPhase(true);
		break;
	default:
		break;
	}
}

void BossEnemyFollow::AttackType(Enemy* enemy, int num)
{
	switch (num)
	{
	case enemy->CIRCLE_1:
		
		break;
	case enemy->CIRCLE_2:
		if (CircleAttack::GetInstance()->GetPhaseEnd() != CircleAttack::PHASEFOUR)
		{
			//5enemy->ChangeState_Boss(new BossEnemyAttackCircle());
		}
		break;
	case enemy->KNOCK:
		if (KnockAttack::GetInstance()->GetPhase() != KnockAttack::PHASETHREE)
		{
			enemy->ChangeState_Boss(new BossEnemyAttackKnock());
		}
		break;
	case enemy->ULT:
		if (UltAttack::GetIns()->GetPhase() != UltAttack::END)
		{
			enemy->ChangeState_Boss(new BossEnemyAttackUlt());
		}
		break;

	case enemy->Beam:
		if (AltAttack::GetInstance()->GetPhaseEnd() != AltAttack::PHASEFOUR)
		{
			enemy->ChangeState_Boss(new BossEnemyAttackBeam());
		}
		break;

	case enemy->Slam:
		if (FrontCircleAttack::GetInstance()->GetPhaseEnd() != FrontCircleAttack::PHASEFOUR)
		{
			enemy->ChangeState_Boss(new BossEnemyAttackSlam());
		}
		break;
	default:
		break;
	}
}
