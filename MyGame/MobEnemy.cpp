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
	
	//mob = new Enemy();
	//Object3d::Initialize(camera);
	//Object3d::SetModel(MobModel);
	
	//
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



void MobEnemy::CollisionField(DebugCamera* camera)
{

	// ���[���h�s��X�V
	UpdateWorldMatrix();

	// ��������
	if (!onGround) {
		// �����������x
		const float fallAcc = -0.01f;
		const float fallVYMin = -0.5f;
		// ����
		fallV.m128_f32[1] = max(fallV.m128_f32[1] + fallAcc, fallVYMin);
		// �ړ�
		//position.x += fallV.m128_f32[0];
		position.y += fallV.m128_f32[1];
		//position.z += fallV.m128_f32[2];

	}

	// ���[���h�s��X�V
	UpdateWorldMatrix();
	collider->Update();

	SphereCollider* sphereCollider = dynamic_cast<SphereCollider*>(collider);
	assert(sphereCollider);

	// �N�G���[�R�[���o�b�N�N���X
	class PlayerQueryCallback : public QueryCallback
	{
	public:
		PlayerQueryCallback(Sphere* sphere) : sphere(sphere) {};

		// �Փˎ��R�[���o�b�N�֐�
		bool OnQueryHit(const QueryHit& info) {

			const XMVECTOR up = { 0,1,0,0 };

			XMVECTOR rejectDir = XMVector3Normalize(info.reject);
			float cos = XMVector3Dot(rejectDir, up).m128_f32[0];

			// �n�ʔ��肵�����l
			const float threshold = cosf(XMConvertToRadians(30.0f));

			if (-threshold < cos && cos < threshold) {
				sphere->center += info.reject;
				move += info.reject;
			}

			return true;
		}

		Sphere* sphere = nullptr;
		DirectX::XMVECTOR move = {};
	};
	
	PlayerQueryCallback callback(sphereCollider);

	// ���ƒn�`�̌�����S����
	CollisionManager::GetInstance()->QuerySphere(*sphereCollider, &callback, COLLISION_ATTR_LANDSHAPE);
	// �����ɂ��r�˕�������
	position.x += callback.move.m128_f32[0];
	position.y += callback.move.m128_f32[1];
	position.z += callback.move.m128_f32[2];
	// ���[���h�s��X�V
	UpdateWorldMatrix();
	collider->Update();

	// ���̏�[���狅�̉��[�܂ł̃��C�L���X�g
	Ray ray;
	ray.start = sphereCollider->center;
	ray.start.m128_f32[1] += sphereCollider->GetRadius();
	ray.dir = { 0,-1,0,0 };
	RaycastHit raycastHit;

	// �ڒn���
	if (onGround) {
		// �X���[�Y�ɍ������ׂ̋z������
		const float adsDistance = 5.0f;
		// �ڒn���ێ�
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.0f + adsDistance)) {
			onGround = true;
			position.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
		}
		// �n�ʂ��Ȃ��̂ŗ���
		else {
			onGround = false;
			fallV = {};
		}
	}
	// �������
	else if (fallV.m128_f32[1] <= 0.0f) {
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.0f)) {
			// ���n
			onGround = true;
			position.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
		}
	}
	//RecvDamagef = false;
	// �s��̍X�V�Ȃ�
	Object3d::Update({ 1,1,1,1 }, camera);

}

