#include "HalfAttack.h"
#include"EnemyControl.h"
#include"PlayerControl.h"
#include"Collision.h"
void HalfAttack::Initialize()
{

}

void HalfAttack::ActionJudg()
{
//fase1�@�J�E���g�_�E���ƒ����ɖ߂鏈��
	PlayerPos = PlayerControl::GetInstance()->GetPlayer()->GetPosition();
	BossEnemyPos = EnemyControl::GetInstance()->GetTutorialEnemyindex()[0]->GetPosition();

	if (fase1) {
		TurnCenter();
		if (Collision::GetLength(BossEnemyPos, CenterPos) < 10) {
			if (CenterPos.x >= BossEnemyPos.x) {
				Area = LEFT;
			} else {
				Area = RIGHT;
			}
			fase2 = true;
			fase1 = false;
		}
	}

	if (fase2) {
		AttackCount++;
		if (AttackCount >= 120) {
			fase3 = true;
			fase2 = false;
		}
	}

	if (fase3) {
		AttackCount = 0;
	}

	
	switch (Area)
	{
	case HalfAttack::LEFT:
		HalfAreaTex->SetAnchorPoint({ 0,0.5 });
		break;
	case HalfAttack::RIGHT:
		HalfAreaTex->SetAnchorPoint({ 0,0.5 });
		break;
	default:
		break;
	}
}

void HalfAttack::Draw()
{

}


void HalfAttack::TurnCenter()
{
		float angleX, angleZ, dis;
		//�ǐՃX�s�[�h
		float centerSpeed = 0.2f;

		angleX = (CenterPos.x - BossEnemyPos.x);
		angleZ = (CenterPos.z - BossEnemyPos.z);

		//�G�ƃv���C���[�̋�������
		dis = sqrtf((BossEnemyPos.x - CenterPos.x) * (BossEnemyPos.x - CenterPos.x)
			+ (BossEnemyPos.z - CenterPos.z) * (BossEnemyPos.z - CenterPos.z));

		//�G���v�G���C���[�̕���������
		XMVECTOR positionA = { CenterPos.x,CenterPos.y, CenterPos.z };
		XMVECTOR positionB = { BossEnemyPos.x,BossEnemyPos.y,BossEnemyPos.z };
		//�v���C���[�ƓG�̃x�N�g���̒���(��)�����߂�
		XMVECTOR SubVector = DirectX::XMVectorSubtract(positionB, positionA);// positionA - positionB;

		//�p�x�̎擾 �v���C���[���G�̍��G�ʒu�ɓ�������������v���C���[�̕���
		RotY = atan2f(SubVector.m128_f32[0], SubVector.m128_f32[2]);

		EnemyControl::GetInstance()->GetTutorialEnemyindex()[0]->SetRotation({
			EnemyControl::GetInstance()->GetTutorialEnemyindex()[0]->GetRotation().x,
			RotY * 60 + 180,
			EnemyControl::GetInstance()->GetTutorialEnemyindex()[0]->GetRotation().z });
}