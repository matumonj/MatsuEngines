#include "EnemyWalkState.h"
#include"EnemyAttackState.h"
#include"EnemyStayState.h"
#include"EnemyFollowState.h"
#include"Collision.h"
void EnemyWalkState::Initialize(Enemy* enmey)
{

}
void EnemyWalkState::Update(Enemy* enemy)
{
	
	//���G�͈�
	const float DetectionRange = 10.0f;
	//�v���C���[�����G�͈͓�������
	bool SearchPlayer = Collision::GetLength(enemy->GetPosition(), Player::GetInstance()->GetPosition()) < DetectionRange;
	if (Player::GetInstance() != nullptr) {
		SearchPlayer = false;
	}
	//�ړ��x�N�g����y������̊p�x�ŉ�]
	XMVECTOR move = { 0,0,0.1f,0 };
	
	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(enemy->GetRotation().y));
	
	move = XMVector3TransformNormal(move, matRot);

	//�����Ă�����Ɉړ�
	//�����Ă���
	if (enemy->GetonFlag()==true) {
		tempx = enemy->GetPosition().x;
		tempz = enemy->GetPosition().z;
		if (enemy->GetMoveFlag()) {
			enemy->SetPosition({
				enemy->GetPosition().x + move.m128_f32[0],
				enemy->GetPosition().y,
				enemy->GetPosition().z + move.m128_f32[2] }
			);
			movement++;
		}
	
		if (movement > enemy->GetRandMoveMent()) {
			//enemy->ChangeState(new EnemyStayState());
			movement = 0;
		}
	}
	else if (enemy->GetonFlag() == false) {
		if (enemy->GetMoveFlag() != false) {
			enemy->SetPosition({ tempx,enemy->GetPosition().y,tempz });
		}
		else {
			enemy->SetPosition({
							enemy->GetPosition().x ,
							enemy->GetPosition().y,
							enemy->GetPosition().z }
			);

		}
	}
	//�U��
	if (SearchPlayer) {
		//�ǐ�
		
			//enemy->ChangeState(new EnemyFollowState());
	}
}