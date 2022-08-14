#include "Enemy.h"
#include"mHelper.h"
#include"EnemyWalkState.h"
#include"EnemyAttackState.h"
#include"EnemyStayState.h"
#include"EnemyFollowState.h"
#include "NodeBase.h"
#include"Collision.h"
#include"mHelper.h"
#include"SphereCollider.h"
#include"CollisionManager.h"
#include"CollisionAttribute.h"
#include"imgui.h"
using namespace DirectX;
Enemy::Enemy(BehaviorTree* ai_tree, float max_hp, float max_mp, float attack, float deffence) :
	m_AiTree(ai_tree),
	m_BehaviorData(NULL),
	m_ActiveNode(NULL)
{
	
	m_BehaviorData = new BehaviorDatas();
}

Enemy::~Enemy()
{
	delete m_BehaviorData;
	delete _state;
	delete SearchTex;
}


//初期化処理
void Enemy::Initialize(DebugCamera* camera)
{

}

//更新処理
void Enemy::Update(DebugCamera* camera)
{
}

void Enemy::SearchInit()
{
	//普通のテクスチャ(スプライトじゃないよ)
	Texture::LoadTexture(64, L"Resources/Sertch.png");
	SearchTex = Texture::Create(64, { 0,0,0 }, { 1,1,1 }, { 1,1,1,1 });
	SearchTex->CreateTexture();
}
void Enemy::SearchAction(DebugCamera* camera)
{
	//追跡中でないときに追跡開始フラグ立てる(ビックリマーク)
	if (!SearchNowFlag) {
		searchFlag = false;
		searchTexSet = true;
	}
	else {
		searchTexSet = false;
	}
	//stime->びっくりアイコン出してる時間
	if (stime == 0 && searchTexSet) {
		if (Collision::GetLength(Player::GetInstance()->GetPosition(),Position) <10) {
			searchFlag = true;
		}
	}

	if (searchFlag) {
		stime++;
		searchMarkDrawFlag = true;
		if(stime >20){
			searchMarkDrawFlag = false;
			searchFlag = false;
		}
	}
	else {
		stime = 0;
	}
	SearchTex->SetAnchorPoint({ 0.0,0 });
	SearchTex->SetRotation({ 180,0,0 });
	SearchTex->SetScale({ 1,1,1 });
	SearchTex->SetPosition({ Position.x,Position.y+10,Position.z });
	SearchTex->Update(camera);
}
void Enemy::SearchDraw()
{
	Texture::PreDraw();
	if (searchFlag &&searchMarkDrawFlag) {
		SearchTex->Draw();
	}
	Texture::PostDraw();
}

void Enemy::Action()
{
	if (m_ActiveNode == NULL)
	{
		m_ActiveNode = m_AiTree->Inference(this, m_BehaviorData);
	}

	if (m_ActiveNode != NULL)
	{
		m_ActiveNode = m_AiTree->Run(this, m_ActiveNode, m_BehaviorData);
	}
	if (onGround == true) {

		if (onGroundTime % 30 == 0) {
			tempx = Position.x;
			tempz = Position.z;
		}
		if (FollowFlag == true||wf) {
			Position = {
				Position.x + move.m128_f32[0],
				Position.y,
				Position.z + move.m128_f32[2] }
			;
		}
		
			movement++;
			//enemy->SetMovement(enemy->GetMovement() + 1);
		

	} else if (onGround == false) {
		if (MoveFlag != false) {
			Position = { tempx,Position.y,tempz };
		}
	}

	
		move = { 0,0,0.1f,0 };
	
	matRot = XMMatrixRotationY(XMConvertToRadians(Rotation.y));

	move = XMVector3TransformNormal(move, matRot);

	AttackCoolTime();

	
		//m_fbxObject->SeteCurrent(animeflag);
		
}
void Enemy::RecvDamage(int Damage) 
{
	RecvDamageJudg = true;
	EnemyHP = EnemyHP - Damage;
}
void Enemy::EnemyPop(int HP)
{
	

}
//描画処理
void Enemy::Draw()
{
}

//解放処理
void Enemy::Finalize()
{
}



void Enemy::Turn_toPlayer()
{
	float angleX, angleZ, dis;
	//追跡スピード
	float centerSpeed = 0.2f;

	angleX = (Player::GetInstance()->GetPosition().x - Position.x);
	angleZ = (Player::GetInstance()->GetPosition().z - Position.z);

	//敵とプレイヤーの距離求め
	dis = sqrtf((Position.x - Player::GetInstance()->GetPosition().x) * (Position.x - Player::GetInstance()->GetPosition().x)
		+ (Position.z - Player::GetInstance()->GetPosition().z) * (Position.z - Player::GetInstance()->GetPosition().z));

	//敵がプエレイヤーの方向く処理
	XMVECTOR positionA = { Player::GetInstance()->GetPosition().x,Player::GetInstance()->GetPosition().y, Player::GetInstance()->GetPosition().z };
	XMVECTOR positionB = { Position.x,Position.y,Position.z };
	//プレイヤーと敵のベクトルの長さ(差)を求める
	XMVECTOR SubVector = DirectX::XMVectorSubtract(positionB, positionA);// positionA - positionB;

	//角度の取得 プレイヤーが敵の索敵位置に入ったら向きをプレイヤーの方に
	RotY = atan2f(SubVector.m128_f32[0], SubVector.m128_f32[2]);

	Rotation.y = RotY * 60 + 180;
}