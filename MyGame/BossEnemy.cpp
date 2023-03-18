#include "BossEnemy.h"
#include <algorithm>
#include "RushAttack.h"
#include "CameraControl.h"
#include "CircleAttack.h"
#include"SphereCollider.h"
#include"DebugCamera.h"
#include "FrontCircleAttack.h"
#include "HalfAttack.h"
#include "ImageManager.h"
#include"mHelper.h"
#include"PlayerControl.h"
#include"imgui.h"
#include "KnockAttack.h"
#include"UltAttack.h"
#include"BronzeAttack.h"
#include "LineCrossAttack.h"
#include"ThrowRockAttack.h"
/// <summary>
/// コンストラクタ
/// </summary>
using namespace DirectX;

BossEnemy::BossEnemy()
{
	//オブジェのセット
	ResourcesSet();
	//ボス攻撃のインスタンス格納
	BossAttackAction.emplace_back(CircleAttack::GetIns());
	BossAttackAction.emplace_back(RushAttack::GetIns());
	BossAttackAction.emplace_back(FrontCircleAttack::GetIns());
	BossAttackAction.emplace_back(UltAttack::GetIns());
	BossAttackAction.emplace_back(BronzeAttack::GetIns());
	BossAttackAction.emplace_back(ThrowRockAttack::GetIns());
	BossAttackAction.emplace_back(LineCrossAttack::GetIns());
	//BossAttackAction.emplace_back(HalfAttack::GetIns());
}

/// <summary>
/// デストラクタ
/// </summary>
BossEnemy::~BossEnemy()
{
	Destroy_unique(m_Object);
	Destroy_unique(m_fbxObject);
}


void BossEnemy::ResourcesSet()
{
	DebugCamera* camera = CameraControl::GetIns()->GetCamera();

	//モデルセット
	m_Object = std::make_unique<Object3d>();
	m_Object->Initialize(camera);

	m_fbxObject = std::make_unique<f_Object3d>();
	m_fbxObject->Initialize();
	m_fbxObject->SetModel(FbxLoader::GetIns()->LoadModelFromFile("boss"));
	m_fbxObject->LoadAnimation();
	m_fbxObject->PlayAnimation(1);

	Sprite* l_Bar = Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::BOSSHPFRAMEINNER), {0, 0});
	Sprite* BossHPFrame = Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::BOSSHPFRAME), {0, 0});
	Sprite* BossHPFrame_Inner = Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::BOSSHPFRAMEINNER2),
	                                           {0, 0});
	m_BossHP.reset(l_Bar);
	m_BossHP->SetAnchorPoint({0.f, 0.0f});
	m_BossHPFrame.reset(BossHPFrame);
	m_BossHPFrame2.reset(BossHPFrame_Inner);

	//オブジェクトの生成と初期化
	Sword = std::make_unique<Object3d>();
	Sword->Initialize(camera);
	Sword->SetModel(ModelManager::GetIns()->GetModel(ModelManager::BOSSWEAPON));
	swordrot = {326.f, 198.f, 297.f};
	Sword->SetScale({9, 9, 9});

	ShieldObj = std::make_unique<Object3d>();

	ShieldObj->Initialize(camera);
	ShieldObj->SetModel(ModelManager::GetIns()->GetModel(ModelManager::SHIELD));
	swordrot = {326.f, 198.f, 297.f};
	ShieldObj->SetScale({38, 42, 38});
}

//初期化処理
void BossEnemy::Initialize()
{
	DebugCamera* camera = CameraControl::GetIns()->GetCamera();

	MaxHP = 690;
	EnemyHP = MaxHP;

	Scale = {0.14f, 0.1f, 0.14f};
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
	FrameScl.x = Percent::GetParcent(static_cast<float>(MaxHP), static_cast<float>(EnemyHP)) * 17.0f;

	//各攻撃処理の初期化

	for (int i = 0; i < BossAttackAction.size(); i++)
	{
		BossAttackAction[i]->Init();
	}
	HalfAttack::GetIns()->Init();
	CircleAttack::GetIns()->SetAttackPhase(false);
	KnockAttack::GetIns()->SetAttackPhase(false);
	HalfAttack::GetIns()->SetAttackPhase(false);
	RushAttack::GetIns()->SetAttackPhase(false);
	FrontCircleAttack::GetIns()->SetAttackPhase(false);
}

//更新処理
void BossEnemy::Update()
{
	if (m_fbxObject == nullptr)
	{
		return;
	}
	//swordrot = { 0,42,164 };

	DebugCamera* camera = CameraControl::GetIns()->GetCamera();

	et += 0.01f;
	//行動遷移
	state_boss->Update(this);
	//if (Input::GetIns()->PushButton(Input::Button_Y))
	{
	}
	DamageTexDisplay();

	Action();

	AttackCollide();
	//fbxアニメーション制御
	//FbxAnimationControl();
	//座標やスケールの反映
	m_fbxObject->SetColor({1.0f, 1.0f, 1.0f, alpha});

	m_fbxObject->SetPosition({Position.x, Position.y + 1.4f, Position.z});
	m_fbxObject->SetRotation(Rotation);
	m_fbxObject->SetScale(Scale);

	m_Object->SetPosition(Position);

	m_fbxObject->Update(m_AnimeLoop, m_AnimeSpeed, m_AnimationStop);
	//攻撃後のクールタイム設定
	AttackCoolTime();
	//攻撃受けたらパーティクル
	DamageParticleSet();
	//持ってる斧の更新
	m_fbxObject->SetHandBoneIndex(hind);

	HPGaugeBoss();
	ColPlayer();
	//持ってる斧の更新
	Sword->Setf(FALSE);
	Sword->SetRotation({303.f, 169.f, 306.f});
	Sword->Update(handmat_left, {1.0f, 1.0f, 1.0f, 1.0f}, camera);

	ShieldObj->Setf(FALSE);
	//ShieldObj->SetPosition(HandPos_Right);
	ShieldObj->SetRotation(swordrot);
	ShieldObj->SetBloomF(true);
	//ShieldObj->SetShadowF(false);
	ShieldObj->Update(handmat_right, {1.0f, 1.0f, 1.0f, 1.0f}, camera);

	Position.x = std::clamp(Position.x, -100.f, 100.f);
	Position.z = std::clamp(Position.z, -100.f, 100.f);

	for (int i = 0; i < BossAttackAction.size(); i++)
	{
		BossAttackAction[i]->Upda();
	}
	HalfAttack::GetIns()->Upda();


	if (GuardAction)
	{
		guardtime++;
	}
	else
	{
		GuarPoint = 0;
		guardtime = 0;
	}
}

void BossEnemy::ColPlayer()
{
}

void BossEnemy::AttackCollide()
{
	//右手のボーン座標取る
	m_fbxObject->GetBoneIndexMat(26, handmat_right);
	HandPos_Right = {
		handmat_right.r[3].m128_f32[0], handmat_right.r[3].m128_f32[1], handmat_right.r[3].m128_f32[2]
	};
	//
	m_fbxObject->GetBoneIndexMat(29, handmat_right2);
	HandPos_Right2 = {
		handmat_right2.r[3].m128_f32[0], handmat_right2.r[3].m128_f32[1], handmat_right2.r[3].m128_f32[2]
	};
	//左
	m_fbxObject->GetBoneIndexMat(69, handmat_left);
	HandPos_Left = {
		handmat_left.r[3].m128_f32[0], handmat_left.r[3].m128_f32[1], handmat_left.r[3].m128_f32[2]
	};
	if (m_Number == BNORMAL)
	{
		if (Collision::GetLength(HandPos_Right,
		                         PlayerControl::GetIns()->GetPlayer()->GetPosition()
		) < 10.f)
		{
			PlayerControl::GetIns()->GetPlayer()->RecvDamage(5);
		}
	}
	//武器
	if (m_Number == BNORMAL2 || m_Number == SWING)
	{
		//手のワールド行列から各成分抜き出し
		HandSiteOBB.SetOBBParam_Pos(Sword->ExtractPositionMat());
		HandSiteOBB.SetOBBParam_Rot(Sword->ExtractRotationMat());
		HandSiteOBB.SetOBBParam_Scl({5.0f, 6.0f, 10.0f});
		//プレイヤーのインスタンス引き出
		Player* l_player = PlayerControl::GetIns()->GetPlayer();

		playerOBB.SetOBBParam_Pos(l_player->GetPosition());
		playerOBB.SetOBBParam_Rot(l_player->GetMatrot());
		playerOBB.SetOBBParam_Scl({3.0f, 9.0f, 3.0f});

		if (Collision::CheckOBBCollision(playerOBB, HandSiteOBB) == true)
		{
			l_player->RecvDamage(10);
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
	//ボス武器描画
	Object3d::PreDraw();
	Sword->Draw();
	ShieldObj->Draw();
	Object3d::PostDraw();


	//ボス敵描画
	Draw_Fbx();

	ImGui::Begin("hin");
	//ImGui::Text("%f", );
	ImGui::Text("%f", ShieldObj->GetRotation().z);
	ImGui::SliderFloat("hz", &rotadds, 0, 360);
	ImGui::End();
	//各攻撃のオブジェクト描画
	for (int i = 0; i < BossAttackAction.size(); i++)
	{
		BossAttackAction[i]->Draw();
	}
	HalfAttack::GetIns()->Draw();
}

void BossEnemy::Death()
{
	if (!DieFlag)
	{
		DieFlag = true;
		//ExpPointSystem::GetIns()->ExpPoint_Get(10);
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
	CameraControl::GetIns()->SetCameraState(CameraControl::RUSHSCENE);

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
		if (m_Number == BNORMAL || m_Number == BNORMAL2)
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
		NowFrameX = Percent::GetParcent(static_cast<float>(MaxHP), static_cast<float>(EnemyHP)) * 17.0f;
		FrameScalingETime += 0.05f;
		if (FrameScl.x > 0.0f)
		{
			FrameScl.x = Easing::EaseOut(FrameScalingETime, OldFrameX, NowFrameX);
		}
	}

	else
	{
		OldFrameX = Percent::GetParcent(static_cast<float>(MaxHP), static_cast<float>(EnemyHP)) * 17.0f;

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
	m_BossHPFrame->SetSize({1760.0f, 117.0f});

	m_BossHP->SetPosition({145.0f, 832.0f});
	m_BossHPFrame2->SetPosition({122.0f, 832.0f});
	m_BossHPFrame->SetPosition({116.0f, 830.0f});
}
