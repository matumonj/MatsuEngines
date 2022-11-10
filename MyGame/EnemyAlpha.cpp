#include "EnemyAlpha.h"
#include"CustomButton.h"
#include"SphereCollider.h"
#include"CollisionManager.h"
#include"CollisionAttribute.h"
#include"DebugCamera.h"
#include"mHelper.h"
#include"imgui.h"
#include"ExpPointSystem.h"
#include"Collision.h"
#include"PlayerControl.h"
#include"SceneManager.h"
/// <summary>
/// コンストラクタ
/// </summary>
using namespace DirectX;
EnemyAlpha::EnemyAlpha()
{
}
/// <summary>
/// デストラクタ
/// </summary>
EnemyAlpha::~EnemyAlpha()
{
	//delete mob, MobModel;
}

//初期化処理
void EnemyAlpha::Initialize(DebugCamera* camera)
{
	m_Object = std::make_unique<Object3d>();
	m_Object->Initialize(camera);
	//m_fbxModel = ModelManager::GetIns()->GetFBXModel(ModelManager::GOLEM);

	EnemyHP = 65.00f;
	MaxHP = 65.00f;
	//パラメータのセット

	Rotation = {-110,0,0 };
	

	m_fbxObject = std::make_unique<f_Object3d>();
	m_fbxObject->Initialize();
	m_fbxObject->SetModel(FbxLoader::GetInstance()->LoadModelFromFile("sniper_blender"));
	m_fbxObject->PlayAnimation();
	radius_adjustment = 0;
	Scale = { 0.05f, 0.05f, 0.05f
	};
	SetCollider();
	AttackTime = 1.5f;
	DeathTime = 6.f;
	nowAttack = false;
	nowDeath = false;
	m_fbxObject->SetColor({ 1,0,0,alpha });
	state_mob->Initialize(this);
	addRotRadians = -180;
	FollowRotAngleCorrect = 0;
	//パーティクルセット
	ParticleManager::LoadTexture(4, L"Resources/ParticleTex/Normal.png");
	particleMan = ParticleManager::Create(4, L"Resources/ParticleTex/Attack.png");
	ParticleManager::LoadTexture(6, L"Resources/ParticleTex/Attack.png");
	particleMan2 = ParticleManager::Create(6, L"Resources/ParticleTex/Attack.png");

}

//更新処理
void EnemyAlpha::Update(DebugCamera* camera)
{
	state_mob->Update(this);

	if (DeathFlag&&alpha>-1) {
		alpha -= 0.005f;
	}
	m_fbxObject->SetColor({ 1,0,0,alpha });

	if (!DeathFlag&& SceneManager::GetInstance()->GetScene() != SceneManager::MAPCREATE) {
		m_fbxObject->SetFogPos(PlayerControl::GetInstance()->GetPlayer()->GetPosition());

		HandSiteOBB.SetOBBParam_Pos(m_fbxObject->GetWorld());
		HandSiteOBB.SetOBBParam_Rot(m_fbxObject->GetWorld());
		HandSiteOBB.SetOBBParam_Scl({ 25.0f,200.0f,250.0f });

		playerOBB.SetOBBParam_Pos(PlayerControl::GetInstance()->GetPlayer()->GetPosition());
		playerOBB.SetOBBParam_Rot(PlayerControl::GetInstance()->GetPlayer()->GetMatrot());
		playerOBB.SetOBBParam_Scl({ 1.0f,5.0f,1.0f });

		//if (f_time >= AttackTime + 1.0f) {
				if (Collision::CheckOBBCollision(playerOBB, HandSiteOBB) == true) {
					PlayerControl::GetInstance()->GetPlayer()->RecvDamage(10);
				}
		//}
	}
	FbxAnimationControl();
	EnemyPop(150);
	AttackCoolTime();
	ParameterSet_Fbx2(camera);

	CollisionField(camera);

	DamageParticleSet();
}

//描画処理
void EnemyAlpha::Draw()
{
	if (alpha > 0) {
		Draw_Fbx();
	}
	ParticleManager::PreDraw();
	// 3Dオブクジェクトの描画
	particleMan->Draw();
	particleMan2->Draw();
	// 3Dオブジェクト描画後処理
	ParticleManager::PostDraw();

}

void EnemyAlpha::Death()
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
}


void EnemyAlpha::FbxAnimationControl()
{
	//1フレーム進める
	f_time += 0.02f;

	if (f_AttackFlag) {
		f_time = AttackTime;
		f_AttackFlag = false;
		nowAttack = true;
	} else {
		if (DeathFlag == false) {
			if (!nowAttack && f_time >= AttackTime) {
				f_time = 0.0f;
			}
		}
	}

	
	if (f_time > DeathTime) {
		nowAttack = false;
	}

	m_fbxObject->SetFbxTime(f_time);
}


void EnemyAlpha::AttackCoolTime()
{
	if (f_time >= DeathTime - 1) {
		AfterAttack = true;
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

void EnemyAlpha::DamageParticleSet()
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

void EnemyAlpha::DamageTexUpdate(DebugCamera* camera)
{
	
}