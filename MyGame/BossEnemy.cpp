#include "BossEnemy.h"

#include "AltAttack.h"
#include "CameraControl.h"
#include "CircleAttack.h"
#include"CustomButton.h"
#include"SphereCollider.h"
#include"CollisionManager.h"
#include"CollisionAttribute.h"
#include"DebugCamera.h"
#include "FrontCircleAttack.h"
#include "HalfAttack.h"
#include "ImageManager.h"
#include"mHelper.h"
#include"PlayerControl.h"
#include"imgui.h"
#include "KnockAttack.h"

/// <summary>
/// コンストラクタ
/// </summary>
using namespace DirectX;

BossEnemy::BossEnemy()
{
	ResourcesSet();
}

/// <summary>
/// デストラクタ
/// </summary>
BossEnemy::~BossEnemy()
{
}

void BossEnemy::ResourcesSet()
{
	DebugCamera* camera = CameraControl::GetInstance()->GetCamera();

	//モデルセット
	m_Object = std::make_unique<Object3d>();
	m_Object->Initialize(camera);

	m_fbxObject = std::make_unique<f_Object3d>();
	m_fbxObject->Initialize();
	m_fbxObject->SetModel(ModelManager::GetIns()->GetFBXModel(ModelManager::BOSS));
	m_fbxObject->LoadAnimation();
	m_fbxObject->PlayAnimation(1);

	Sprite* l_Bar = Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::BOSSHPFRAMEINNER), {0, 0});
	Sprite* BossHPFrame = Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::BOSSHPFRAME), {0, 0});
	Sprite* BossHPFrame_Inner = Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::BOSSHPFRAMEINNER2),
	                                           {0, 0});
	m_BossHP.reset(l_Bar);
	m_BossHP->SetAnchorPoint({ 0.f,0.0f });
	m_BossHPFrame.reset(BossHPFrame);
	m_BossHPFrame2.reset(BossHPFrame_Inner);
}

//初期化処理
void BossEnemy::Initialize()
{
	DebugCamera* camera = CameraControl::GetInstance()->GetCamera();

	//m_Object->CreateGraphicsPipeline(L"Resources/Shader/Object3dVS.hlsl", L"Resources/Shader/Object3dPS.hlsl", L"Resources/Shader/BasicGS.hlsl");
	MaxHP = 690;
	EnemyHP = MaxHP;

	Scale = {0.2f, 0.2f, 0.2f};
	Rotation = {180.0f, 0.0f, -181.0f};
	
	radius_adjustment = 0;
cooltime = 0;
	
	DeathFlag = false;

	//FollowRotAngleCorrect = -85.0f;
	addRotRadians = 180.f;
	SetCollider();
	state_boss->Initialize(this);

	m_BossHP->SetSize({0, 20});
	m_BossHPFrame->SetSize({600, 50});
	m_BossHP->SetAnchorPoint({0.0f, 0.0f});
	m_BossHPFrame->SetAnchorPoint({0.0f, 0.5f});

	BarPos = {381.0f, 862.0f};
	BarFramePos = {122.0f, 830.0f};
	FrameScl.x = Percent::GetParcent(static_cast<float>(MaxHP), static_cast<float>(EnemyHP)) *17.0f;

	//各攻撃処理の初期化
	HalfAttack::GetInstance()->Initialize();
	KnockAttack::GetInstance()->Initialize();
	CircleAttack::GetInstance()->Initialize();
	AltAttack::GetInstance()->Initialize();
	FrontCircleAttack::GetInstance()->Initialize();
	CircleAttack::GetInstance()->SetAttackPhase(true);

	CircleAttack::GetInstance()->SetAttackPhase(false);
	KnockAttack::GetInstance()->SetAttackPhase(false);
	HalfAttack::GetInstance()->SetAttackPhase(false);
	HalfAttack::GetInstance()->SetAttackPhase(false);
	AltAttack::GetInstance()->SetAttackPhase(false);
	FrontCircleAttack::GetInstance()->SetAttackPhase(false);

	for(int i=0;i<7;i++)
	{
		SetAttack_Start(i, false);
		SetAttack_End(i, false);
	}
}

//更新処理
void BossEnemy::Update()
{
	if (m_fbxObject == nullptr)
	{
		return;
	}
	DebugCamera* camera = CameraControl::GetInstance()->GetCamera();

	et += 0.01f;
	//行動遷移
	state_boss->Update(this);

	DamageTexDisplay();

	Action();

	AttackCollide();
	//fbxアニメーション制御
	//FbxAnimationControl();
	//座標やスケールの反映
	KnockAttack::GetInstance()->ActionJudg();

	m_fbxObject->SetColor({1.0f, 1.0f, 1.0f, alpha});
	m_fbxObject->SetPosition({ Position.x, Position.y+1.4f, Position.z });
	m_fbxObject->SetRotation(Rotation);
	m_fbxObject->SetScale(Scale);

	m_Object->SetPosition(Position);

	//m_fbxObject->SetColor({1,1,1,1});

	//m_fbxObject->PlayAnimation(m_Number);
	m_fbxObject->Update(m_AnimeLoop, m_AnimeSpeed, m_AnimationStop);
	//攻撃後のクールタイム設定
	AttackCoolTime();
	//地形当たり判定
	//CollisionField();
	//攻撃受けたらパーティクル
	DamageParticleSet();

	HPGaugeBoss();
	//m_fbxObject->SetFogPos({camera->GetEye()});
	m_fbxObject->SetHandBoneIndex(0);
}

void BossEnemy::AttackCollide()
{
	if (f_time >0.2f)
	{
		if (Collision::GetLength({ m_fbxObject->GetHandBoneMatWorld().r[3].m128_f32[0],
		m_fbxObject->GetHandBoneMatWorld().r[3].m128_f32[1],
		m_fbxObject->GetHandBoneMatWorld().r[3].m128_f32[2] },
			PlayerControl::GetInstance()->GetPlayer()->GetPosition()
		) < 10.f)
		{
			PlayerControl::GetInstance()->GetPlayer()->RecvDamage(5);
		}
	}
}

void BossEnemy::EnemyHPDraw()
{
	if (m_fbxObject == nullptr ||
		m_BossHPFrame == nullptr)
	{
		return;
	}
	Sprite::PreDraw();

	m_BossHPFrame2->Draw();

	m_BossHP->Draw();
	m_BossHPFrame->Draw();

	Sprite::PostDraw();
}

//描画処理
void BossEnemy::Draw()
{
	if (alpha < 0)
	{
		return;
	}
	Object3d::PreDraw();
	//Sword->Draw();
	Object3d::PostDraw();
	Draw_Fbx();
	float P = float(GetFbxTimeEnd());
	ImGui::Begin("bossp");
	ImGui::SliderFloat("Rotx", &Rotation.y,0,360);
	ImGui::SliderFloat("Roty", &addRotRadians, 0, 360);
	ImGui::SliderFloat("Rotz", &FollowRotAngleCorrect, 0, 360);
	ImGui::SliderFloat("time", &P, -30, 30);
	ImGui::End();

	// 3Dオブジェクト描画前処理
	// 3Dオブジェクト描画前処理
}

void BossEnemy::Death()
{
	if (!DieFlag)
	{
		DieFlag = true;
		//ExpPointSystem::GetInstance()->ExpPoint_Get(10);
	}
	else
	{
		if (f_time < DeathMotionTime_Start)
		{
			f_time = DeathMotionTime_Start;
		}
	}
	if (f_time > m_fbxObject->GetEndTime())
	{
		alpha -= 0.01f;
	}
	CameraControl::GetInstance()->SetCameraState(CameraControl::RUSHSCENE);

	f_time += 0.009f;


	movestop = false;
}

void BossEnemy::Move()
{
}

void BossEnemy::Smoke(bool& createf)
{
}

#include"PlayerAttackState.h"

void BossEnemy::AttackCoolTime()
{
	
	if (AfterAttack)
	{
		cooltime++;
		if (cooltime > 250)
		{
			AfterAttack = false;
		}
	}
	else
	{
		if (m_Number == BNORMAL)
		{
			AfterAttack = true;
		}
		cooltime = 0;
	}
}

void BossEnemy::FbxAnimationControl()
{
}

void BossEnemy::DamageParticleSet()
{
}

void BossEnemy::HPFrameInit()
{
}

void BossEnemy::HPGaugeBoss()
{
	if (RecvDamagef)
	{
		if (FrameScalingETime >= 1.0f)
		{
			InnerFrameScalingF = true;
			RecvDamagef = false;
		}
		FrameScalingETime_Inner = 0.0f;
		if (!InnerFrameScalingF)
		{
			OldFrameX_Inner = OldFrameX;
		}
		NowFrameX = Percent::GetParcent(static_cast<float>(MaxHP), static_cast<float>(EnemyHP)) *17.0f;
		FrameScalingETime += 0.05f;
		if (FrameScl.x > 0.0f)
		{
			FrameScl.x = Easing::EaseOut(FrameScalingETime, OldFrameX, NowFrameX);
		}
	}

	else
	{
		OldFrameX = Percent::GetParcent(static_cast<float>(MaxHP), static_cast<float>(EnemyHP)) *17.0f;

		FrameScalingETime = 0.0f;
	}
	if (EnemyHP <= 0)
	{
		if (FrameScl_Inner.x > 0.f)
		{
			FrameScalingETime_Inner += 0.02f;
		}
		FrameScl_Inner.x = Easing::EaseOut(FrameScalingETime_Inner, OldFrameX_Inner, 0.f);
		InnerFrameScalingF = false;
	}
	if (InnerFrameScalingF)
	{
		FrameScalingETime_Inner += 0.02f;
		//体力が０なったときだけEaseの終わりを０に
		if (EnemyHP <= 0)
		{
			FrameScl_Inner.x = Easing::EaseOut(FrameScalingETime_Inner, OldFrameX_Inner, 0.f);
			InnerFrameScalingF = false;
		}
		else
		{
			FrameScl_Inner.x = Easing::EaseOut(FrameScalingETime_Inner, OldFrameX_Inner, NowFrameX);
		}
		//ゲージの減りが止まったらフラグ切る
		if (FrameScalingETime_Inner >= 1.0f)
		{
			InnerFrameScalingF = false;
		}
	}
	m_BossHP->SetSize({FrameScl.x, 110.f});
	m_BossHPFrame2->SetSize({FrameScl_Inner.x, 110.0f});
	m_BossHPFrame->SetSize({1800.0f, 117.0f});

	m_BossHP->SetPosition({145.0f, 832.0f});
	m_BossHPFrame2->SetPosition({135.0f, 832.0f});
	m_BossHPFrame->SetPosition({122.0f, 830.0f});
}


