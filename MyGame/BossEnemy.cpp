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
	MaxHP = 200.0f;
	EnemyHP = MaxHP;

	Scale = { 0.15f, 0.1f, 0.15f };
	Rotation = { 96.0f, -48.0f, -101.0f };
	Sword = std::make_unique<Object3d>();
	Sword->Initialize(camera);
	Sword->SetModel(ModelManager::GetIns()->GetModel(ModelManager::WOOD));
	Sword->SetRotation({ 0, 0 + 30, 0 + 100 });

	m_fbxObject = std::make_unique<f_Object3d>();
	m_fbxObject->Initialize();
	m_fbxObject->SetModel(ModelManager::GetIns()->GetFBXModel(ModelManager::BOSS));
	m_fbxObject->PlayAnimation();
	radius_adjustment = 0;
	AttackTime = 51.000f / 60.000f;
	cooltime = 0;
	NormalAttackTime = 51.000f / 60.000f;
	AttackTime_End = 215.000f / 60.000f;
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
	SwingTime = 1078.00f / 60.00f;
	SwingTime_End =

		SideWalk_LeftTime = 640.00f * 2.00f / 60.00f;;
	SideWalk_LeftTime_End = 676.00f * 2.00f / 60.0f;

	SideWalk_RightTime = 680.00f * 2.00f / 60.00f;
	SideWalk_RightTime_End = 715.00f * 2.00f / 60.00f;

	DeathMotionTime_Start = 720.00f * 2.00f / 60.00f;

	DeathTime = 4.9f;
	DeathFlag = false;
	f_time = 200 / 60;

	FollowRotAngleCorrect = -85.0f;
	addRotRadians = -111.0f;

	state_boss->Initialize(this);
}

//更新処理
void BossEnemy::Update(DebugCamera* camera)
{
	et += 0.01f;
	//行動遷移
	state_boss->Update(this);

	DamageTexDisplay();

	Action();

	AttackCollide();
	//fbxアニメーション制御
	FbxAnimationControl();
	//座標やスケールの反映
	ParameterSet_Fbx(camera);
	//攻撃後のクールタイム設定
	AttackCoolTime();
	//地形当たり判定

	//攻撃受けたらパーティクル
	DamageParticleSet();


	m_fbxObject->SetFbxTime(f_time);
	m_fbxObject->SetHandBoneIndex(hind);
	Sword->Setf(FALSE);
	Sword->SetScale({ 10,10,10 });
	Sword->SetRotation({ -23, 43, 83 });
	Sword->Update(m_fbxObject->GetHandBoneMatWorld(), { 1.0f, 1.0f, 1.0f, 1.0f }, camera);


}

void BossEnemy::AttackCollide()
{
	HandSiteOBB.SetOBBParam_Pos(m_fbxObject->ExtractPositionMat(m_fbxObject->GetHandBoneMatWorld()));
	HandSiteOBB.SetOBBParam_Rot(m_fbxObject->ExtractRotationMat(m_fbxObject->GetHandBoneMatWorld()));
	HandSiteOBB.SetOBBParam_Scl({ 5.0f, 5.0f, 5.0f });

	playerOBB.SetOBBParam_Pos(PlayerControl::GetInstance()->GetPlayer()->GetPosition());
	playerOBB.SetOBBParam_Rot(PlayerControl::GetInstance()->GetPlayer()->GetMatrot());
	playerOBB.SetOBBParam_Scl({ 1.0f, 5.0f, 1.0f });


	if (f_time >= NormalAttackTime + 1.0f)
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
	ImGui::SliderInt("indx", &hind, 0, 60);
	ImGui::SliderFloat("fbxt", &Rotation.x, -180, 360);

	ImGui::SliderFloat("fbxy", &Rotation.y, -180, 360);
	ImGui::SliderFloat("rotz", &Rotation.z, -180, 360);
	ImGui::End();
	ImGui::Begin("we");
	ImGui::Text("%d", nowMotion);
	ImGui::SliderFloat("rotfolow", &FollowRotAngleCorrect, -180, 360);
	ImGui::SliderFloat("rotadd", &addRotRadians, -180, 360);
	ImGui::End();
	if (alpha < 0)
	{
		return;
	}
	Draw_Fbx();
	// 3Dオブジェクト描画前処理
	Object3d::PreDraw();
	//Sword->Draw();
	Object3d::PostDraw();
	// 3Dオブジェクト描画前処理
}

void BossEnemy::Death()
{
	if (!DeathFlag)
	{
		//ExpPointSystem::GetInstance()->ExpPoint_Get(10);
		DeathFlag = true;
		if (f_time < DeathMotionTime_Start)
		{
			f_time = DeathMotionTime_Start;
		}

	}
	if (f_time > m_fbxObject->GetEndTime()) {
		alpha -= 0.01f;
	}

	f_time += 0.009f;
	movestop = false;
}


void BossEnemy::FbxAnimationControl()
{
	if (DeathFlag)return;
	if (nowMotion == ROAR && f_time <= RoarTime + 0.2f)
	{
		f_time += 0.006f;
	} else
	{
		f_time += 0.015f;
	}
	SetMotion(f_AttackFlag, NORMAL, NormalAttackTime, AttackTime_End);
	SetMotion(MagicMotionStart, MAGIC, MagicAttackTime, MagicAttackTime_End);
	SetMotion(EvaMotionStart, EVASION, EvaTime, EvaTime_End);
	SetMotion(FalterFlag, FALTER, FalterTime, FalterTime_End);
	SetMotion(IdleMotionFlag, IDLE, IdleTime, IdleTime_End);
	SetMotion(RoarMotionFlag, ROAR, RoarTime, RoarTime_End);
	SetMotion(SwingFlag, SWING, SwingTime, SwingTime_End);
	SetMotion(SideWalk_LeftMotionFlag, LSIDEWALK, SideWalk_LeftTime, SideWalk_LeftTime_End);
	SetMotion(SideWalk_RightMotionFlag, RSIDEWALK, SideWalk_RightTime, SideWalk_RightTime_End);
	SetMotion(DieFlag, DEATH, DeathMotionTime_Start, DeathMotionTime_End);

	if (nowMotion == NON && f_time > NormalAttackTime)
	{
		f_time = 0.0f;
	}

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
	} else
	{
		cooltime = 0;
	}
}

void BossEnemy::DamageParticleSet()
{
}
