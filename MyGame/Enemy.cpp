#include "Enemy.h"
#include"mHelper.h"
#include"EnemyWalkState.h"
#include"EnemyAttackState.h"
#include"EnemyStayState.h"
#include"EnemyFollowState.h"
#include"EnemyFollowState.h"
#include"BossEnemyStay.h"
#include"PlayerControl.h"
using namespace DirectX;

Enemy::Enemy()
{
	state_boss = new BossEnemyStay();
	state_mob = new EnemyWalkState();
}

Enemy::~Enemy()
{
	delete state_mob;
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
	if (this == nullptr || EnemyHP < 0)
	{
		return;
	}

	RecvDamagef = true;
	DamageSize = Damage;
	DamageTexPos = Position;
	if (EnemyHP <= 10)
	{
	}

	std::unique_ptr<DamageManager>newdTex = std::make_unique<DamageManager>(XMFLOAT3(Position.x+rand()%10-5,Position.y+rand()%10-5,Position.z), Damage);

	dMans_.push_back(std::move(newdTex));

	EnemyHP = EnemyHP - Damage;
	DamageParticleCreateF = true;
}

void Enemy::DestroyJudg()
{
	if (f_time >= m_fbxObject->GetEndTime())
	{
		//alpha -= 0.05f;
	}
}

void Enemy::DamageTexDisplay()
{
	for (std::unique_ptr<DamageManager>&dTex:dMans_) {
		dTex->DamageDisPlay(1, { 1,1,1,1 });
	}
	dMans_.remove_if([](std::unique_ptr<DamageManager>& dTex) {
		return dTex->GetAlpha() <= 0.1f;});
}
void Enemy::DamageTexDisplay_Draw()
{
	for (std::unique_ptr<DamageManager>& dTex : dMans_) {
		dTex->Draw();
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
