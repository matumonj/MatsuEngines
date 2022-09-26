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
	Destroy(state_mob);
	Destroy(particleMan);
	//delete mob, MobModel;
}

//初期化処理
void MobEnemy::Initialize(DebugCamera* camera)
{
	

	m_Object = std::make_unique<Object3d>();
	m_Object->Initialize(camera);
	//m_fbxModel = ModelManager::GetIns()->GetFBXModel(ModelManager::GOLEM);

	EnemyHP = 30.00f;
	MaxHP = 30.00f;
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
	Texture::LoadTexture(93, L"Resources/ParticleTex/slash.png");

	SlashTex = Texture::Create(93, { 0.0f ,0.0f ,0.0f }, { 100.0f ,100.0f ,1.0f }, { 1.0f ,1.0f ,1.0f ,1.0f });
	SlashTex->CreateTexture();
	SlashTex->SetAnchorPoint({ 0.5f,0.5f });

	ParticleManager::LoadTexture(4, L"Resources/ParticleTex/Normal.png");
	particleMan=ParticleManager::Create(4,L"Resources/ParticleTex/Attack.png");
	ParticleManager::LoadTexture(6, L"Resources/ParticleTex/Attack.png");
	particleMan2 = ParticleManager::Create(6, L"Resources/ParticleTex/Attack.png");
	SlashPos = { Position.x,Position.y,Position.z };
	
	Damagetxt = std::make_unique<DebugTxt>();
	Damagetxt->Initialize(47);

}

//更新処理
void MobEnemy::Update(DebugCamera* camera)
{
	state_mob->Update(this);
	if (EnemyHP <= 0) {
		alpha -= 0.005f;
	}
	
	FbxAnimationControl();
	EnemyPop(150);
	
	AttackCoolTime();
	ParameterSet_Fbx(camera);

	CollisionField(camera);

	SlashPos.z = Position.z;
	if (DamageParticleCreateF&&!SlashF2) {
		SlashPos.x = Position.x + 5;
		SlashPos.y = Position.y + 10;
		SlashAlpha = 0.0f;
		SlashF = true;
	}
	if (SlashF && !SlashF2) {
		if (PlayerControl::GetInstance()->GetPlayer()->GetFbxTime() >= 1.7f) {
			
			SlashAlpha = 1.0f;
			SlashF2 = true;
		}
	}
	if(SlashF2){
		SlashF = false;
		SlashPos.x -= 1.0f;
		SlashPos.y -= 1.0f;
			SlashAlpha -= 0.05f;
			if (SlashAlpha <= 0.0f) {
			SlashF2 = false;
		}
	}
	if (!SlashF && !SlashF2) {
		SlashAlpha = 0.0f;
	}
	SlashTex->SetUVMove(false);
	SlashTex->SetBillboard(false);

	SlashTex->Update(camera);
	SlashTex->SetPosition(SlashPos);
	SlashTex->SetColor({ 1.0f,1.0f,1.0f,1 });
	SlashTex->SetRotation({0,180,0});
	SlashTex->SetScale({ 2.0f ,2.0f ,3.0f });

	DamageParticleSet();
	DamageTexUpdate();
}

//描画処理
void MobEnemy::Draw()
{
	Draw_Fbx();
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
		Damagetxt->DrawAll();
	
	Texture::PostDraw();

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

		XMFLOAT3 vel2{};
		vel2.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 3.0f;
		vel2.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 3.0f;
		vel2.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 3.0f;

		XMFLOAT3 acc2{};
		const float rnd_acc2 = 0.01f;
		acc2.y = -(float)rand() / RAND_MAX * rnd_acc2;
	//	//追加
		if (DamageParticleCreateF) {
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

void MobEnemy::DamageTexUpdate()
{
	std::ostringstream Damagestr;

	if (DamageDisplay) {
		DamageTexAlpha = 1.0f;
		DamagTexPos = { Position.x, Position.y + 5.0f, Position.z };
		DamageDisplay = false;
	}
	if (DamageTexAlpha >= 0.0f) {
		DamagTexPos.y-=0.2f;
		DamageTexAlpha -= 0.02f;
	}
	Damagestr << std::fixed << std::setprecision(2)
		<< GetDamage;
	Damagetxt->SetColor({ 1,1,1,DamageTexAlpha });
	Damagetxt->Print(Damagestr.str(),DamagTexPos.x, DamagTexPos.y, DamagTexPos.z, 2);

}