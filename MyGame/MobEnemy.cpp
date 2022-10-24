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
	

	m_Object = std::make_unique<Object3d>();
	m_Object->Initialize(camera);
	
	EnemyHP = 30.0f;
	MaxHP = 30.0f;
	//�p�����[�^�̃Z�b�g
	Rotation = { -70.0f,180.0f,0.0f };
	Scale = { 0.04f, 0.04f, 0.04f };

	m_fbxObject = std::make_unique<f_Object3d>();
	m_fbxObject->Initialize();
	m_fbxObject->SetModel(FbxLoader::GetInstance()->LoadModelFromFile("monster_golem"));
	m_fbxObject->PlayAnimation();
	m_fbxObject->SetColor({ 1,0,0,alpha });
	//�R���C�_�[����
	radius_adjustment = 0;
	SetCollider();

	//FBX�؂�ւ��̃^�C���w��
	AttackTime = 1.5f;
	DeathTime = 4.9f;

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

//�X�V����
void MobEnemy::Update(DebugCamera* camera)
{
	state_mob->Update(this);
	
	if (EnemyHP <= 0) {
		alpha -= 0.005f;
	}
	HandMat = m_fbxObject->GetRot();

	HandSiteOBB.SetOBBParam_Pos(HandMat);
	HandSiteOBB.SetOBBParam_Rot(HandMat);
	HandSiteOBB.SetOBBParam_Scl({ 2.0f,2.0f,2.0f });
	
	playerOBB.SetOBBParam_Pos(PlayerControl::GetInstance()->GetPlayer()->GetPosition());
	playerOBB.SetOBBParam_Rot(PlayerControl::GetInstance()->GetPlayer()->GetMatrot());
	playerOBB.SetOBBParam_Scl({ 1.0f,1.0f,1.0f });
	
	if (Collision::CheckOBBCollision(playerOBB,HandSiteOBB)==true) {
		PlayerControl::GetInstance()->GetPlayer()->RecvDamage(10);
	}
	SearchPlayer(camera);
	FbxAnimationControl();
	
	EnemyPop(150);
	
	AttackCoolTime();
	
	ParameterSet_Fbx(camera);

	CollisionField(camera);


	DamageParticleSet();
}

//�`�揈��
void MobEnemy::Draw()
{
	Draw_Fbx();
	// 3D�I�u�W�F�N�g�`��O����
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

	ArrowDraw();
}

void MobEnemy::Death()
{
	if (f_time > DeathTime) {
		movestop = false;
		DeathFlag = true;
	}
}


void MobEnemy::FbxAnimationControl()
{
	//1�t���[���i�߂�
	if (!movestop) {
		f_time += 0.02f;
	}
			if (f_AttackFlag) {
				f_time = AttackTime;
				f_AttackFlag = false;
				nowAttack = true;
			} else {
				if (nowDeath == false) {
					if (!nowAttack && f_time >= AttackTime) {
						f_time = 0.0f;
					}
				}
			}

			if (DeathFlag) {
				f_time = DeathTime;
				nowDeath = true;
				DeathFlag = false;
				
			} 
	if (f_time > DeathTime) {
		nowAttack = false;
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


}

void MobEnemy::DamageTexUpdate(DebugCamera* camera)
{
	
}