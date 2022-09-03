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
#include"SceneManager.h"
#include"EnemyFollowState.h"
using namespace DirectX;
Enemy::Enemy()
{
	state_mob = new EnemyWalkState();
}

Enemy::~Enemy()
{
	delete _state, state_mob;
}


//初期化処理
void Enemy::Initialize(DebugCamera* camera)
{

}

//更新処理
void Enemy::Update(DebugCamera* camera)
{
}


void Enemy::Action()
{
	AttackCoolTime();
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

void Enemy::ChangeState_Mob(EnemyState* state)
{
	delete state_mob;
	state_mob = state;
}