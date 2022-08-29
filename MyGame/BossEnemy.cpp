#include "BossEnemy.h"

#include"SphereCollider.h"
#include"CollisionManager.h"
#include"CollisionAttribute.h"
#include"DebugCamera.h"
#include"mHelper.h"
/// <summary>
/// コンストラクタ
/// </summary>
using namespace DirectX;
BossEnemy::BossEnemy(BehaviorTree* ai_tree, float max_hp, float max_mp, float attack, float deffence) :
	Enemy(ai_tree, max_hp, max_mp, attack, deffence)
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
	m_fbxModel = FbxLoader::GetInstance()->LoadModelFromFile("sniper_blender");

	RandMove = rand() % 90 + 20;
	RandMovement = rand() % 100 + 80;

	EnemyHP = 250.0f;

	Texture::LoadTexture(11, L"Resources/Sertch.png");
	SearchPlayerTexture = Texture::Create(11, { 0,-50,50 }, { 1,1,1 }, { 1,1,1,1 });
	SearchPlayerTexture->CreateTexture();
	//パラメータのセット

	Rotation = { -70,180,0 };


	m_fbxObject = std::make_unique<f_Object3d>();
	m_fbxObject->Initialize();
	m_fbxObject->SetModel(m_fbxModel);
	m_fbxObject->PlayAnimation();
	radius_adjustment = 0;
	Scale = { 0.04f, 0.04f, 0.04f
	};
	SetCollider();
	AttackTime = 1.5f;
	DeathTime = 4.9f;
	f_time = 200 / 60;
}

//更新処理
void BossEnemy::Update( DebugCamera* camera)
{
	if (FollowFlag) {
		m_fbxObject->SetColor({ 1,0,0,1 });
	} else {
		m_fbxObject->SetColor({ 1,1,1,1 });
	}

	Action();

	FbxAnimationControl();
	EnemyPop(150);

	//m_fbxObject->SeteCurrent(animeflag);

	ParameterSet_Fbx(camera);
	CollisionField(camera);

}

//描画処理
void BossEnemy::Draw()
{
	Draw_Fbx();
}
//解放処理
void BossEnemy::Finalize()
{

}

void BossEnemy::Death()
{
	DeathFlag = true;
	m_fbxObject->SetDeathFlag(true);
	DeathFlag = true;
}

void BossEnemy::Walk()
{
	onGroundTime++;
	if (wf) {
		if (movement > RandMovement) {
			wf = false;
			movement = 0;
			sf = true;
		}
	}
	animeflag = false;
}

void BossEnemy::Stop()
{
	if (StayCount == 0) {
		endsearch = false;
		//イージング掛ける前の敵の向き
		BeforeRot = Rotation.y;
		//掛けた後の敵の向き
		AfterRot = Rotation.y + RandMove;
	}

	if (sf) {
		StayCount++;

		if (StayCount > 190) {//停止時間
			RotTime += 0.01f;
			Rotation = {
				Rotation.x,
				//enemy->GetRotation().y+80,
				Easing::EaseOut(RotTime,BeforeRot, AfterRot),
				Rotation.z
			};
		}

		if (Rotation.y >= AfterRot) {
			RotTime = 0;
			StayCount = 0;
			sf = false;
			wf = true;
		}
	}
	if (sf) {
		animeflag = true;

	}
	if (wf) {
		animeflag = false;
	}
}


void BossEnemy::Follow()
{
	animeflag = false;

	Turn_toPlayer();

	if (time > 210) {
		wf = true;
		endsearch = true;
		RecvDamageJudg = false;
		FollowFlag = false;
		time = 0;
	} else {
		time++;
		FollowFlag = true;
		wf = false;
		sf = false;
	}
}

void BossEnemy::FbxAnimationControl()
{
	//アニメーション
		//1フレーム進める
	f_time += 0.006;
	//最後まで再生したら先頭に戻す


	m_fbxObject->SetFbxTime(f_time);

}
void BossEnemy::Attack()
{
	if (state != NOW_ATTACK) {
		Turn_toPlayer();
		wf = false;
		Player::GetInstance()->RecvDamage(10);
		//m_fbxObject->SetAttackFlag(true);
		f_AttackFlag = true;
		state = NOW_ATTACK;
	}

	if (f_time > AttackTime) {
		AfterAttack = true;
		state = None;

	}
}

void BossEnemy::AttackCoolTime()
{

}