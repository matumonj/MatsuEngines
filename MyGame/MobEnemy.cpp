#include "MobEnemy.h"

#include"SphereCollider.h"
#include"CollisionManager.h"
#include"CollisionAttribute.h"
#include"DebugCamera.h"
#include "PhysicsAction.h"
#include "WalkJudgement.h"
#include"WalkAction.h"
#include"StayJudgment.h"
#include"StayAction.h"

/// <summary>
/// コンストラクタ
/// </summary>
using namespace DirectX;
MobEnemy::MobEnemy(BehaviorTree* ai_tree, float max_hp, float max_mp, float attack, float deffence) :
	Enemy(ai_tree, max_hp, max_mp, attack, deffence)
{

}
/// <summary>
/// デストラクタ
/// </summary>
MobEnemy::~MobEnemy()
{
	delete SearchPlayerTexture;
	//m_Object.reset();
	//delete mob, MobModel;
}

//初期化処理
void MobEnemy::Initialize(DebugCamera* camera)
{
	m_Object = std::make_unique<Object3d>();
	m_Object->Initialize(camera);
	m_fbxModel = FbxLoader::GetInstance()->LoadModelFromFile("monster_golem_demo");

	RandMove = rand() % 90 + 20;
	RandMovement = rand() % 100 + 80;

	EnemyHP = 150.0f;
	
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
	m_fbxObject->SetAttackTime(1.5f);
	m_fbxObject->SetDeathTime(4.9f);
}

//更新処理
void MobEnemy::Update(DebugCamera* camera)
{
	if (FollowFlag) {
		m_fbxObject->SetColor({ 1,0,0,1 });
	} else {
		m_fbxObject->SetColor({ 1,1,1,1 });
	}
	
	Action();

	EnemyPop(150);

	//m_fbxObject->SeteCurrent(animeflag);

	ParameterSet_Fbx(camera);
	CollisionField(camera);
	if (m_fbxObject->GetIsEnd()) {
		state = DEAD;
	}
}

//描画処理
void MobEnemy::Draw()
{
	Draw_Fbx();
	Texture::PreDraw();
	SearchPlayerTexture->Draw();
	Texture::PostDraw();
}
//解放処理
void MobEnemy::Finalize()
{
	delete SearchPlayerTexture;
	//delete mob, MobModel;
}


void MobEnemy::Death()
{
	DeathFlag = true;
	m_fbxObject->SetDeathFlag(true);
	
	//}
}
