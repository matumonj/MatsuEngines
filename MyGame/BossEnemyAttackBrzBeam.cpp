#include "BossEnemyAttackBrzBeam.h"
#include "BossEnemy.h"
#include"BossEnemyFollow.h"
#include "BronzeAttack.h"
#include "Collision.h"
#include"KnockAttack.h"
#include"mHelper.h"

using namespace DirectX;
void BossEnemyAttackBrzBeam::Initialize(Enemy* enmey)
{
}

void BossEnemyAttackBrzBeam::Update(Enemy* enemy)
{
	enemy->SetRecvDamage2(false);
	//BronzeAttack::GetIns()->SetAttackPhase(true);
	
	TurnCenter(enemy);
	if (Percent::GetParcent(static_cast<float>(enemy->GetMaxHP()), static_cast<float>(enemy->GetHP())) <= 90.0f)
	{
		enemy->SetAttack_End(enemy->BRONZEATTACK_W, true);
	}
	if (Percent::GetParcent(static_cast<float>(enemy->GetMaxHP()), static_cast<float>(enemy->GetHP())) <= 99.0f)
	{
		enemy->SetAttack_End(enemy->BRONZEATTACK_H, true);
	}


	if (BronzeAttack::GetIns()->GetPhase() ==BronzeAttack::END)
	{
	enemy->ChangeState_Boss(new BossEnemyFollow());
	}
}

void BossEnemyAttackBrzBeam::TurnCenter(Enemy* enemy)
{
	float RotY; 
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
	if (Collision::GetLength(enemy->GetPosition(),{0.f,0.f,0.f}) > 15.f)
	{

		positionA = { 0.f,0.f,0.f };
		enemy->SetAnimation(BossEnemy::NowAttackMotion::BWALK, true,1.f);
		enemy->SetPosition({
				enemy->GetPosition().x + move.m128_f32[0] * 6.f,
				enemy->GetPosition().y,
				enemy->GetPosition().z + move.m128_f32[2] * 6.f
			}
		);
	}
	else
	{
		positionA = { 0,0,-50 };
			enemy->SetAnimation(BossEnemy::NowAttackMotion::CHARGE, false, 1.0);
	}
}

