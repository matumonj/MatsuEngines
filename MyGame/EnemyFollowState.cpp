#include "EnemyFollowState.h"
#include"PlayerControl.h"
#include"EnemyAttackState.h"
#include"EnemyDeathState.h"
#include"EnemyKnockState.h"

void EnemyFollowState::Initialize(Enemy* enmey)
{
}

void EnemyFollowState::Update(Enemy* enemy)
{
	//���G�͈�
	const float DetectionRange = 30.0f;
	//�v���C���[�����G�͈͓�������
	bool SearchPlayer = Collision::GetLength(enemy->GetPosition(),
	                                         PlayerControl::GetInstance()->GetPlayer()->GetPosition()) < DetectionRange;

	float dis;
	//�ǐՃX�s�[�h
	float centerSpeed = 0.01f;


	//�G�ƃv���C���[�̋�������
	dis = sqrtf(
		(enemy->GetPosition().x - PlayerControl::GetInstance()->GetPlayer()->GetPosition().x) * (enemy->GetPosition().x
			- PlayerControl::GetInstance()->GetPlayer()->GetPosition().x)
		+ (enemy->GetPosition().z - PlayerControl::GetInstance()->GetPlayer()->GetPosition().z) * (enemy->GetPosition().
			z - PlayerControl::GetInstance()->GetPlayer()->GetPosition().z));


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
	XMVECTOR move = {0, 0, 0.1f, 0};

	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(enemy->GetRotation().y + enemy->GetRotRadians()));

	move = XMVector3TransformNormal(move, matRot);
	//	if (enemy->GetFbxTime()< 510.000f / 60.000f) {
	enemy->SetRotation({
		enemy->GetRotation().x,
		RotY * 60 + enemy->GetRotCorrect(),
		enemy->GetRotation().z
	});

	if (!enemy->GetMoveStop() && PlayerControl::GetInstance()->GetPlayer()->GetStopFlag() == false)
	{
		if (Collision::GetLength(enemy->GetPosition(), PlayerControl::GetInstance()->GetPlayer()->GetPosition()) > 10)
		{
			enemy->SetPosition({
					enemy->GetPosition().x + move.m128_f32[0],
					enemy->GetPosition().y,
					enemy->GetPosition().z + move.m128_f32[2]
				}
			);
			//}
		}

		enemy->SetAnimeState(enemy->WALK);
	}
	//movement++;
	if (enemy->GetRecvDamage2())
	{
		enemy->ChangeState_Mob(new EnemyKnockState());
	}
	if (SearchPlayer && enemy->GetCoolTime() == 0)
	{
		//�ǐ�
		enemy->ChangeState_Mob(new EnemyAttackState());
	}
	if (enemy->GetHP() <= 0.0f)
	{
		enemy->ChangeState_Mob(new EnemyDeathState());
	}
}
