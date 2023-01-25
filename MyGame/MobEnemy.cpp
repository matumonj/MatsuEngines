#include "MobEnemy.h"
#include"SphereCollider.h"
#include"CollisionManager.h"
#include"DebugCamera.h"
#include"mHelper.h"
#include"imgui.h"
#include"Collision.h"
#include"PlayerControl.h"
#include "CameraControl.h"
#include"PlayerAttackState.h"
#include"ImageManager.h"
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
	Destroy_unique(Sword);
	//Destroy_unique(m_fbxObject);

	for (int i = 0; i < 3; i++)
	{
		HPFrame[i].reset();
	}
}

void MobEnemy::HPFrameInit()
{
	//体力バー初期化
	Sprite* l_frame1 = Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::ENMEYHPFRAME1), {0, 0});
	Sprite* l_frame2 = Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::ENMEYHPFRAME2), {0, 0});
	Sprite* l_frame3 = Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::ENMEYHPFRAME4), {0, 0});
	Sprite* l_frame4 = Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::ENMEYHPFRAME3), {0, 0});

	Sprite* l_enemyname = Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::ENEMYNAME_GOLEM), {0, 0});

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

	//モデル初期化
	m_Object = std::make_unique<Object3d>();
	m_Object->Initialize(camera);

	m_fbxObject = std::make_unique<f_Object3d>();
	m_fbxObject->Initialize();
	m_fbxObject->SetModel(FbxLoader::GetInstance()->LoadModelFromFile("monster_golem_demo"));
	m_fbxObject->PlayAnimation();

	//体力設定
	MaxHP = 100;
	EnemyHP = MaxHP;
	//パラメータのセット
	Rotation = {114.0f, 118.0f, 165.0f};
	Scale = {0.05f, 0.05f, 0.05f};

	//コライダー周り
	radius_adjustment = 0;
	SetCollider();

	//FBX切り替わりのタイム指定
	NormalAttackTime = 0.9f;
	DeathTime = 6.9f;

	//攻撃中と死亡モーション中のフラグ
	nowAttack = false;
	nowDeath = false;
	DeathFlag = false;
	//state初期化
	state_mob->Initialize(this);

	//追跡の回転角の調整用
	addRotRadians = 0;
	FollowRotAngleCorrect = 180;

	HPFrameInit();

	//適番号：ゴーレム
	ENumber = GOLEM;

	attackinter = rand() % 120 + 90;
}

//更新処理
void MobEnemy::Update()
{
	//カメラのインスタンス持ってくる
	DebugCamera* camera = CameraControl::GetInstance()->GetCamera();

	//state更新
	state_mob->Update(this);

	if (SceneManager::GetInstance()->GetScene() != SceneManager::MAPCREATE)
	{
		m_fbxObject->SetFogPos(PlayerControl::GetInstance()->GetPlayer()->GetPosition());
	}

	if (SceneManager::GetInstance()->GetScene() != SceneManager::BOSS)
	{
		CollisionField();
	}
	//FBXアニメーション更新
	FbxAnimationControl();
	//HPバー更新
	HPFrameUpda();
	//EnemyPop(150);

	AttackCoolTime();

	DamageTexDisplay();
	ParameterSet_Fbx();


	//敵の色
	m_fbxObject->SetColor({1.0f, 1.0f, 1.0f, alpha});
	//敵の手のボーンインデックス
	m_fbxObject->SetFbxTime(f_time);
	HandIndex = 23;
	m_fbxObject->SetHandBoneIndex(HandIndex);
	//持ってる斧の更新
	Sword->Setf(FALSE);
	Sword->SetRotation({-23, 43, 83});
	Sword->Update(m_fbxObject->GetHandBoneMatWorld(), {1.0f, 1.0f, 1.0f, 1.0f}, camera);

	HandMat = m_fbxObject->GetHandBoneMatWorld();

	//斧とプレイヤーのあたり判定
	OBBSetParam();
}


void MobEnemy::OBBSetParam()
{
	//プレイヤーのインスタンス引き出す
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

	//手のワールド行列から各成分抜き出し
	HandSiteOBB.SetOBBParam_Pos(Sword->ExtractPositionMat());
	HandSiteOBB.SetOBBParam_Rot(Sword->ExtractRotationMat());
	HandSiteOBB.SetOBBParam_Scl({10.0f, 10.0f, 8.0f});

	//プレイヤー
	playerOBB.SetOBBParam_Pos(l_player->GetPosition());
	playerOBB.SetOBBParam_Rot(l_player->GetMatrot());
	playerOBB.SetOBBParam_Scl({3.0f, 9.0f, 3.0f});

	//横薙ぎ攻撃
	if (atcktype == SIDEAWAY)
	{
		//アニメーションが一定フレーム越したら
		if (f_time >= NormalAttackTime + 1.f)
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
		if (f_time >= 4.8f && f_time <= 5.2f)
		{
			if (Collision::CheckOBBCollision(playerOBB, HandSiteOBB) == true)
			{
				l_player->RecvDamage(10);
			}
		}
	}
}


//描画処理
void MobEnemy::Draw()
{
	//アルファ値が０なったら病が切る
	if (alpha >= 0.0f)
	{
		Draw_Fbx();

		// 3Dオブジェクト描画前処理
		Object3d::PreDraw();
		Sword->Draw();
		Object3d::PostDraw();
	}
	ImGui::Begin("hindex");
	ImGui::SliderInt("ind", &HandIndex, 0, 40);
	ImGui::End();
}

void MobEnemy::EnemyHPDraw()
{
	if (alpha <= 0.f)return;
	Player* l_player = PlayerControl::GetInstance()->GetPlayer();

	if (Collision::GetLength(Position, l_player->GetPosition()) < 40)
	{
		Sprite::PreDraw();
		for (int i = 0; i < 4; i++)
		{
			HPFrame[i]->Draw();
		}
		EnemyName->Draw();
		Sprite::PostDraw();
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

void MobEnemy::Move()
{
}

void MobEnemy::Smoke(bool& createf)
{
}

void MobEnemy::FbxAnimationControl()
{
	if (DeathFlag)
	{
		return;
	}
	//アニメーションスピード
	float fbxanimationTime;


	//ヒットストップ時
	if (PlayerAttackState::GetInstance()->GetHitStopJudg())
	{
		fbxanimationTime = 0.002f;
	}
	else
	{
		if (nowAttack)
		{
			//攻撃中
			fbxanimationTime = 0.015f;
		}
		else
		{
			//歩きとか死亡時
			fbxanimationTime = 0.01f;
		}
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
		if (cooltime > attackinter)
		{
			AfterAttack = false;
		}
	}
	else
	{
		cooltime = 0;
	}
}
