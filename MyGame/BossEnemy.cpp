#include "BossEnemy.h"

#include"SphereCollider.h"
#include"CollisionManager.h"
#include"CollisionAttribute.h"
#include"DebugCamera.h"
/// <summary>
/// �R���X�g���N�^
/// </summary>
using namespace DirectX;
BossEnemy::BossEnemy(BehaviorTree* ai_tree, float max_hp, float max_mp, float attack, float deffence) :
	Enemy(ai_tree, max_hp, max_mp, attack, deffence)
{
	//m_BehaviorData = new BehaviorDatas();
}
/// <summary>
/// �f�X�g���N�^
/// </summary>
BossEnemy::~BossEnemy()
{
	delete mob, MobModel;
}

//����������
void BossEnemy::Initialize(DebugCamera* camera)
{
	SetCollider();
}

//�X�V����
void BossEnemy::Update( DebugCamera* camera)
{

}

//�`�揈��
void BossEnemy::Draw()
{
	
}
//�������
void BossEnemy::Finalize()
{

}

float BossEnemy::Distance(Player* player)
{
	distance = sqrtf(((player->GetPosition().x - Position.x) * (player->GetPosition().x - Position.x))
		+ ((player->GetPosition().y - Position.y) * (player->GetPosition().y - Position.y))
		+ ((player->GetPosition().z - Position.z) * (player->GetPosition().z - Position.z)));
	return distance;
}

void BossEnemy::EnemySearchPlayer(Player* player)
{
	//�v���C���[�ƓG�̋������v�Z
	float distance;
	distance = sqrtf(((Position.x - player->GetPosition().x) * (Position.x - player->GetPosition().x)) +
		((Position.y - player->GetPosition().y) * (Position.y - player->GetPosition().y)) +
		((Position.z - player->GetPosition().z) * (Position.z - player->GetPosition().z)));



	XMVECTOR positionA = { player->GetPosition().x, player->GetPosition().y, player->GetPosition().z };
	XMVECTOR positionB = { Position.x,Position.y,Position.z };
	//�v���C���[�ƓG�̃x�N�g���̒���(��)�����߂�
	XMVECTOR SubVector = DirectX::XMVectorSubtract(positionB, positionA);// positionA - positionB;
	if (SearchPlayerFlag == TRUE) {
		//�ǐ�
		//Follow(player);
		//�p�x�̎擾 �v���C���[���G�̍��G�ʒu�ɓ�������������v���C���[�̕���
		rotx = atan2f(SubVector.m128_f32[0], SubVector.m128_f32[2]);

		Rotation.y = rotx * 60 + 180;//60=�p�x�����p 180=���]
	}

}

