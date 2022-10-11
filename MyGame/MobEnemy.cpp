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
	
	EnemyHP = 30.00f;
	MaxHP = 30.00f;
	//パラメータのセット
	Rotation = { -70,180,0 };
	Scale = { 0.01f, 0.01f, 0.01f };

	m_fbxObject = std::make_unique<f_Object3d>();
	m_fbxObject->Initialize();
	m_fbxObject->SetModel(FbxLoader::GetInstance()->LoadModelFromFile("monster_golem"));
	m_fbxObject->PlayAnimation();
	m_fbxObject->SetColor({ 1,0,0,alpha });
	//コライダー周り
	radius_adjustment = 0;
	SetCollider();

	//FBX切り替わりのタイム指定
	AttackTime = 1.5f;
	DeathTime = 4.9f;

	nowAttack = false;
	nowDeath = false;

	//state初期化
	state_mob->Initialize(this);

	//
	Texture::LoadTexture(93, L"Resources/ParticleTex/slash.png");
	SlashTex = Texture::Create(93, { 0.0f ,0.0f ,0.0f }, { 100.0f ,100.0f ,1.0f }, { 1.0f ,1.0f ,1.0f ,1.0f });
	SlashTex->CreateTexture();
	SlashTex->SetAnchorPoint({ 0.5f,0.5f });
	SlashPos = { Position.x,Position.y,Position.z };

	//パーティクルセット
	ParticleManager::LoadTexture(4, L"Resources/ParticleTex/Normal.png");
	particleMan=ParticleManager::Create(4,L"Resources/ParticleTex/Attack.png");
	ParticleManager::LoadTexture(6, L"Resources/ParticleTex/Attack.png");
	particleMan2 = ParticleManager::Create(6, L"Resources/ParticleTex/Attack.png");

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


	DamageParticleSet();
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


}

void MobEnemy::DamageTexUpdate(DebugCamera* camera)
{
	SlashPos.z = Position.z;
	if (DamageParticleCreateF && !SlashF2) {
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
	if (SlashF2) {
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
	SlashTex->SetRotation({ 0,180,0 });
	SlashTex->SetScale({ 2.0f ,2.0f ,3.0f });

}