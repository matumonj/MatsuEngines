#include "GuardianFollowState.h"
#include"GuardianNAttackState.h"
#include "PlayerControl.h"
#include"GuardianBomState.h"
#include"GuardianDeathState.h"
#include"GuardianBomAttack.h"

void GuardianFollowState::Initialize(Enemy* enemy)
{
}

void GuardianFollowState::Update(Enemy* enemy)
{
	GuardianBomAttack::GetIns()->SetActionNon();
	Follow(enemy);
}

void GuardianFollowState::Follow(Enemy* enemy)
{
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
	XMVECTOR move = {0.f, 0.f, 0.1f, 0};

	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(enemy->GetRotation().y + enemy->GetRotRadians()));

	move = XMVector3TransformNormal(move, matRot);
	//	if (enemy->GetFbxTime()< 510.000f / 60.000f)
	XMFLOAT3 rot = enemy->GetRotation();
	enemy->SetRotation({
		rot.x += 1.5f,
		RotY * 60.f + 180.f,
		rot.z
	});

	PosYMovingT++;

	if (!enemy->GetMoveStop() && PlayerControl::GetInstance()->GetPlayer()->GetStopFlag() == false)
	{
		enemy->SetPosition({
				enemy->GetPosition().x + move.m128_f32[0] * 2.f,
				-20.f + sinf(3.14f * 2.f / 120.f * PosYMovingT) * 2.f,
				enemy->GetPosition().z + move.m128_f32[2] * 2.f
			}
		);
	}

	if (Collision::GetLength(enemy->GetPosition(), PlayerControl::GetInstance()->GetPlayer()->GetPosition()) > 10)
	{
		//	enemy->ChangeState_Guardian(new GuardianBomState());
	}

	if (enemy->GetHP() <= 0)
	{
		enemy->ChangeState_Guardian(new GuardianDeathState());
	}
}
