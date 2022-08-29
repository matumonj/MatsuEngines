#include "MobEnemy.h"

#include"SphereCollider.h"
#include"CollisionManager.h"
#include"CollisionAttribute.h"
#include"DebugCamera.h"
#include "PhysicsAction.h"
#include "WalkJudgement.h"
#include"WalkAction.h"
#include"mHelper.h"
#include"imgui.h"
#include"Collision.h"
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
	//delete m_Model;
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

	EnemyHP = 150.00f;
	MaxHP = 150.00f;
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
	m_fbxObject->SetColor({ 1,0,0,alpha });
}

//更新処理
void MobEnemy::Update(DebugCamera* camera)
{
	

	Action();
	if (EnemyHP <= 0) {
		alpha -= 0.005f;
	}

	FbxAnimationControl();
	EnemyPop(150);
	
	//m_fbxObject->SeteCurrent(animeflag);
	AttackCoolTime();
	ParameterSet_Fbx(camera);
	if (FollowFlag) {
		
	} else {
		//m_fbxObject->SetColor({ 0,1,1,alpha });
	}
	CollisionField(camera);
	
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
	
	//..DeathFlag = true;
	//}
}


void MobEnemy::Walk()
{
	onGroundTime++;
	if (wf) {
		if (movement > RandMovement) {
			sf = true;
			wf = false;
		}
	}
	animeflag = false;
}

void MobEnemy::Stop()
{
	if (StayCount == 0) {
		endsearch = false;
		//イージング掛ける前の敵の向き
		BeforeRot = Rotation.y;
		//掛けた後の敵の向き
		AfterRot = Rotation.y + RandMove;
	}

	if (sf) {
		movement = 0;
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
			wf = true;
			sf = false;
		}
	}
}


void MobEnemy::Follow()
{
	//PlayerControl::GetInstance()->GetPlayer()->RecvDamage(10);
	Turn_toPlayer();
	if (time > 3000) {
		FollowFlag = false;
		wf = true;
		endsearch = true;
		RecvDamageJudg = false;
		time = 0;
	} else {
		endsearch = false;
		FollowFlag = true;
		time++;
		wf = false;
		sf = false;
		if (Collision::GetLength(Position, PlayerControl::GetInstance()->GetPlayer()->GetPosition()) < 10)
		{
			folatc = true;
		}
		else {
			folatc = false;
		}
	}
}

void MobEnemy::FbxAnimationControl()
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
				f_time = DeathTime;
				nowDeath = true;
				DeathFlag = false;
				
			} else {
				if (!AttackFlag && !nowDeath && f_time >= end_time) {
					//f_time = 0;
				}
			}
	if (f_time > DeathTime) {
		nowAttack = false;
	}
	
	m_fbxObject->SetFbxTime(f_time);
}



void MobEnemy::Attack()
{
	endsearch = true;
	if (state != NOW_ATTACK) {
		
		Turn_toPlayer();
		wf = false;
		//PlayerControl::GetInstance()->GetPlayer()->RecvDamage(10);
		//m_fbxObject->SetAttackFlag(true);
		f_AttackFlag = true;
		state = NOW_ATTACK;
	}


}
void MobEnemy::AttackCoolTime()
{
	if(state == NOW_ATTACK){

		if (f_time >= DeathTime-1) {
			AfterAttack = true;
			state =AFTER_ATTACK;
		}
	}
	if (AfterAttack) {
		
		cooltime++;
		if (cooltime > 480) {
			wf = true;
			state = None;
			AfterAttack = false;
		}
	}
	else {
		cooltime = 0;
	}
	
}

