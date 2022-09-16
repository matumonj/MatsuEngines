#include "BossEnemyFollow.h"
#include "EnemyFollowState.h"
#include"PlayerControl.h"
#include"CameraControl.h"
#include"BossEnemyAttack.h"
#include"BossEnemyAttackCircle.h"
#include"BossEnemyAttackknock.h"
#include"BossEnemyAttackhalf.h"
#include"mHelper.h"
#include"Input.h"
#include"HalfAttack.h"
#include"CircleAttack.h"
#include"BossEnemyDeath.h"
#include"KnockAttack.h"
void BossEnemyFollow::Initialize(Enemy* enmey)
{

}

void BossEnemyFollow::Update(Enemy* enemy)
{
	//���G�͈�
	const float DetectionRange = 10.0f;
	//�v���C���[�����G�͈͓�������
	bool SearchPlayer = Collision::GetLength(enemy->GetPosition(), PlayerControl::GetInstance()->GetPlayer()->GetPosition()) < DetectionRange;

	float angleX, angleZ, dis;
	//�ǐՃX�s�[�h
	float centerSpeed = 0.01f;

	angleX = (PlayerControl::GetInstance()->GetPlayer()->GetPosition().x - enemy->GetPosition().x);
	angleZ = (PlayerControl::GetInstance()->GetPlayer()->GetPosition().z - enemy->GetPosition().z);

	//�G�ƃv���C���[�̋�������
	dis = sqrtf((enemy->GetPosition().x - PlayerControl::GetInstance()->GetPlayer()->GetPosition().x) * (enemy->GetPosition().x - PlayerControl::GetInstance()->GetPlayer()->GetPosition().x)
		+ (enemy->GetPosition().z - PlayerControl::GetInstance()->GetPlayer()->GetPosition().z) * (enemy->GetPosition().z - PlayerControl::GetInstance()->GetPlayer()->GetPosition().z));


	//�G���v�G���C���[�̕���������
	XMVECTOR positionA = { PlayerControl::GetInstance()->GetPlayer()->GetPosition().x,PlayerControl::GetInstance()->GetPlayer()->GetPosition().y, PlayerControl::GetInstance()->GetPlayer()->GetPosition().z };
	XMVECTOR positionB = { enemy->GetPosition().x,enemy->GetPosition().y,enemy->GetPosition().z };
	//�v���C���[�ƓG�̃x�N�g���̒���(��)�����߂�
	XMVECTOR SubVector = DirectX::XMVectorSubtract(positionB, positionA);// positionA - positionB;

	//�p�x�̎擾 �v���C���[���G�̍��G�ʒu�ɓ�������������v���C���[�̕���
	RotY = atan2f(SubVector.m128_f32[0], SubVector.m128_f32[2]);
	//�ړ��x�N�g����y������̊p�x�ŉ�]
	XMVECTOR move = { 0,0,0.1f,0 };

	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(enemy->GetRotation().y));

	move = XMVector3TransformNormal(move, matRot);

	enemy->SetRotation({ enemy->GetRotation().x,
		 RotY * 60 + 180,
		enemy->GetRotation().z });
	if (Collision::GetLength(enemy->GetPosition(), PlayerControl::GetInstance()->GetPlayer()->GetPosition()) > 10) {
		enemy->SetPosition({
					enemy->GetPosition().x + move.m128_f32[0] ,
					enemy->GetPosition().y,
					enemy->GetPosition().z + move.m128_f32[2] }
		);
	}

	if ( Collision::GetLength(enemy->GetPosition(), PlayerControl::GetInstance()->GetPlayer()->GetPosition()) < 20) {
		if (enemy->GetCoolTime() == 0) {
			enemy->ChangeState_Boss(new BossEnemyAttack());
		}
	}

	AttackSelect(enemy, Percent::GetParcent(enemy->GetMaxHP(), enemy->GetHP()) <= 60.0f, enemy->HALF_1);
	AttackSelect(enemy, Percent::GetParcent(enemy->GetMaxHP(), enemy->GetHP()) <= 40.0f, enemy->CIRCLE_1);
	AttackSelect(enemy, Percent::GetParcent(enemy->GetMaxHP(), enemy->GetHP()) <= 20.0f, enemy->HALF_2);
	AttackSelect(enemy, Percent::GetParcent(enemy->GetMaxHP(), enemy->GetHP()) <= 50.0f, enemy->KNOCK);
	if (enemy->GetHP() <= 0.0f) {
		enemy->ChangeState_Boss(new BossEnemyDeath());
	}
}

void BossEnemyFollow::AttackSelect(Enemy* enemy, bool judg, int num)
{
	if (judg) {
		AttackStart(enemy, num);
		if (enemy->GetAttack_End(num) == false) {
			enemy->SetAttack_Start(num, true);
		} else {
			enemy->SetAttack_Start(num, false);
		}
		if (enemy->GetAttack_Start(num)) {
			AttackType(enemy, num);
		}
	}
}

void BossEnemyFollow::AttackStart(Enemy* enemy, int num)
{
	switch (num)
	{
	case enemy->CIRCLE_1:
		CircleAttack::GetInstance()->SetAttackFase(true);
		break;
	case enemy->CIRCLE_2:
		CircleAttack::GetInstance()->SetAttackFase(true);
		break;
	case enemy->KNOCK:
		KnockAttack::GetInstance()->SetAttackFase(true);
		break;
	case enemy->HALF_1:
		HalfAttack::GetInstance()->SetAttackFase(true);
		break;
	case enemy->HALF_2:
		HalfAttack::GetInstance()->SetAttackFase(true);
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
		if (CircleAttack::GetInstance()->GetFaseEnd() != CircleAttack::FASEFOUR) {
			enemy->ChangeState_Boss(new BossEnemyAttackCircle());
		}
		break;
	case enemy->CIRCLE_2:
		if (CircleAttack::GetInstance()->GetFaseEnd() != CircleAttack::FASEFOUR) {
			enemy->ChangeState_Boss(new BossEnemyAttackCircle());
		}
		break;
	case enemy->KNOCK:
		if (KnockAttack::GetInstance()->GetFase() != KnockAttack::FASETHREE) {
			enemy->ChangeState_Boss(new BossEnemyAttackKnock());
		}
		break;
	case enemy->HALF_1:
		if (HalfAttack::GetInstance()->GetFaseEnd() != HalfAttack::FASEFOUR) {
			enemy->ChangeState_Boss(new BossEnemyAttackHalf());
		}
		break;

	case enemy->HALF_2:
		if (HalfAttack::GetInstance()->GetFaseEnd() != HalfAttack::FASEFOUR) {
			enemy->ChangeState_Boss(new BossEnemyAttackHalf());
		}
		break;
	default:
		break;
	}

}