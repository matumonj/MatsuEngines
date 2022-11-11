#include "BossEnemy.h"
#include"CustomButton.h"
#include"SphereCollider.h"
#include"CollisionManager.h"
#include"CollisionAttribute.h"
#include"DebugCamera.h"
#include"mHelper.h"
#include"PlayerControl.h"
#include"imgui.h"

/// <summary>
/// コンストラクタ
/// </summary>
using namespace DirectX;
BossEnemy::BossEnemy()
{

}
/// <summary>
/// デストラクタ
/// </summary>
BossEnemy::~BossEnemy()
{
	delete mob;
}

//初期化処理
void BossEnemy::Initialize(DebugCamera* camera)
{
	m_Object = std::make_unique<Object3d>();
	m_Object->Initialize(camera);
	//m_Object->CreateGraphicsPipeline(L"Resources/Shader/Object3dVS.hlsl", L"Resources/Shader/Object3dPS.hlsl", L"Resources/Shader/BasicGS.hlsl");
	MaxHP = 200.0f;
	EnemyHP = MaxHP;

	Scale = { 0.04f, 0.04f, 0.04f};
	Rotation = { -70.0f,180.0f,0.0f };

	m_fbxObject = std::make_unique<f_Object3d>();
	m_fbxObject->Initialize();
	m_fbxObject->SetModel(FbxLoader::GetInstance()->LoadModelFromFile("monster_golem"));
	m_fbxObject->PlayAnimation();
	radius_adjustment = 0;
	
	SetCollider();
	cooltime = 0;
	AttackTime = 1.5f;
	DeathTime = 4.9f;
	DeathFlag = false;
	f_time = 200 / 60;

	state_boss->Initialize(this);
	Wand = std::make_unique<Object3d>();
	Wand->Initialize(camera);
	Wand->SetModel(Model::CreateFromOBJ("Wand"));
	Wand->SetRotation({ 0,0 + 30,0 + 100 });

	particleMan = ParticleManager::Create(4, L"Resources/ParticleTex/Attack.png");
	particleMan2 = ParticleManager::Create(6, L"Resources/ParticleTex/Attack.png");
	
}

//更新処理
void BossEnemy::Update(DebugCamera* camera)
{
	//行動遷移
	state_boss->Update(this);
	m_fbxObject->SetHandBoneIndex(19);
	Wand->Setf(FALSE);
	Wand->SetRotation({ -23,43,83 });
	Wand->Update(m_fbxObject->GetRot(), { 1.0f,1.0f,1.0f,1.0f }, camera);

	if (DeathFlag) {
		alpha -= 0.005f;
	}
	Action();
	HandSiteOBB.SetOBBParam_Pos(Wand->GetMatWorld());
	HandSiteOBB.SetOBBParam_Rot(Wand->GetMatWorld());
	HandSiteOBB.SetOBBParam_Scl({ 5.0f,20.0f,5.0f });

	playerOBB.SetOBBParam_Pos(PlayerControl::GetInstance()->GetPlayer()->GetPosition());
	playerOBB.SetOBBParam_Rot(PlayerControl::GetInstance()->GetPlayer()->GetMatrot());
	playerOBB.SetOBBParam_Scl({ 1.0f,5.0f,1.0f });


	if (f_time >= AttackTime + 1.0f) {
		if (Collision::CheckOBBCollision(playerOBB, HandSiteOBB) == true) {
			PlayerControl::GetInstance()->GetPlayer()->RecvDamage(10);
		}
	}
	//fbxアニメーション制御
	FbxAnimationControl();
	//座標やスケールの反映
	ParameterSet_Fbx(camera);
	//攻撃後のクールタイム設定
	AttackCoolTime();
	//地形当たり判定
	CollisionField(camera);
	
	//攻撃受けたらパーティクル
	DamageParticleSet();
	
}

//描画処理
void BossEnemy::Draw()
{
	if (alpha < 0)return;
		Object3d::PreDraw();
		Wand->Draw();
		Object3d::PostDraw();

		Draw_Fbx();
		if (GigaBossObj != nullptr) {
			GigaBossObj->Draw();
		}
		// 3Dオブジェクト描画前処理
		ParticleManager::PreDraw();
		// 3Dオブクジェクトの描画
		particleMan->Draw();
		particleMan2->Draw();
		// 3Dオブジェクト描画後処理
		ParticleManager::PostDraw();

}

void BossEnemy::Death()
{
	if (!DeathFlag) {
		DeathFlag = true;
	}
}


void BossEnemy::FbxAnimationControl()
{
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
void BossEnemy::AttackCoolTime()
{

	if (f_time >= DeathTime - 1) {
		AfterAttack = true;
	}
	if (AfterAttack) {

		cooltime++;
		if (cooltime > 240) {
			AfterAttack = false;
		}
	} else {
		cooltime = 0;
	}
}

void BossEnemy::DamageParticleSet()
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
