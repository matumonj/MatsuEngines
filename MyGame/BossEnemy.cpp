#include "BossEnemy.h"

#include "CameraControl.h"
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
}

/// <summary>
/// デストラクタ
/// </summary>
BossEnemy::~BossEnemy()
{
}

//初期化処理
void BossEnemy::Initialize(DebugCamera* camera)
{
	m_Object = std::make_unique<Object3d>();
	m_Object->Initialize(camera);
	//m_Object->CreateGraphicsPipeline(L"Resources/Shader/Object3dVS.hlsl", L"Resources/Shader/Object3dPS.hlsl", L"Resources/Shader/BasicGS.hlsl");
	MaxHP = 2000.0f;
	EnemyHP = MaxHP;

	Scale = {0.15f, 0.1f, 0.15f};
	Rotation = {66.0f, 252.0f, 20.0f};

	m_fbxObject = std::make_unique<f_Object3d>();
	m_fbxObject->Initialize();
	m_fbxObject->SetModel(FbxLoader::GetInstance()->LoadModelFromFile("BossGolem"));
	m_fbxObject->PlayAnimation();
	radius_adjustment = 0;

	SetCollider();
	cooltime = 0;
	AttackTime = 51.000f / 60.000f;
	NormalAttackTime_End = 215.000f / 60.000f;
	MagicAttackTime = 216.000f / 60.00f;
	MagicAttackTime_End = 360.000f / 60.000f;
	EvaTime = 370.000f / 60.000f;
	EvaTime_End = 422.000f / 60.000f;
	FalterTime = 428.00f / 60.00f;
	FalterTime_End = 524.00f / 60.00f;
	RoarTime = 528.00f / 60.00f;
	RoarTime_End = 700.00f / 60.00f;
	IdleTime = 708.00f / 60.00f;
	IdleTime_End = 1066.00f / 60.00f;
	DeathTime = 4.9f;
	DeathFlag = false;
	f_time = 200 / 60;

	FollowRotAngleCorrect = 252.0f;
	addRotRadians = 323.0f;
	state_boss->Initialize(this);

	particleMan = ParticleManager::Create(4, L"Resources/ParticleTex/Attack.png");
	particleMan2 = ParticleManager::Create(6, L"Resources/ParticleTex/Attack.png");

}
//更新処理
void BossEnemy::Update(DebugCamera* camera)
{
	et += 0.01f;
	//行動遷移
	state_boss->Update(this);
	m_fbxObject->SetHandBoneIndex(hand);

	DamageTexDisplay();
	if (DeathFlag)
	{
		alpha -= 0.005f;
	}
	Action();

	AttackCollide();
	//fbxアニメーション制御
	FbxAnimationControl();
	//座標やスケールの反映
	ParameterSet_Fbx(camera);
	//攻撃後のクールタイム設定
	AttackCoolTime();
	//地形当たり判定
	CollisionField(camera);

	//攻撃受けたらパーティクル
	DamageParticleSet();
}

void BossEnemy::AttackCollide()
{
	HandSiteOBB.SetOBBParam_Pos(m_fbxObject->ExtractPositionMat(m_fbxObject->GetHandBoneMatWorld()));
	HandSiteOBB.SetOBBParam_Rot(m_fbxObject->ExtractRotationMat(m_fbxObject->GetHandBoneMatWorld()));
	HandSiteOBB.SetOBBParam_Scl({5.0f, 5.0f, 5.0f});

	playerOBB.SetOBBParam_Pos(PlayerControl::GetInstance()->GetPlayer()->GetPosition());
	playerOBB.SetOBBParam_Rot(PlayerControl::GetInstance()->GetPlayer()->GetMatrot());
	playerOBB.SetOBBParam_Scl({1.0f, 5.0f, 1.0f});


	if (f_time >= AttackTime + 1.0f)
	{
		if (Collision::CheckOBBCollision(playerOBB, HandSiteOBB) == true)
		{
			PlayerControl::GetInstance()->GetPlayer()->RecvDamage(10);
		}
	}
}

//描画処理
void BossEnemy::Draw()
{
	ImGui::Begin("rotx");
	ImGui::SliderInt("rx", &cooltime, 0, 100);
	ImGui::SliderFloat("roty", &Rotation.y, -180, 360);
	ImGui::SliderFloat("rotz", &Rotation.z, -180, 360);
	ImGui::End();
	ImGui::Begin("we");
	ImGui::Text("%d", nowMotion);
	ImGui::SliderFloat("rotfolow", &FollowRotAngleCorrect, -180, 360);
	ImGui::SliderFloat("rotadd", &addRotRadians, -180, 360);
	ImGui::End();
	if (alpha < 0)return;
	Draw_Fbx();
	if (GigaBossObj != nullptr)
	{
		GigaBossObj->Draw();
	}
	// 3Dオブジェクト描画前処理
	ParticleManager::PreDraw();
	// 3Dオブクジェクトの描画
	particleMan->Draw();
	particleMan2->Draw();
	// 3Dオブジェクト描画後処理
	ParticleManager::PostDraw();

	
}

void BossEnemy::Death()
{
	if (!DeathFlag)
	{
		DeathFlag = true;
	}
}


void BossEnemy::FbxAnimationControl()
{
	if (nowMotion == ROAR && f_time <= RoarTime + 0.2f)
	{
		f_time += 0.006f;
	}
	else
	{
		f_time += 0.015f;
	}
	SetMotion(f_AttackFlag, NORMAL, AttackTime, NormalAttackTime_End);
	SetMotion(MagicMotionStart, MAGIC, MagicAttackTime, MagicAttackTime_End);
	SetMotion(EvaMotionStart, EVASION, EvaTime, EvaTime_End);
	SetMotion(FalterFlag, FALTER, FalterTime, FalterTime_End);
	SetMotion(IdleMotionFlag, IDLE, IdleTime, IdleTime_End);
	SetMotion(RoarMotionFlag, ROAR, RoarTime, RoarTime_End);

	if (nowMotion == NON && f_time > AttackTime)
	{
		f_time = 0.0f;
	}

	if (DeathFlag)
	{
		f_time = DeathTime;
		nowDeath = true;
		DeathFlag = false;
	}
	if (f_time > DeathTime)
	{
		nowAttack = false;
	}

	m_fbxObject->SetFbxTime(f_time);
}

void BossEnemy::SetMotion(bool& motionStartJudg, NowAttackMotion motion, float actionStartTime, float actionEndTime)
{
	if (motionStartJudg)
	{
		nowMotion = motion;
		f_time = actionStartTime;
		motionStartJudg = false;
	}

	if (nowMotion == motion && (f_time >= actionEndTime))
	{
		AfterAttack = true;
		nowMotion = NON;
	}
}

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
		cooltime = 0;
	}
}

void BossEnemy::DamageParticleSet()
{
	for (int i = 0; i < ParticleSize; i++)
	{
		const float rnd_vel = 0.5f;
		XMFLOAT3 vel{};
		vel.x = static_cast<float>(rand()) / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = static_cast<float>(rand()) / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = static_cast<float>(rand()) / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		XMFLOAT3 acc{};
		const float rnd_acc = 0.001f;
		acc.y = -static_cast<float>(rand()) / RAND_MAX * rnd_acc;


		if (DamageParticleCreateF)
		{
			particlePos = {Position.x, Position.y + 10, Position.z};
			particleMan->Add(particleLife, particlePos, vel, acc, 3.0f, 0.0f);
			if (i == ParticleSize - 1)
			{
				DamageParticleCreateF = false;
			}
		}
	}
	particleMan->SetColor({1.0f, 0.2f, 0.2f, 0.7f});
	particleMan->Update(particleMan->NORMAL);
}
