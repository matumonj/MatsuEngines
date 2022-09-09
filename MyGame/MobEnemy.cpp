#include "MobEnemy.h"

#include"SphereCollider.h"
#include"CollisionManager.h"
#include"CollisionAttribute.h"
#include"DebugCamera.h"
#include"mHelper.h"
#include"imgui.h"
#include"Collision.h"
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
	//delete mob, MobModel;
}

//����������
void MobEnemy::Initialize(DebugCamera* camera)
{
	m_Object = std::make_unique<Object3d>();
	m_Object->Initialize(camera);
	m_fbxModel = FbxLoader::GetInstance()->LoadModelFromFile("monster_golem_demo");

	EnemyHP = 15.00f;
	MaxHP = 15.00f;
	Texture::LoadTexture(11, L"Resources/Sertch.png");
	//�p�����[�^�̃Z�b�g

	Rotation = { -70,180,0 };
	

	m_fbxObject = std::make_unique<f_Object3d>();
	m_fbxObject->Initialize();
	m_fbxObject->SetModel(m_fbxModel);
	m_fbxObject->PlayAnimation();
	radius_adjustment = 0;
	Scale = { 0.04f, 0.04f, 0.04f
	};
	SetCollider();
	AttackTime = 1.5f;
	DeathTime = 4.9f;
	m_fbxObject->SetColor({ 1,0,0,alpha });
	state_mob->Initialize(this);
}

//�X�V����
void MobEnemy::Update(DebugCamera* camera)
{
	

	Action();
	
	state_mob->Update(this);
	if (EnemyHP <= 0) {
		alpha -= 0.005f;
	}

	FbxAnimationControl();
	EnemyPop(150);
	
	//m_fbxObject->SeteCurrent(animeflag);
	AttackCoolTime();
	ParameterSet_Fbx(camera);

	CollisionField(camera);
	
}

//�`�揈��
void MobEnemy::Draw()
{
	Draw_Fbx();
	Texture::PreDraw();
	//SearchPlayerTexture->Draw();
	Texture::PostDraw();
}
//�������
void MobEnemy::Finalize()
{
	//delete SearchPlayerTexture;
	//delete mob, MobModel;
}


void MobEnemy::Death()
{
	if (f_time < DeathTime) {
		DeathFlag = true;
	}
}


void MobEnemy::FbxAnimationControl()
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
				f_time = DeathTime;
				nowDeath = true;
				DeathFlag = false;
				
			} else {
				if (!AttackFlag && !nowDeath && f_time >= end_time) {
					//f_time = 0;
				}
			}
	if (f_time > DeathTime) {
		nowAttack = false;
	}
	
	m_fbxObject->SetFbxTime(f_time);
}


void MobEnemy::AttackCoolTime()
{
	if(state == NOW_ATTACK){

		if (f_time >= DeathTime-1) {
			AfterAttack = true;
			state =AFTER_ATTACK;
		}
	}
	if (AfterAttack) {
		
		cooltime++;
		if (cooltime > 480) {
			state = None;
			AfterAttack = false;
		}
	}
	else {
		cooltime = 0;
	}
	
}

