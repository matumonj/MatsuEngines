#include "EnemyWalkState.h"
#include"EnemyAttackState.h"
#include"EnemyStayState.h"
#include"EnemyFollowState.h"
#include"Collision.h"
#include"PlayerControl.h"
void EnemyWalkState::Initialize(Enemy* enmey)
{

}
void EnemyWalkState::Update(Enemy* enemy)
{

	float RandMove = rand() % 90 + 20;
	float RandMovement = rand() % 180 + 120;
	//���G�͈�
	const float DetectionRange = 30.0f;

	//�v���C���[�����G�͈͓�������
	if (PlayerControl::GetInstance()->GetPlayer() != nullptr) {
		SearchPlayer = Collision::GetLength(enemy->GetPosition(), PlayerControl::GetInstance()->GetPlayer()->GetPosition()) < DetectionRange;
	}
	//�ړ��x�N�g����y������̊p�x�ŉ�]
	XMVECTOR move = { 0,0,0.1f,0 };
	
	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(enemy->GetRotation().y+enemy->GetRotRadians()));

	move = XMVector3TransformNormal(move, matRot);

	//�����Ă�����Ɉړ�
	//�����Ă���
	if (enemy->GetonFlag()==true) {
		tempx = enemy->GetPosition().x;
		tempz = enemy->GetPosition().z;
		if (!enemy->GetMoveStop()) {
			enemy->SetPosition({
				enemy->GetPosition().x + move.m128_f32[0],
				enemy->GetPosition().y,
				enemy->GetPosition().z + move.m128_f32[2] }
			);
			movement++;
		}
	
		if (movement > RandMovement) {
			enemy->ChangeState_Mob(new EnemyStayState());
			movement = 0;
		}
	}
	
	BacktoGround(enemy);
	//enemy->SetAnimeState(enemy->WALK);
	//�U��
	if (SearchPlayer) {
		//�ǐ�
		enemy->ChangeState_Mob(new EnemyFollowState());
}
}

void EnemyWalkState::BacktoGround(Enemy*enemy)
{
	//�ڒn��ԂłȂ��������Ă���Ƃ�
	if (enemy->GetonFlag())return;
	if (!enemy->GetMoveFlag()) return;

	enemy->SetPosition({ tempx,enemy->GetPosition().y,tempz });
}