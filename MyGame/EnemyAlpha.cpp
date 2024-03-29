#include "EnemyAlpha.h"

#include "CameraControl.h"
#include "ChestControl.h"
#include"SphereCollider.h"
#include"DebugCamera.h"
#include"mHelper.h"
#include"Collision.h"
#include "Field.h"
#include "ImageManager.h"
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


void EnemyAlpha::HPFrameInit()
{
	//体力バー初期化
	Sprite* l_frame1 = Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::ENMEYHPFRAME1), {0, 0});
	Sprite* l_frame2 = Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::ENMEYHPFRAME2), {0, 0});
	Sprite* l_frame3 = Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::ENMEYHPFRAME4), {0, 0});
	Sprite* l_frame4 = Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::ENMEYHPFRAME3), {0, 0});

	Sprite* l_enemyname = Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::ENEMYNAME_LIZARD), {0, 0});

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
void EnemyAlpha::Initialize()
{
	DebugCamera* camera = CameraControl::GetIns()->GetCamera();

	//オブジェクトの生成と初期化
	m_Object = std::make_unique<Object3d>();
	m_Object->Initialize(camera);

	m_fbxObject = std::make_unique<f_Object3d>();
	m_fbxObject->Initialize();
	m_fbxObject->SetModel(FbxLoader::GetIns()->LoadModelFromFile("sniper_blender"));
	m_fbxObject->PlayAnimation();

	//パラメータのセット

	MaxHP = 100;
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

	HPFrameInit();
	//タスク用敵の種類
	ENumber = FLOG;
	//状態初期化
	state_mob->Initialize(this);
}

//更新処理
void EnemyAlpha::Update()
{
	DebugCamera* camera = CameraControl::GetIns()->GetCamera();

	state_mob->Update(this);

	if (DeathFlag && alpha > -1)
	{
		alpha -= 0.005f;
	}
	m_fbxObject->SetColor({1, 0, 0, alpha});

	if (alpha > 0.f && SceneManager::GetIns()->GetScene() != SceneManager::MAPCREATE)
	{
		m_fbxObject->SetFogPos(PlayerControl::GetIns()->GetPlayer()->GetPosition());


		//if (f_time >= NormalAttackTime + 1.0f) {
		if (Collision::GetLength(Position, PlayerControl::GetIns()->GetPlayer()->GetPosition()) < 10.f)
		{
			PlayerControl::GetIns()->GetPlayer()->RecvDamage(10);
		}
		//}
	}
	if (Field::GetIns()->GetPedestalDown() || ChestControl::GetIns()->GetChestEventNothing())
	{
		ReturnCreatePos = true;
	}
	FbxAnimationControl();
	AttackCoolTime();
	ParameterSet_Fbx2();
	DamageTexDisplay();
	CollisionField();
	HPFrameUpda();
	DamageParticleSet();
}

//描画処理
void EnemyAlpha::Draw()
{
	if (alpha > 0)
	{
		Draw_Fbx();
	}
}

void EnemyAlpha::Death()
{
	if (!DeathFlag)
	{
		//f_time = DeathTime;
		//if (f_time > DeathTime) {
		DeathFlag = true;
		//EnemyHP = MaxHP;
	}
	alpha -= 0.02f;
	//if (f_time < DeathTime) {
	//f_time = DeathTime;
	//	}
	movestop = false;
}

void EnemyAlpha::EnemyHPDraw()
{
	if (alpha <= 0.f)
	{
		return;
	}
	Player* l_player = PlayerControl::GetIns()->GetPlayer();

	Sprite::PreDraw();
	if (Collision::GetLength(Position, l_player->GetPosition()) < 40)
	{
		for (int i = 0; i < 4; i++)
		{
			HPFrame[i]->Draw();
		}
		EnemyName->Draw();
		Sprite::PostDraw();
	}
}


void EnemyAlpha::Move()
{
}

void EnemyAlpha::Smoke(bool& createf)
{
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
