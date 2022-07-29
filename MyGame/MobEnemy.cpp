#include "MobEnemy.h"

#include"SphereCollider.h"
#include"CollisionManager.h"
#include"CollisionAttribute.h"
#include"DebugCamera.h"
#include "PhysicsAction.h"
#include "WalkJudgement.h"
#include"WalkAction.h"
#include"StayJudgment.h"
#include"StayAction.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
using namespace DirectX;
MobEnemy::MobEnemy(BehaviorTree* ai_tree, float max_hp, float max_mp, float attack, float deffence) :
Enemy(ai_tree,  max_hp, max_mp, attack, deffence)
{
	
}
/// <summary>
/// �f�X�g���N�^
/// </summary>
MobEnemy::~MobEnemy()
{
	//delete mob, MobModel;
}

//����������
void MobEnemy::Initialize(DebugCamera* camera)
{
	m_Object = std::make_unique<Object3d>();
	m_Object->Initialize(camera);
	m_fbxModel = FbxLoader::GetInstance()->LoadModelFromFile("monster");
	
	RandMove = rand() % 90+20;
	RandMovement = rand() % 100 + 80;
		
	InitAction();
	EnemyHP = 150.0f;
	//���f���̓Ǎ�
	//MobModel = Model::CreateFromOBJ("subenemy");
	//���f�����蓖��camera
	
	Texture::LoadTexture(11, L"Resources/Sertch.png");
	SearchPlayerTexture = Texture::Create(11, { 0,-50,50 }, { 1,1,1 }, { 1,1,1,1 });
	SearchPlayerTexture->CreateTexture();
	//�p�����[�^�̃Z�b�g
	//Scale = { 2,2,2 };
	Rotation = { 0,180,0 };
	//Position = { -12,-5,-2 };

	
	m_fbxObject = std::make_unique<f_Object3d>();
	m_fbxObject->Initialize();
	m_fbxObject->SetModel(m_fbxModel);
	m_fbxObject->PlayAnimation();
	radius_adjustment = 0;
	SetCollider();
}

//�X�V����
void MobEnemy::Update(DebugCamera* camera)
{
	if (FollowFlag) {
		m_fbxObject->SetColor({ 1,0,0,1 });
	}
	else {
		m_fbxObject->SetColor({ 1,1,1,1 });
	}
	Scale={ 0.02f, 0.02f, 0.02f
		};
	//Position = { 0,0,0 };
		Action();

		EnemyPop(150);
		
			m_fbxObject->SeteCurrent(animeflag);
			m_fbxObject->SetScale(Scale);
			m_fbxObject->SetPosition({ Position.x,Position.y-3,Position.z });
		m_fbxObject->SetRotation({ Rotation.x-70, Rotation.y, Rotation.z });
		m_fbxObject->Updata(true);
		m_Object->SetPosition(Position);

		CollisionField(camera);
}

//�`�揈��
void MobEnemy::Draw()
{
	//if (Alive) {
		/*Object3d::PreDraw();
		Object3d::Draw();
		Object3d::PostDraw();*/
	m_fbxObject->Draw();
		Texture::PreDraw();
		//if (distance <= 30) {
			SearchPlayerTexture->Draw();
	//	}
		Texture::PostDraw();
	//}
}
//�������
void MobEnemy::Finalize()
{
	delete SearchPlayerTexture;
	//delete mob, MobModel;
}

float MobEnemy::Distance(Player* player)
{
	distance = sqrtf(((player->GetPosition().x - Position.x) * (player->GetPosition().x - Position.x))
		+ ((player->GetPosition().y - Position.y) * (player->GetPosition().y - Position.y))
		+ ((player->GetPosition().z - Position.z) * (player->GetPosition().z - Position.z)));
	return distance;
}

void MobEnemy::EnemySearchPlayer(Player* player)
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


