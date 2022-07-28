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
bool MobEnemy::Initialize(DebugCamera* camera)
{

	MobModel = FbxLoader::GetInstance()->LoadModelFromFile("monster");
	
	RandMove = rand() % 90+20;
	RandMovement = rand() % 100 + 80;
		
	InitAction();
	EnemyHP = 150.0f;
	//���f���̓Ǎ�
	//MobModel = Model::CreateFromOBJ("subenemy");
	//���f�����蓖��camera
	Object3d::Initialize(camera);

	Texture::LoadTexture(11, L"Resources/Sertch.png");
	SearchPlayerTexture = Texture::Create(11, { 0,-50,50 }, { 1,1,1 }, { 1,1,1,1 });
	SearchPlayerTexture->CreateTexture();
	//�p�����[�^�̃Z�b�g
	//Scale = { 2,2,2 };
	Rotation = { 0,180,0 };
	//position = { -12,-5,-2 };

	
	MobObject = new f_Object3d();
	MobObject->Initialize();
	MobObject->SetModel(MobModel);
	MobObject->PlayAnimation();


	float radius = 5.0f;
	SetCollider(new SphereCollider(XMVECTOR({ 0,radius,0,0 }), radius));
	collider->SetAttribute(COLLISION_ATTR_ALLIES);

	return true;
}

//�X�V����
void MobEnemy::Update(XMFLOAT4 color, DebugCamera* camera)
{
	if (FollowFlag) {
		MobObject->SetColor({ 1,0,0,1 });
	}
	else {
		MobObject->SetColor({ 1,1,1,1 });
	}
	scale={ 0.02f, 0.02f, 0.02f
		};
	//position = { 0,0,0 };
		Action();

		EnemyPop(150);
		
		//Object3d::SetPosition(position);
		//if (animeflag) {
			MobObject->SeteCurrent(animeflag);
		//}
			MobObject->SetScale(scale);
		//Object3d::SetRotation(Rotation);
			//rotation = { 0,0,0 };
			MobObject->SetPosition({ position.x,position.y-3,position.z });
		MobObject->SetRotation({ rotation.x-70, rotation.y, rotation.z });
		MobObject->Updata(true);


		CollisionField(camera);
}

//�`�揈��
void MobEnemy::Draw()
{
	//if (Alive) {
		/*Object3d::PreDraw();
		Object3d::Draw();
		Object3d::PostDraw();*/
	MobObject->Draw();
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
	distance = sqrtf(((player->GetPosition().x - position.x) * (player->GetPosition().x - position.x))
		+ ((player->GetPosition().y - position.y) * (player->GetPosition().y - position.y))
		+ ((player->GetPosition().z - position.z) * (player->GetPosition().z - position.z)));
	return distance;
}

void MobEnemy::EnemySearchPlayer(Player* player)
{
	//�v���C���[�ƓG�̋������v�Z
	float distance;
	distance = sqrtf(((position.x - player->GetPosition().x) * (position.x - player->GetPosition().x)) +
		((position.y - player->GetPosition().y) * (position.y - player->GetPosition().y)) +
		((position.z - player->GetPosition().z) * (position.z - player->GetPosition().z)));



	XMVECTOR positionA = { player->GetPosition().x, player->GetPosition().y, player->GetPosition().z };
	XMVECTOR positionB = { position.x,position.y,position.z };
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


