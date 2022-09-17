#include "BossEnemy.h"
#include"CustomButton.h"
#include"SphereCollider.h"
#include"CollisionManager.h"
#include"CollisionAttribute.h"
#include"DebugCamera.h"
#include"mHelper.h"
/// <summary>
/// �R���X�g���N�^
/// </summary>
using namespace DirectX;
BossEnemy::BossEnemy()
{
	//m_BehaviorData = new BehaviorDatas();
}
/// <summary>
/// �f�X�g���N�^
/// </summary>
BossEnemy::~BossEnemy()
{
	//	delete  m_Model;
	delete mob, MobModel;
}

//����������
void BossEnemy::Initialize(DebugCamera* camera)
{
	m_Object = std::make_unique<Object3d>();
	m_Object->Initialize(camera);

	MaxHP = 300.0f;
	EnemyHP = MaxHP;

	Scale = { 0.04f, 0.04f, 0.04f};
	Rotation = { -70.0f,180.0f,0.0f };

	m_fbxObject = std::make_unique<f_Object3d>();
	m_fbxObject->Initialize();
	m_fbxObject->SetModel(FbxLoader::GetInstance()->LoadModelFromFile("monster_golem"));
	m_fbxObject->PlayAnimation();
	radius_adjustment = 0;
	
	SetCollider();
	AttackTime = 1.5f;
	DeathTime = 4.9f;
	DeathFlag = false;
	f_time = 200 / 60;

	state_boss->Initialize(this);

	particleMan = ParticleManager::Create();
	
}

//�X�V����
void BossEnemy::Update(DebugCamera* camera)
{
	Action();
	
	//fbx�A�j���[�V��������
	FbxAnimationControl();
	EnemyPop(150);
	//���W��X�P�[���̔��f
	ParameterSet_Fbx(camera);
	//�U����̃N�[���^�C���ݒ�
	AttackCoolTime();
	//�n�`�����蔻��
	CollisionField(camera);
	//�U���󂯂���p�[�e�B�N��
	DamageParticleSet();
	//�s���J��
	state_boss->Update(this);
}

//�`�揈��
void BossEnemy::Draw()
{
	Draw_Fbx();
	// 3D�I�u�W�F�N�g�`��O����
	ParticleManager::PreDraw();
	// 3D�I�u�N�W�F�N�g�̕`��
	particleMan->Draw();
	// 3D�I�u�W�F�N�g�`��㏈��
	ParticleManager::PostDraw();
}

void BossEnemy::Death()
{
	if (f_time < DeathTime) {
		DeathFlag = true;
	}
}


void BossEnemy::FbxAnimationControl()
{
	const float timespeed = 0.02f;
	//�A�j���[�V����
	f_time += timespeed;;
	//�Ō�܂ōĐ�������擪�ɖ߂�

	if (f_AttackFlag) {
		f_time = AttackTime;
		f_AttackFlag = false;
		nowAttack = true;
	} else {
		if (nowDeath == false) {
			if (!nowAttack && f_time >= AttackTime) {
				f_time = 0;
			}
		}
	}

	if (DeathFlag) {
		nowDeath = true;
		f_time = DeathTime;
		DeathFlag = false;

	}
	if (f_time > DeathTime) {
		//nowDeath = false;
		nowAttack = false;
	}

	m_fbxObject->SetFbxTime(f_time);

}
void BossEnemy::AttackCoolTime()
{
	const int CoolMax = 480;
	//fbxtime�����S���[�V�����܂ł�������
	if (f_AttackFlag) {
		if (f_time >= DeathTime - 1) {
			AfterAttack = true;
		}
	}
	if (AfterAttack) {
		cooltime++;
		if (cooltime >CoolMax) {
			AfterAttack = false;
		}
	} else {
		cooltime = 0;
	}
}

void BossEnemy::DamageParticleSet()
{
	const int ParticleSize = 20;
	for (int i = 0; i < ParticleSize; i++) {
		const float rnd_vel = 0.5f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		XMFLOAT3 acc{};
		const float rnd_acc = 0.001f;
		acc.y = -(float)rand() / RAND_MAX * rnd_acc;

		//�ǉ�
		if (DamageParticleCreateF) {
			particleMan->Add(80, { Position.x,Position.y + 10,Position.z }, vel, acc, 3.0f, 0.0f);
			if (i == ParticleSize - 1) {
				DamageParticleCreateF = false;
			}
		}

	}
	particleMan->Update(particleMan->ABSORPTION);
}