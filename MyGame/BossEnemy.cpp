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

	EnemyHP = 300.0f;

	Rotation = { -70,180,0 };

	MaxHP = 300.0f;
	m_fbxObject = std::make_unique<f_Object3d>();
	m_fbxObject->Initialize();
	m_fbxObject->SetModel(FbxLoader::GetInstance()->LoadModelFromFile("monster_golem"));
	m_fbxObject->PlayAnimation();
	radius_adjustment = 0;
	Scale = { 0.04f, 0.04f, 0.04f
	};
	SetCollider();
	AttackTime = 1.5f;
	DeathTime = 4.9f;
	DeathFlag = false;
	f_time = 200 / 60;
	state_boss->Initialize(this);

	particleMan = ParticleManager::Create();
	particleMan->Update();
}

//�X�V����
void BossEnemy::Update(DebugCamera* camera)
{
	Action();
	if (CustomButton::GetInstance()->GetAttackAction()) {
		EnemyHP -= 10;
	}
	FbxAnimationControl();
	EnemyPop(150);

	ParameterSet_Fbx(camera);
	AttackCoolTime();
	CollisionField(camera);

	DamageParticleSet();
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
	//�A�j���[�V����
		//1�t���[���i�߂�
	f_time += 0.02;
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
	if (f_AttackFlag) {

		if (f_time >= DeathTime - 1) {
			AfterAttack = true;
		}
	}
	if (AfterAttack) {
		cooltime++;
		if (cooltime > 480) {
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
	particleMan->Update();
}