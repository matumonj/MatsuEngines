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
bool BossEnemy::Initialize(DebugCamera* camera)
{
	RandMove = rand() % 120 + 90;
	RandMovement = rand() % 180 + 120;

	InitAction();
	EnemyHP = 150.0f;
	//���f���̓Ǎ�
	MobModel = Model::CreateFromOBJ("chr_sword");
	//���f�����蓖��camera
	Object3d::Initialize(camera);
	//mob = new Enemy();
	Object3d::Initialize(camera);
	Object3d::SetModel(MobModel);

	//
	Texture::LoadTexture(11, L"Resources/Sertch.png");
	SearchPlayerTexture = Texture::Create(11, { 0,-50,50 }, { 1,1,1 }, { 1,1,1,1 });
	SearchPlayerTexture->CreateTexture();
	//�p�����[�^�̃Z�b�g
	Scale = { 2,2,2 };
	Rotation = { 0,180,0 };
	Position = { -12,-5,-2 };

	float radius = 5.0f;
	SetCollider(new SphereCollider(XMVECTOR({ 0,radius,0,0 }), radius));
	collider->SetAttribute(COLLISION_ATTR_ALLIES);
	return true;
}

//�X�V����
void BossEnemy::Update(XMFLOAT4 color, DebugCamera* camera)
{

	//Position = { 0,0,0 };
	Action();
	EnemyPop(150);

	Object3d::SetPosition(Position);

	Object3d::SetScale({ 5,5,5 });
	Object3d::SetRotation(Rotation);
	CollisionField(camera);

}

//�`�揈��
void BossEnemy::Draw()
{
	//if (Alive) {
	Object3d::PreDraw();
	Object3d::Draw();
	Object3d::PostDraw();
	Texture::PreDraw();
	//if (distance <= 30) {
	SearchPlayerTexture->Draw();
	//	}
	Texture::PostDraw();
	//}
}
//�������
void BossEnemy::Finalize()
{
	delete SearchPlayerTexture;
	delete mob, MobModel;
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

