#include "EnemyReturnCreatePos.h"
#include "EnemyFollowState.h"
#include"PlayerControl.h"
#include"EnemyAttackState.h"
#include"EnemyDeathState.h"
#include"EnemyKnockState.h"
#include "EnemyStayState.h"

void EnemyReturnCreatePos::Initialize(Enemy* enmey)
{
}

void EnemyReturnCreatePos::Update(Enemy* enemy)
{
	//���G�͈�
	const float DetectionRange = 15.0f;
	//�v���C���[�����G�͈͓�������
	bool SearchPlayer = Collision::GetLength(enemy->GetPosition(),
	                                         PlayerControl::GetIns()->GetPlayer()->GetPosition()) < DetectionRange;

	float dis;
	//�ǐՃX�s�[�h
	float centerSpeed = 0.01f;


	//�G�ƃv���C���[�̋�������
	dis = sqrtf(
		(enemy->GetPosition().x - PlayerControl::GetIns()->GetPlayer()->GetPosition().x) * (enemy->GetPosition().x
			- PlayerControl::GetIns()->GetPlayer()->GetPosition().x)
		+ (enemy->GetPosition().z - PlayerControl::GetIns()->GetPlayer()->GetPosition().z) * (enemy->GetPosition().
			z - PlayerControl::GetIns()->GetPlayer()->GetPosition().z));


	//�G���v�G���C���[�̕���������
	XMVECTOR positionA = {
		enemy->GetCreatePos().x,
		enemy->GetCreatePos().y,
		enemy->GetCreatePos().z
	};
	XMVECTOR positionB = {enemy->GetPosition().x, enemy->GetPosition().y, enemy->GetPosition().z};
	//�v���C���[�ƓG�̃x�N�g���̒���(��)�����߂�
	XMVECTOR SubVector = XMVectorSubtract(positionB, positionA); // positionA - positionB;

	//�p�x�̎擾 �v���C���[���G�̍��G�ʒu�ɓ�������������v���C���[�̕���
	RotY = atan2f(SubVector.m128_f32[0], SubVector.m128_f32[2]);
	//�ړ��x�N�g����y������̊p�x�ŉ�]
	XMVECTOR move = {0, 0, 0.1f, 0};

	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(enemy->GetRotation().y + enemy->GetRotRadians()));

	move = XMVector3TransformNormal(move, matRot);
	//	if (enemy->GetFbxTime()< 510.000f / 60.000f) {
	enemy->SetRotation({
		enemy->GetRotation().x,
		RotY * 60 + enemy->GetRotCorrect(),
		enemy->GetRotation().z
	});


	if (Collision::GetLength(enemy->GetPosition(), enemy->GetCreatePos()) > 10.f)
	{
		enemy->SetPosition({
				enemy->GetPosition().x + move.m128_f32[0] * 4.f,
				enemy->GetPosition().y,
				enemy->GetPosition().z + move.m128_f32[2] * 4.f
			}
		);
		//}
	}


	else
	{
		enemy->SetReturnCreatePos(false);
		enemy->ChangeState_Mob(new EnemyStayState());
	}

	enemy->SetAnimeState(enemy->WALK);
}
