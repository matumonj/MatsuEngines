#include "MobEnemy.h"
#include"CustomButton.h"
#include"SphereCollider.h"
#include"CollisionManager.h"
#include"CollisionAttribute.h"
#include"DebugCamera.h"
#include"mHelper.h"
#include"imgui.h"
#include"Collision.h"
/// <summary>
/// コンストラクタ
/// </summary>
using namespace DirectX;
MobEnemy::MobEnemy()
{
}
/// <summary>
/// デストラクタ
/// </summary>
MobEnemy::~MobEnemy()
{
	//delete mob, MobModel;
}

//初期化処理
void MobEnemy::Initialize(DebugCamera* camera)
{
	m_Object = std::make_unique<Object3d>();
	m_Object->Initialize(camera);
	//m_fbxModel = ModelManager::GetIns()->GetFBXModel(ModelManager::GOLEM);

	EnemyHP = 150.00f;
	MaxHP = 150.00f;
	//パラメータのセット

	Rotation = { -70,180,0 };
	

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
	nowAttack = false;
	nowDeath = false;
	m_fbxObject->SetColor({ 1,0,0,alpha });
	state_mob->Initialize(this);

	particleMan = ParticleManager::Create();
	particleMan->Update();

}

//更新処理
void MobEnemy::Update(DebugCamera* camera)
{
	state_mob->Update(this);
	if (EnemyHP <= 0) {
		alpha -= 0.005f;
	}
	if (CustomButton::GetInstance()->GetAttackAction()) {
		//RecvDamage(10);
	}
	FbxAnimationControl();
	EnemyPop(150);
	
	AttackCoolTime();
	ParameterSet_Fbx(camera);

	CollisionField(camera);
	
	for (int i = 0; i < ParticleSize; i++) {
		const float rnd_vel = 0.5f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		XMFLOAT3 acc{};
		const float rnd_acc = 0.001f;
		acc.y = -(float)rand() / RAND_MAX * rnd_acc;

		//追加
		if (DamageParticleCreateF) {
			particleMan->Add(80, { Position.x,Position.y + 10,Position.z }, vel, acc, 3.0f, 0.0f);
			if (i == ParticleSize-1) {
				DamageParticleCreateF = false;
			}
		}
		
	}
	particleMan->Update();
}

//描画処理
void MobEnemy::Draw()
{
	Draw_Fbx();
	// 3Dオブジェクト描画前処理
	ParticleManager::PreDraw();

	// 3Dオブクジェクトの描画
	particleMan->Draw();

	// 3Dオブジェクト描画後処理
	ParticleManager::PostDraw();
}

void MobEnemy::Death()
{
	if (f_time < DeathTime) {
		DeathFlag = true;
	}
}


void MobEnemy::FbxAnimationControl()
{
	//1フレーム進める
			f_time += 0.02f;
		
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

