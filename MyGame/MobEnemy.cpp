#include "MobEnemy.h"
#include"CustomButton.h"
#include"SphereCollider.h"
#include"CollisionManager.h"
#include"CollisionAttribute.h"
#include"DebugCamera.h"
#include"mHelper.h"
#include"imgui.h"
#include"Collision.h"
#include"PlayerControl.h"
#include<iomanip>

/// <summary>
/// �R���X�g���N�^
/// </summary>
using namespace DirectX;
MobEnemy::MobEnemy()
{
}
/// <summary>
/// �f�X�g���N�^
/// </summary>
MobEnemy::~MobEnemy()
{
	Destroy(state_mob);
	Destroy(particleMan);
	//delete mob, MobModel;
}

//����������
void MobEnemy::Initialize(DebugCamera* camera)
{
	Sword = std::make_unique<Object3d>();
	Sword->Initialize(camera);
	Sword->SetModel(ModelManager::GetIns()->GetModel(ModelManager::BIGSWORD));
	Sword->SetRotation({ 0,0 + 30,0 + 100 });

	m_Object = std::make_unique<Object3d>();
	m_Object->Initialize(camera);
	//m_Object->CreateGraphicsPipeline(L"Resources/Shader/Object3dVS.hlsl", L"Resources/Shader/Object3dPS.hlsl", L"Resources/Shader/BasicGS.hlsl");
	EnemyHP = 200.0f;
	MaxHP = 200.0f;
	//�p�����[�^�̃Z�b�g
	Rotation = { -163.0f,71.0f,-16.0f };
	Scale = { 0.04f, 0.04f, 0.04f };

	m_fbxObject = std::make_unique<f_Object3d>();
	m_fbxObject->Initialize();
	m_fbxObject->SetModel(FbxLoader::GetInstance()->LoadModelFromFile("monster_golem_demo"));
	m_fbxObject->PlayAnimation();
//	m_fbxObject->SetColor({ 1,0,0,alpha });
	//�R���C�_�[����
	radius_adjustment = 0;
	SetCollider();

	//FBX�؂�ւ��̃^�C���w��
	AttackTime = 0.9f;
	DeathTime = 5.9f;

	nowAttack = false;
	nowDeath = false;

	//state������
	state_mob->Initialize(this);

	//
	Texture::LoadTexture(93, L"Resources/ParticleTex/slash.png");
	SlashTex = Texture::Create(93, { 0.0f ,0.0f ,0.0f }, { 100.0f ,100.0f ,1.0f }, { 1.0f ,1.0f ,1.0f ,1.0f });
	SlashTex->CreateTexture();
	SlashTex->SetAnchorPoint({ 0.5f,0.5f });
	SlashPos = { Position.x,Position.y,Position.z };

	//�p�[�e�B�N���Z�b�g
	ParticleManager::LoadTexture(4, L"Resources/ParticleTex/Normal.png");
	particleMan=ParticleManager::Create(4,L"Resources/ParticleTex/Attack.png");
	ParticleManager::LoadTexture(6, L"Resources/ParticleTex/Attack.png");
	particleMan2 = ParticleManager::Create(6, L"Resources/ParticleTex/Attack.png");

}
#include"SceneManager.h"
//�X�V����
void MobEnemy::Update(DebugCamera* camera)
{
	state_mob->Update(this);
	
	if (DeathFlag) {
		alpha -= 0.005f;
	}
	
	SearchPlayer(camera);
	FbxAnimationControl();
	
	EnemyPop(150);
	
	AttackCoolTime();
	

	m_fbxObject->SetColor({ 1,0,1,alpha });
	ParameterSet_Fbx(camera);

	CollisionField(camera);
	
	
	m_fbxObject->SetHandBoneIndex(19);
	Sword->Setf(FALSE);
	Sword->SetRotation({-23,43,83});
	Sword->Update(m_fbxObject->GetRot(), { 1.0f,1.0f,1.0f,1.0f }, camera);

	DamageParticleSet();

	HandMat = m_fbxObject->GetRot();

	if (SceneManager::GetInstance()->GetScene() == SceneManager::TUTORIAL) {
		HandSiteOBB.SetOBBParam_Pos(Sword->GetMatWorld());
		HandSiteOBB.SetOBBParam_Rot(Sword->GetMatWorld());
		HandSiteOBB.SetOBBParam_Scl({ 5.0f,20.0f,5.0f });

		playerOBB.SetOBBParam_Pos(PlayerControl::GetInstance()->GetPlayer()->GetPosition());
		playerOBB.SetOBBParam_Rot(PlayerControl::GetInstance()->GetPlayer()->GetMatrot());
		playerOBB.SetOBBParam_Scl({ 1.0f,5.0f,1.0f });

		if (atcktype == SIDEAWAY) {
			if (f_time >= AttackTime + 1.0f) {
				if (Collision::CheckOBBCollision(playerOBB, HandSiteOBB) == true) {
					PlayerControl::GetInstance()->GetPlayer()->RecvDamage(10);
				}
			}
		}
		if (atcktype ==VERTICAL) {
			if (f_time >= 4.5f&& f_time <= 5.5f) {
				if (Collision::CheckOBBCollision(playerOBB, HandSiteOBB) == true) {
					PlayerControl::GetInstance()->GetPlayer()->RecvDamage(15);
				}
			}
		}
	}
}


//�`�揈��
void MobEnemy::Draw()
{

	if (alpha >= 0.0f) {
		Draw_Fbx();

		// 3D�I�u�W�F�N�g�`��O����
		Object3d::PreDraw();
		Sword->Draw();
		Object3d::PostDraw();

		ParticleManager::PreDraw();
		// 3D�I�u�N�W�F�N�g�̕`��
		particleMan->Draw();
		particleMan2->Draw();
		// 3D�I�u�W�F�N�g�`��㏈��
		ParticleManager::PostDraw();
		Texture::PreDraw();
		if (SlashF2) {
			SlashTex->Draw();
		}
		Texture::PostDraw();
	};
	
	ArrowDraw();
}

#include"ExpPointSystem.h"
void MobEnemy::Death()
{
	if (!DeathFlag) {
		ExpPointSystem::GetInstance()->ExpPoint_Get(10);
		//f_time = DeathTime;
		//if (f_time > DeathTime) {
		DeathFlag = true;
		EnemyHP = MaxHP;
	}
	//if (f_time < DeathTime) {
		//f_time = DeathTime;
//	}
	movestop = false;
	
	//}
}


void MobEnemy::FbxAnimationControl()
{
	//1�t���[���i�߂�
	//if (!movestop) {
		f_time += 0.01f;
	//}
			if (f_AttackFlag) {
				rand_Attackindex = rand() % 100;
				if (rand_Attackindex <= 50) {
					atcktype = SIDEAWAY;
					f_time = AttackTime;
				}
				else {
					atcktype = VERTICAL;
					f_time = 3.7f;
				}
				
				nowAttack = true;
				f_AttackFlag = false;
				
			} else {
				if (DeathFlag == false) {
					if (!nowAttack && f_time >= AttackTime) {
						f_time = 0.0f;
					}
				}
			}

			if (atcktype == SIDEAWAY) {
				if (f_time >3.7f) {
					nowAttack = false;
				}
			}
			else if (atcktype == VERTICAL) {
				if (f_time > DeathTime) {
					nowAttack = false;
				}
			}
	
	m_fbxObject->SetFbxTime(f_time);
}


void MobEnemy::AttackCoolTime()
{
		if (f_time >= DeathTime-1) {
			AfterAttack = true;
		}
	if (AfterAttack) {
		cooltime++;
		if (cooltime > 480) {
			AfterAttack = false;
		}
	}
	else {
		cooltime = 0;
	}
	
}

void MobEnemy::DamageParticleSet()
{

	for (int i = 0; i < ParticleSize; i++) {
		const float rnd_vel = 0.5f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		XMFLOAT3 acc{};
		const float rnd_acc = 0.001f;
		acc.y = -(float)rand() / RAND_MAX * rnd_acc;

		//	//�ǉ�
		if (DamageParticleCreateF) {
			particlePos = { Position.x,Position.y + 10,Position.z };
			particleMan->Add(particleLife, particlePos, vel, acc, 3.0f, 0.0f);
			if (i == ParticleSize - 1) {
				DamageParticleCreateF = false;
			}
		}

	}
	particleMan->SetColor({ 1.0f,0.2f,0.2f,0.7f });
	particleMan->Update(particleMan->NORMAL);

}

void MobEnemy::DamageTexUpdate(DebugCamera* camera)
{
	
}