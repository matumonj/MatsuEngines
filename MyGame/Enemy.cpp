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

#include"PlayerAttackState.h"
#include"CameraControl.h"
void Enemy::RecvDamage(int Damage) 
{
	if (this==nullptr||EnemyHP < 0)return;
	float texZ= PlayerControl::GetInstance()->GetPlayer()->GetPosition().z- CameraControl::GetInstance()->GetCamera()->GetEye().z;
	float texX= PlayerControl::GetInstance()->GetPlayer()->GetPosition().x - CameraControl::GetInstance()->GetCamera()->GetEye().x;
	damageDisF = true;
	DamageSize = Damage;
	DamageTexPos = Position;
	DamageManager::GetIns()->SetTexSize(2.0f);
	DamageManager::GetIns()->SetAlpha(1.0f);
	if (EnemyHP <= 10) {
		
	}
	EnemyHP = EnemyHP - Damage;
	DamageParticleCreateF = true;
}
void Enemy::DestroyJudg()
{
	if (f_time >= m_fbxObject->GetEndTime()) { 
		//alpha -= 0.05f;
	}
	
}
void Enemy::DamageTexDisplay()
{
	if (damageDisF) {
		DamageManager::GetIns()->DamageDisPlay(DamageSize, { 1.0f,1.0f,1.0f,1.0f }, DamageTexPos);
	}
}
void Enemy::isRespawn()
{
	EnemyHP = MaxHP;
	alpha = 1.0f;
	Position = RespawnPos;
	//f_time = 0;
	//DeathFlag = false;
	//nowDeath = false;
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