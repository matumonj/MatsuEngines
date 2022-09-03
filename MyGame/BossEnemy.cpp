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
	m_fbxModel = FbxLoader::GetInstance()->LoadModelFromFile("monster_golem_demo");

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
	//if (FollowFlag) {
		//m_fbxObject->SetColor({ 1,0,0,1 });
	//} else {
		//m_fbxObject->SetColor({ 1,1,1,1 });
	//}

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


void BossEnemy::FbxAnimationControl()
{
	//アニメーション
		//1フレーム進める
	f_time += 0.006;
	//最後まで再生したら先頭に戻す


	m_fbxObject->SetFbxTime(f_time);

}
void BossEnemy::AttackCoolTime()
{

}