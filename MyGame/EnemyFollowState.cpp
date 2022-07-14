#include "EnemyFollowState.h"

void EnemyFollowState::Initialize(Enemy* enmey)
{

}

void EnemyFollowState::Update(Enemy* enemy)
{
	float angleX, angleZ,dis;
	//�ǐՃX�s�[�h
	float centerSpeed = 0.01f;
	
	angleX = (Player::GetInstance()->GetPosition().x - enemy->GetPosition().x);
	angleZ = (Player::GetInstance()->GetPosition().z - enemy->GetPosition().z);

	//�G�ƃv���C���[�̋�������
	dis = sqrtf((enemy->GetPosition().x - Player::GetInstance()->GetPosition().x) * (enemy->GetPosition().x - Player::GetInstance()->GetPosition().x)
		+ (enemy->GetPosition().z - Player::GetInstance()->GetPosition().z) * (enemy->GetPosition().z - Player::GetInstance()->GetPosition().z));
	
	
	//�G���v�G���C���[�̕���������
	XMVECTOR positionA = { Player::GetInstance()->GetPosition().x,Player::GetInstance()->GetPosition().y, Player::GetInstance()->GetPosition().z };
	XMVECTOR positionB = { enemy->GetPosition().x,enemy->GetPosition().y,enemy->GetPosition().z };
	//�v���C���[�ƓG�̃x�N�g���̒���(��)�����߂�
	XMVECTOR SubVector = DirectX::XMVectorSubtract(positionB, positionA);// positionA - positionB;

	//�p�x�̎擾 �v���C���[���G�̍��G�ʒu�ɓ�������������v���C���[�̕���
	RotY = atan2f(SubVector.m128_f32[0], SubVector.m128_f32[2]);

	enemy->SetRotation({ enemy->GetRotation().x,RotY * 60 + 180,enemy->GetRotation().z });
	//���W�̃Z�b�g
	enemy->SetPosition({ enemy->GetPosition().x + (angleX / dis) * centerSpeed,enemy->GetPosition().y,enemy->GetPosition().z + (angleZ / dis) * centerSpeed });
	
}