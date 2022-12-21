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

#include "CameraControl.h"
#include"PlayerAttackState.h"
#include"ImageManager.h"
#include"mHelper.h"
#include "SceneManager.h"
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
	for (int i = 0; i < 3; i++)
	{
		HPFrame[i].reset();
	}
}

//初期化処理
void MobEnemy::Initialize()
{
	DebugCamera* camera = CameraControl::GetInstance()->GetCamera();

	//オブジェクトの生成と初期化
	Sword = std::make_unique<Object3d>();
	Sword->Initialize(camera);
	Sword->SetModel(ModelManager::GetIns()->GetModel(ModelManager::BIGSWORD));
	Sword->SetRotation({0, 0 + 30, 0 + 100});

	m_Object = std::make_unique<Object3d>();
	m_Object->Initialize(camera);

	m_fbxObject = std::make_unique<f_Object3d>();
	m_fbxObject->Initialize();
	m_fbxObject->SetModel(FbxLoader::GetInstance()->LoadModelFromFile("monster_golem_demo"));
	m_fbxObject->PlayAnimation();

	MaxHP = 100;

	EnemyHP = MaxHP;
	//パラメータのセット
	Rotation = {114.0f, 118.0f, 165.0f};
	Scale = {0.03f, 0.03f, 0.03f};

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

	Sprite* l_frame1 = Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::ENMEYHPFRAME1), {0, 0});
	Sprite* l_frame2 = Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::ENMEYHPFRAME2), {0, 0});
	Sprite* l_frame3 = Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::ENMEYHPFRAME4), {0, 0});
	Sprite* l_frame4 = Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::ENMEYHPFRAME3), {0, 0});

	Sprite* l_enemyname = Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::ENEMYNAME_GOLEM), { 0, 0 });

	HPFrame[0].reset(l_frame1);
	HPFrame[1].reset(l_frame2);
	HPFrame[2].reset(l_frame3);
	HPFrame[3].reset(l_frame4);

	EnemyName.reset(l_enemyname);

	for (int i = 0; i < 4; i++)
	{
		HPFrame[i]->SetAnchorPoint({0.0f, 0.0f});
	}
	FrameScl.x = Percent::GetParcent(static_cast<float>(MaxHP), static_cast<float>(EnemyHP)) * 2.0f;
	ENumber = GOLEM;
}

//更新処理
void MobEnemy::Update()
{
	DebugCamera* camera = CameraControl::GetInstance()->GetCamera();

	state_mob->Update(this);

	if (SceneManager::GetInstance()->GetScene() != SceneManager::MAPCREATE)
	{
		m_fbxObject->SetFogPos(PlayerControl::GetInstance()->GetPlayer()->GetPosition());
	}
	if (SceneManager::GetInstance()->GetScene() != SceneManager::BOSS)
	{
		CollisionField();
	}
	FbxAnimationControl();

	HPFrameScaling();
	//EnemyPop(150);

	AttackCoolTime();

	DamageTexDisplay();
	ParameterSet_Fbx();


	m_fbxObject->SetColor({1.0f, 1.0f, 1.0f, alpha});
	m_fbxObject->SetHandBoneIndex(25);

	Sword->Setf(FALSE);
	Sword->SetRotation({-23, 43, 83});
	Sword->Update(m_fbxObject->GetHandBoneMatWorld(), {1.0f, 1.0f, 1.0f, 1.0f}, camera);

	HandMat = m_fbxObject->GetHandBoneMatWorld();

	OBBSetParam();
	m_fbxObject->SetFbxTime(f_time);
}

void MobEnemy::HPFrameScaling()
{
	DebugCamera* camera = CameraControl::GetInstance()->GetCamera();

	XMVECTOR tex2DPos[4];
	for (int i = 0; i < 4; i++)
	{
		tex2DPos[i] = {Position.x, Position.y + 13.0f, Position.z};
		tex2DPos[i] = MatCal::PosDivi(tex2DPos[i], camera->GetViewMatrix(), false);
		tex2DPos[i] = MatCal::PosDivi(tex2DPos[i], camera->GetProjectionMatrix(), true);
		tex2DPos[i] = MatCal::WDivi(tex2DPos[i], false);
		tex2DPos[i] = MatCal::PosDivi(tex2DPos[i], camera->GetViewPort(), false);

		HPFrame[i]->SetPosition({tex2DPos[i].m128_f32[0] - 80.0f, tex2DPos[i].m128_f32[1]});
	}
	if (RecvDamagef)
	{
		FrameScalingETime_Inner = 0.0f;
		if (!InnerFrameScalingF)
		{
			OldFrameX_Inner = OldFrameX;
		}
		NowFrameX = Percent::GetParcent(static_cast<float>(MaxHP), static_cast<float>(EnemyHP)) * 2.0f;
		FrameScalingETime += 0.05f;
		FrameScl.x = Easing::EaseOut(FrameScalingETime, OldFrameX, NowFrameX);

		if (FrameScalingETime >= 1.0f)
		{
			InnerFrameScalingF = true;
			RecvDamagef = false;
		}
	}

	else
	{
		OldFrameX = Percent::GetParcent(float(MaxHP), float(EnemyHP)) * 2.0f;

		FrameScalingETime = 0.0f;
	}

	if (InnerFrameScalingF)
	{
		FrameScalingETime_Inner += 0.02f;
		FrameScl_Inner.x = Easing::EaseOut(FrameScalingETime_Inner, OldFrameX_Inner, NowFrameX);
		if (FrameScalingETime_Inner >= 1.0f)
		{
			InnerFrameScalingF = false;
		}
	}
	else
	{
		FrameScalingETime_Inner = 0.0f;
	}
	HPFrame[3]->SetSize({FrameScl.x, 15});
	HPFrame[2]->SetSize({FrameScl_Inner.x, 15.0f});
	HPFrame[1]->SetSize({200.0f, 15.0f});
	HPFrame[0]->SetSize({200.0f, 15.0f});

	EnemyName->SetPosition({ tex2DPos[0].m128_f32[0]-80.0f, tex2DPos[0].m128_f32[1]-30.f });
	EnemyName->SetSize({ 200.0f,15.0f });
	FrameScl.x = max(FrameScl.x, 0.0f);
	FrameScl_Inner.x = max(FrameScl_Inner.x, 0.0f);
}


void MobEnemy::OBBSetParam()
{
	Player* l_player = PlayerControl::GetInstance()->GetPlayer();
	if (SceneManager::GetInstance()->GetScene() == SceneManager::MAPCREATE)
	{
		return;
	}
	if (l_player == nullptr)
	{
		return;
	}
	if (DeathFlag || Collision::GetLength(Position, l_player->GetPosition()) > 40)
	{
		return;
	}

	HandSiteOBB.SetOBBParam_Pos(Sword->GetMatWorld());
	HandSiteOBB.SetOBBParam_Rot(Sword->GetMatWorld());
	HandSiteOBB.SetOBBParam_Scl({8.0f, 10.0f, 8.0f});

	playerOBB.SetOBBParam_Pos(l_player->GetPosition());
	playerOBB.SetOBBParam_Rot(l_player->GetMatrot());
	playerOBB.SetOBBParam_Scl({1.0f, 9.0f, 1.0f});

	//横薙ぎ攻撃
	if (atcktype == SIDEAWAY)
	{
		//アニメーションが一定フレーム越したら
		if (f_time >= NormalAttackTime + 1.0f)
		{
			if (Collision::CheckOBBCollision(playerOBB, HandSiteOBB) == true)
			{
				l_player->RecvDamage(5);
			}
		}
	}
	//縦振り攻撃
	if (atcktype == VERTICAL)
	{
		if (f_time >= 4.8f && f_time <= 5.2f)
		{
			if (Collision::CheckOBBCollision(playerOBB, HandSiteOBB) == true)
			{
				l_player->RecvDamage(5);
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

void MobEnemy::EnemyHPDraw()
{
	Player* l_player = PlayerControl::GetInstance()->GetPlayer();

	if (Collision::GetLength(Position, l_player->GetPosition()) > 40) { return; }
	Sprite::PreDraw();
	for (int i = 0; i < 4; i++)
	{
		HPFrame[i]->Draw();
	}
	EnemyName->Draw();
	Sprite::PostDraw();
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
	if (f_time >= m_fbxObject->GetEndTime())
	{
		alpha -= 0.02f;
	}
	if (f_time <= DeathTime + 1.5f)
	{
		PlayerAttackState::GetInstance()->SetHitStopJudg(true);
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
	{
		//攻撃中
		fbxanimationTime = 0.02f;
	}
	else
	{
		//歩きとか死亡時
		fbxanimationTime = 0.01f;
	}

	//ヒットストップ時
	if (PlayerAttackState::GetInstance()->GetHitStopJudg())
	{
		fbxanimationTime = 0.002f;
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
		{
			//以上は縦振り
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
