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
void Enemy::InitAction()
{
	//_state = new EnemyWalkState();
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
		if (wf && MoveFlag) {
			Position = {
				Position.x + move.m128_f32[0],
				Position.y,
				Position.z + move.m128_f32[2] }
			;
			movement++;
			//enemy->SetMovement(enemy->GetMovement() + 1);
		}

	} else if (onGround == false) {
		if (MoveFlag != false) {
			Position = { tempx,Position.y,tempz };
		}
	}
	if (EnemyHP <= 0) {
		state = DEAD;
	}
	move = { 0,0,0.1f,0 };

	matRot = XMMatrixRotationY(XMConvertToRadians(Rotation.y));

	move = XMVector3TransformNormal(move, matRot);

}
void Enemy::RecvDamage(int Damage) 
{
	RecvDamageJudg = true;
	EnemyHP = EnemyHP - Damage;
}
void Enemy::EnemyPop(int HP)
{
	if (state == DEAD) {
		PopCount++;
		if (PopCount >600) {
			Position = StartPosition;
			EnemyHP = HP;
			wf = true;
			state = ALIVE; 
			
			
		}
	}
	else {
		PopCount = 0;
		
	}

}
//描画処理
void Enemy::Draw()
{
}

//解放処理
void Enemy::Finalize()
{
}

float Enemy::Distance(Player* player)
{

		distance = sqrtf(((player->GetPosition().x - Position.x) * (player->GetPosition().x - Position.x))
			+ ((player->GetPosition().y - Position.y) * (player->GetPosition().y - Position.y))
			+ ((player->GetPosition().z - Position.z) * (player->GetPosition().z - Position.z)));
		return distance;
	
}
void Enemy::EnemySearchPlayer(Player* player)
{

}

void Enemy::ChangeState(EnemyState* newState)
{
	delete _state;
	_state = newState;
}



void Enemy::Walk()
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

void Enemy::Stop()
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

void Enemy::Follow()
{
	animeflag = false;
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

	Rotation={0,RotY * 60 + 180,0 };
	//座標のセット
	if (Collision::GetLength(Player::GetInstance()->GetPosition(), Position) > 10) {
		Position = { Position.x + (angleX / dis) * centerSpeed,Position.y,Position.z + (angleZ / dis) * centerSpeed };
	}
	if (time> 210) {
		wf = true;
		endsearch = true;
		RecvDamageJudg = false;
		FollowFlag = false;
		time = 0;
	}
	else {
		time++;
		FollowFlag = true;
		wf = false;
		sf = false;
	}
}

void Enemy::Attack()
{
	
	AttackFlag = true;
	Player::GetInstance()->RecvDamage(10);
	if (AttackFlag) {
		AttackFlag=false;
		AfterAttack = true;
	}
}
int Enemy::AttackCoolTime()
{
	if (AfterAttack) {
		cooltime++;
		if (cooltime > 180) {
			AfterAttack = false;
			cooltime = 0;
	}
	}
	return cooltime;
}
bool Enemy::GetSearchPlayer_Distance() {
	if (Collision::GetLength(Player::GetInstance()->GetPosition(), Position) < 10) { return true; }
	else { return false; }
}

