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
	//オブジェクトの生成と初期化
	m_Object = std::make_unique<Object3d>();
	m_Object->Initialize(camera);

	m_fbxObject = std::make_unique<f_Object3d>();
	m_fbxObject->Initialize();
	m_fbxObject->SetModel(FbxLoader::GetInstance()->LoadModelFromFile("sniper_blender"));
	m_fbxObject->PlayAnimation();

	//パラメータのセット
	
	MaxHP = 100.00f;
	EnemyHP = MaxHP;
	Rotation = {-110, 0, 0};
	Scale = {0.05f, 0.05f, 0.05f};
	radius_adjustment = 0;
	
	SetCollider();
	//fbxアニメーション時間の設定
	NormalAttackTime = 1.5f;
	DeathTime = 6.f;

	nowAttack = false;
	nowDeath = false;
	m_fbxObject->SetColor({1, 0, 0, alpha});
	
	addRotRadians = -180;
	FollowRotAngleCorrect = 0;
	//タスク用敵の種類
	ENumber = EnemyNumber::FLOG;
	//状態初期化
	state_mob->Initialize(this);
}

//更新処理
void EnemyAlpha::Update(DebugCamera* camera)
{
	state_mob->Update(this);

	if (DeathFlag && alpha > -1)
	{
		alpha -= 0.005f;
	}
	m_fbxObject->SetColor({1, 0, 0, alpha});

	if (!DeathFlag && SceneManager::GetInstance()->GetScene() != SceneManager::MAPCREATE)
	{
		m_fbxObject->SetFogPos(PlayerControl::GetInstance()->GetPlayer()->GetPosition());

		HandSiteOBB.SetOBBParam_Pos(m_fbxObject->GetWorld());
		HandSiteOBB.SetOBBParam_Rot(m_fbxObject->GetWorld());
		HandSiteOBB.SetOBBParam_Scl({25.0f, 200.0f, 250.0f});

		playerOBB.SetOBBParam_Pos(PlayerControl::GetInstance()->GetPlayer()->GetPosition());
		playerOBB.SetOBBParam_Rot(PlayerControl::GetInstance()->GetPlayer()->GetMatrot());
		playerOBB.SetOBBParam_Scl({1.0f, 5.0f, 1.0f});

		//if (f_time >= NormalAttackTime + 1.0f) {
		if (Collision::CheckOBBCollision(playerOBB, HandSiteOBB) == true)
		{
			PlayerControl::GetInstance()->GetPlayer()->RecvDamage(10);
		}
		//}
	}
	FbxAnimationControl();
	AttackCoolTime();
	ParameterSet_Fbx2(camera);
	DamageTexDisplay();
	CollisionField(camera);

	DamageParticleSet();
}

//描画処理
void EnemyAlpha::Draw()
{
	if (alpha > 0)
	{
		Draw_Fbx();
	}
	ParticleManager::PreDraw();
	// 3Dオブジェクト描画後処理
	ParticleManager::PostDraw();
}

void EnemyAlpha::Death()
{
	if (!DeathFlag)
	{
		ExpPointSystem::GetInstance()->ExpPoint_Get(10);
		//f_time = DeathTime;
		//if (f_time > DeathTime) {
		DeathFlag = true;
		//EnemyHP = MaxHP;
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

	if (f_AttackFlag)
	{
		f_time = NormalAttackTime;
		f_AttackFlag = false;
		nowAttack = true;
	}
	else
	{
		if (DeathFlag == false)
		{
			if (!nowAttack && f_time >= NormalAttackTime)
			{
				f_time = 0.0f;
			}
		}
	}


	if (f_time > DeathTime)
	{
		nowAttack = false;
	}

	m_fbxObject->SetFbxTime(f_time);
}


void EnemyAlpha::AttackCoolTime()
{
	if (f_time >= DeathTime - 1)
	{
		AfterAttack = true;
	}
	if (AfterAttack)
	{
		cooltime++;
		if (cooltime > 480)
		{
			AfterAttack = false;
		}
	}
	else
	{
		cooltime = 0;
	}
}

void EnemyAlpha::DamageParticleSet()
{
}
