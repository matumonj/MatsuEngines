#include "Enemy.h"
#include"mHelper.h"
#include"EnemyWalkState.h"
#include"EnemyAttackState.h"
#include"EnemyStayState.h"
#include"EnemyFollowState.h"
#include"EnemyFollowState.h"
#include"BossEnemyFollow.h"
using namespace DirectX;
Enemy::Enemy()
{
	state_boss= new BossEnemyFollow();
	state_mob = new EnemyWalkState();
}

Enemy::~Enemy()
{
	delete particleMan;
	delete  state_mob;
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
	EnemyHP = EnemyHP - Damage;
	DamageParticleCreateF = true;
}
void Enemy::EnemyPop(int HP)
{
	

}
//描画処理
void Enemy::Draw()
{
}

void Enemy::Turn_toPlayer()
{
	
}

void Enemy::ChangeState_Mob(EnemyState* state)
{
	delete state_mob;
	state_mob = state;
}

void Enemy::ChangeState_Boss(BossEnemyState* state)
{
	delete state_boss;
	state_boss = state;
}