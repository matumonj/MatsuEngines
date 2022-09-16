#include "BossEnemy.h"
#include"CustomButton.h"
#include"SphereCollider.h"
#include"CollisionManager.h"
#include"CollisionAttribute.h"
#include"DebugCamera.h"
#include"mHelper.h"
/// <summary>
/// コンストラクタ
/// </summary>
using namespace DirectX;
BossEnemy::BossEnemy()
{
	//m_BehaviorData = new BehaviorDatas();
}
/// <summary>
/// デストラクタ
/// </summary>
BossEnemy::~BossEnemy()
{
	//	delete  m_Model;
	delete mob, MobModel;
}

//初期化処理
void BossEnemy::Initialize(DebugCamera* camera)
{
	m_Object = std::make_unique<Object3d>();
	m_Object->Initialize(camera);

	EnemyHP = 300.0f;

	Rotation = { -70,180,0 };

	MaxHP = 300.0f;
	m_fbxObject = std::make_unique<f_Object3d>();
	m_fbxObject->Initialize();
	m_fbxObject->SetModel(FbxLoader::GetInstance()->LoadModelFromFile("monster_golem"));
	m_fbxObject->PlayAnimation();
	radius_adjustment = 0;
	Scale = { 0.04f, 0.04f, 0.04f
	};
	SetCollider();
	AttackTime = 1.5f;
	DeathTime = 4.9f;
	DeathFlag = false;
	f_time = 200 / 60;
	state_boss->Initialize(this);
}

//更新処理
void BossEnemy::Update(DebugCamera* camera)
{
	Action();
	if (CustomButton::GetInstance()->GetAttackAction()) {
		EnemyHP -= 10;
	}
	FbxAnimationControl();
	EnemyPop(150);

	ParameterSet_Fbx(camera);
	AttackCoolTime();
	CollisionField(camera);

	state_boss->Update(this);
}

//描画処理
void BossEnemy::Draw()
{
	Draw_Fbx();
}

void BossEnemy::Death()
{
	if (f_time < DeathTime) {
		DeathFlag = true;
	}


}


void BossEnemy::FbxAnimationControl()
{
	//アニメーション
		//1フレーム進める
	f_time += 0.02;
	//最後まで再生したら先頭に戻す

	if (f_AttackFlag) {
		f_time = AttackTime;
		f_AttackFlag = false;
		nowAttack = true;
	} else {
		if (nowDeath == false) {
			if (!nowAttack && f_time >= AttackTime) {
				f_time = 0;
			}
		}
	}

	if (DeathFlag) {
		nowDeath = true;
		f_time = DeathTime;
		DeathFlag = false;

	}
	if (f_time > DeathTime) {
		//nowDeath = false;
		nowAttack = false;
	}

	m_fbxObject->SetFbxTime(f_time);

}
void BossEnemy::AttackCoolTime()
{
	if (f_AttackFlag) {

		if (f_time >= DeathTime - 1) {
			AfterAttack = true;
		}
	}
	if (AfterAttack) {
		cooltime++;
		if (cooltime > 480) {
			AfterAttack = false;
		}
	} else {
		cooltime = 0;
	}
}