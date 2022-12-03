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
#include"PlayerAttackState.h"
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
	//Destroy(particleMan);
	//delete mob, MobModel;
}

//初期化処理
void MobEnemy::Initialize(DebugCamera* camera)
{
	//オブジェクトの生成と初期化
	Sword = std::make_unique<Object3d>();
	Sword->Initialize(camera);
	Sword->SetModel(ModelManager::GetIns()->GetModel(ModelManager::BIGSWORD));
	Sword->SetRotation({0, 0 + 30, 0 + 100});

	m_Object = std::make_unique<Object3d>();
	m_Object->Initialize(camera);
	//m_Object->CreateGraphicsPipeline(L"Resources/Shader/Object3dVS.hlsl", L"Resources/Shader/Object3dPS.hlsl", L"Resources/Shader/BasicGS.hlsl");
	
	m_fbxObject = std::make_unique<f_Object3d>();
	m_fbxObject->Initialize();
	m_fbxObject->SetModel(FbxLoader::GetInstance()->LoadModelFromFile("monster_golem_demo"));
	m_fbxObject->PlayAnimation();
	
	MaxHP = 120.0f;

	EnemyHP = MaxHP;
	//パラメータのセット
	Rotation = { 114.0f, 118.0f, 165.0f };
	Scale = { 0.05f, 0.05f, 0.05f };

	//コライダー周り
	radius_adjustment = 0;
	SetCollider();

	//FBX切り替わりのタイム指定
	NormalAttackTime = 0.9f;
	DeathTime = 6.9f;

	nowAttack = false;
	nowDeath = false;

	//state初期化
	state_mob->Initialize(this);

	addRotRadians = 0;
	FollowRotAngleCorrect = 180;

	ENumber = EnemyNumber::GOLEM;
}

#include"SceneManager.h"
//更新処理
void MobEnemy::Update(DebugCamera* camera)
{
	state_mob->Update(this);

	if (SceneManager::GetInstance()->GetScene() != SceneManager::MAPCREATE)
	{
		m_fbxObject->SetFogPos(PlayerControl::GetInstance()->GetPlayer()->GetPosition());
	}
	if (SceneManager::GetInstance()->GetScene() != SceneManager::BOSS)
	{
		CollisionField(camera);
	
	}
	FbxAnimationControl();

	//EnemyPop(150);

	AttackCoolTime();

	DamageTexDisplay();
	ParameterSet_Fbx(camera);

	
	m_fbxObject->SetColor({1.0f, 1.0f, 1.0f, alpha});
	m_fbxObject->SetHandBoneIndex(25);

	Sword->Setf(FALSE);
	Sword->SetRotation({-23, 43, 83});
	Sword->Update(m_fbxObject->GetHandBoneMatWorld(), {1.0f, 1.0f, 1.0f, 1.0f}, camera);

	HandMat = m_fbxObject->GetHandBoneMatWorld();

	OBBSetParam();
	m_fbxObject->SetFbxTime(f_time);
}
void MobEnemy::OBBSetParam()
{
	Player* l_player = PlayerControl::GetInstance()->GetPlayer();
	if (SceneManager::GetInstance()->GetScene() == SceneManager::MAPCREATE)
	{
		return;
	}
	if(l_player==nullptr)
	{
		return;
	}
	if (DeathFlag|| Collision::GetLength(Position, l_player->GetPosition()) > 40)return;

		HandSiteOBB.SetOBBParam_Pos(Sword->GetMatWorld());
		HandSiteOBB.SetOBBParam_Rot(Sword->GetMatWorld());
		HandSiteOBB.SetOBBParam_Scl({ 8.0f, 30.0f, 8.0f });

		playerOBB.SetOBBParam_Pos(l_player->GetPosition());
		playerOBB.SetOBBParam_Rot(l_player->GetMatrot());
		playerOBB.SetOBBParam_Scl({ 1.0f, 9.0f, 1.0f });

	//横薙ぎ攻撃
		if (atcktype == SIDEAWAY)
		{
			//アニメーションが一定フレーム越したら
			if (f_time >= NormalAttackTime + 1.0f)
			{
				if (Collision::CheckOBBCollision(playerOBB, HandSiteOBB) == true)
				{
					l_player->RecvDamage(10);
				}
			}
		}
	//縦振り攻撃
		if (atcktype == VERTICAL)
		{
			if (f_time >= 4.8f&& f_time <= 5.2f)
			{
				if (Collision::CheckOBBCollision(playerOBB, HandSiteOBB) == true)
				{
					l_player->RecvDamage(15);
				}
			}
		}
}


//描画処理
void MobEnemy::Draw()
{
	if (alpha >= 0.0f)
	{
		Draw_Fbx();

		// 3Dオブジェクト描画前処理
		Object3d::PreDraw();
		Sword->Draw();
		Object3d::PostDraw();
	}
}

#include"ExpPointSystem.h"

void MobEnemy::Death()
{
	if (!DeathFlag)
	{
		DeathFlag = true;
		//アニメーションを死亡時に合わせる
		if (f_time < DeathTime)
		{
			f_time = DeathTime;
		}
	}
	//最終フレーム超えたら透明に
	if(f_time>=m_fbxObject->GetEndTime())
	{
		alpha -= 0.02f;
	}

	f_time += 0.01f;

//	PlayerAttackState::GetInstance()->SetHitStopJudg(TRUE);
	
	movestop = false;
}

void MobEnemy::FbxAnimationControl()
{
	if (DeathFlag)
	{
		return;
	}
	//アニメーションスピード
	float fbxanimationTime;

	if (nowAttack)
	{//攻撃中
		fbxanimationTime = 0.02f;
	}
	else
	{//歩きとか死亡時
		fbxanimationTime = 0.01f;
	}

	//ヒットストップ時
	if (PlayerAttackState::GetInstance()->GetHitStopJudg())
	{
		fbxanimationTime = 0.000f;
	}
	//アニメーションカウント進める
	f_time += fbxanimationTime;

	//攻撃方法ランダムで決定
	if (f_AttackFlag)
	{
		//50以下では横薙ぎ
		rand_Attacktype = static_cast<float>(rand() % 100);
		if (rand_Attacktype <= 50)
		{
			atcktype = SIDEAWAY;
			f_time = NormalAttackTime;
		}
		else
		{//以上は縦振り
			atcktype = VERTICAL;
			f_time = 3.7f;
		}

		nowAttack = true;
		f_AttackFlag = false;
	}
	else
	{
		if (!nowAttack && f_time >= NormalAttackTime)
		{
			f_time = 0.0f;
		}
	}

	if (atcktype == SIDEAWAY)
	{
		if (f_time > 3.7f)
		{
			nowAttack = false;
		}
	}
	else if (atcktype == VERTICAL)
	{
		if (f_time > DeathTime)
		{
			nowAttack = false;
		}
	}
}


void MobEnemy::AttackCoolTime()
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
