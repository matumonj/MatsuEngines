#include "Enemy.h"
#include"mHelper.h"
#include"EnemyWalkState.h"
#include"EnemyAttackState.h"
#include"EnemyStayState.h"
#include"EnemyFollowState.h"
#include"EnemyFollowState.h"
#include"BossEnemyStay.h"
#include"DamageManager.h"
#include"PlayerControl.h"
using namespace DirectX;
Enemy::Enemy()
{
	state_boss= new BossEnemyStay();
	state_mob = new EnemyWalkState();
}

Enemy::~Enemy()
{
	delete particleMan,particleMan2;
	delete  state_mob;
	delete state_boss;
	delete SlashTex;
}


//‰Šú‰»ˆ—
void Enemy::Initialize(DebugCamera* camera)
{

}

//XVˆ—
void Enemy::Update(DebugCamera* camera)
{
}


void Enemy::Action()
{
	AttackCoolTime();
}

#include"AttackCollision.h"
#include"CameraControl.h"
void Enemy::RecvDamage(int Damage) 
{
	if (EnemyHP <= 0)return;
	float texZ= PlayerControl::GetInstance()->GetPlayer()->GetPosition().z- Position.z ;
	float texX= PlayerControl::GetInstance()->GetPlayer()->GetPosition().x - Position.x;
	DamageManager::GetIns()->DamageDisPlay(Damage, { 1,1,1,1 }, { Position.x,Position.y + 20, CameraControl::GetInstance()->GetCamera()->GetEye().z + 40.0f });

	EnemyHP = EnemyHP - Damage;
	DamageParticleCreateF = true;
}

void Enemy::isRespawn()
{
	EnemyHP = MaxHP;
	alpha = 1.0f;
	Position = RespawnPos;
	f_time = 0;
	DeathFlag = false;
	nowDeath = false;
}

void Enemy::EnemyPop(int HP)
{
	

}
//•`‰æˆ—
void Enemy::Draw()
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