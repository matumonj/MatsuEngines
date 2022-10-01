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
	//m_BehaviorData = new BehaviorDatas();
}
/// <summary>
/// デストラクタ
/// </summary>
BossEnemy::~BossEnemy()
{
	//	delete  m_Model;
	delete mob, MobModel;
}

//初期化処理
void BossEnemy::Initialize(DebugCamera* camera)
{
	m_Object = std::make_unique<Object3d>();
	m_Object->Initialize(camera);

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
	
	SlashTex = Texture::Create(93, { 0.0f ,0.0f ,0.0f }, { 100.0f ,100.0f ,1.0f }, { 1.0f ,1.0f ,1.0f ,1.0f });
	SlashTex->CreateTexture();
	SlashTex->SetAnchorPoint({ 0.5f,0.5f });

	//ParticleManager::LoadTexture(4, L"Resources/ParticleTex/Normal.png");
	particleMan = ParticleManager::Create(4, L"Resources/ParticleTex/Attack.png");
	//ParticleManager::LoadTexture(6, L"Resources/ParticleTex/Attack.png");
	particleMan2 = ParticleManager::Create(6, L"Resources/ParticleTex/Attack.png");
	SlashPos = { Position.x,Position.y,Position.z };
}

//更新処理
void BossEnemy::Update(DebugCamera* camera)
{
	//行動遷移
	state_boss->Update(this);

	Action();
	
	//fbxアニメーション制御
	FbxAnimationControl();
	//座標やスケールの反映
	ParameterSet_Fbx(camera);
	//攻撃後のクールタイム設定
	AttackCoolTime();
	//地形当たり判定
	CollisionField(camera);
	SlashPos.z = Position.z;
	if (!SlashF2 && !SlashF) {
		SlashPos.x = Position.x + 5;
		SlashPos.y = Position.y + 20;
		SlashAlpha = 1.0f;
		if (DamageParticleCreateF) {
			SlashF = true;
		}
		//sf = false;
	}
	if (SlashF) {
		if (PlayerControl::GetInstance()->GetPlayer()->GetFbxTime() >= 1.7f) {

			SlashF2 = true;
		}
	}
	if (SlashF2) {
		SlashF = false;
		SlashPos.x -= 0.5f;
		SlashPos.y -= 0.5f;
		if (Position.x+5 - SlashPos.x > 10) {
			SlashF2 = false;
		}
	}
	SlashTex->SetUVMove(false);
	SlashTex->SetBillboard(false);

	SlashTex->Update(camera);
	SlashTex->SetPosition(SlashPos);
	//SlashTex->SetColor({ 1.0f,1.0f,1.0f,1 });
	SlashTex->SetRotation({ 0,180,0 });
	SlashTex->SetScale({ 2.0f ,2.0f ,3.0f });

	SummonGigaBoss(camera);
	//攻撃受けたらパーティクル
	DamageParticleSet();
	

}

//描画処理
void BossEnemy::Draw()
{
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
	Texture::PreDraw();
	if (SlashF2) {
		SlashTex->Draw();
	}
	Texture::PostDraw();


	// 3Dオブジェクト描画前処理
	ImGui::Begin("slp");
	ImGui::Text("%f", SlashPos.x);
	ImGui::End();
}

void BossEnemy::Death()
{
	if (f_time < DeathTime) {
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

		XMFLOAT3 vel2{};
		vel2.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 3.0f;
		vel2.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 3.0f;
		vel2.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 3.0f;

		XMFLOAT3 acc2{};
		const float rnd_acc2 = 0.01f;
		acc2.y = -(float)rand() / RAND_MAX * rnd_acc2;
		//	//追加
		if (DamageParticleCreateF) {
			sf = true;
			particlePos = { Position.x,Position.y + 10,Position.z };
			particleMan->Add(particleLife, particlePos, vel, acc, 3.0f, 0.0f);

			particleMan2->Add(particleLife, { Position.x,Position.y + 10,Position.z }, vel2, acc2, 3.0f, 0.0f);

			if (i == ParticleSize - 1) {
				DamageParticleCreateF = false;
			}
		}

	}

	particleMan2->SetColor({ 1.0f,1.0f,0.2f,0.7f });
	particleMan2->Update(particleMan->NORMAL);
	particleMan->SetColor({ 1.0f,0.2f,0.2f,0.7f });
	particleMan->Update(particleMan->NORMAL);
}

void BossEnemy::DamageTexUpdate(DebugCamera* camera)
{

}

void BossEnemy::SummonGigaBoss(DebugCamera* camera)
{
	
	if (gigantic) {
		GigaBossObj = std::make_unique<f_Object3d>();
		GigaBossObj->Initialize();
		GigaBossObj->SetModel(FbxLoader::GetInstance()->LoadModelFromFile("monster_golem"));
		GigaBossObj->PlayAnimation();

		gigantic = false;
	}
	if (GigaBossObj != nullptr) {
		GigaBossObj->SetPosition({ 0.0f,-190.0f,180.0f});

		GigaBossObj->SetRotation({ -70.0f,180.0f,0.0f });
		GigaBossObj->SetScale({0.6f,0.6f,0.6f});
//GigaBossObj->SetColor({1,1,1,1});
		GigaBossObj->Updata(true);
	}
}