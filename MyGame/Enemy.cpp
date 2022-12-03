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
	Destroy(state_mob);
	Destroy(state_boss);
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

	RecvDamagef2 = true;
	RecvDamagef = true;
	DamageSize = Damage;
	DamageTexPos = Position;
	if (EnemyHP <= 10)
	{
	}

	std::unique_ptr<DamageManager>newdTex = std::make_unique<DamageManager>(XMFLOAT3(Position.x + rand() % 10 - 5, Position.y + rand() % 10 - 5, Position.z), Damage);

	dMans_.push_back(std::move(newdTex));

	EnemyHP = EnemyHP - Damage;
	DamageParticleCreateF = true;
}

void Enemy::DamageTexDisplay()
{
	//ダメージスプライト生成
	for (std::unique_ptr<DamageManager>& dTex : dMans_) {
		dTex->DamageDisPlay(1, { 1,1,1,1 });
	}

	//アルファ値一定以下なったら破棄
	dMans_.remove_if([](std::unique_ptr<DamageManager>& dTex) {
		return dTex->GetAlpha() <= 0.1f; });
}

void Enemy::DamageTexDisplay_Draw()
{
	if (this == nullptr)return;
	for (std::unique_ptr<DamageManager>& dTex : dMans_) {
		dTex->Draw();
	}
}

void Enemy::isRespawn()
{
	
	//f_time = 0;
	//DeathFlag = false;
	//nowDeath = false;
}

void Enemy::EnemyPop(int HP)
{
}


void Enemy::ChangeState_Mob(EnemyState* state)
{
	Destroy(state_mob);
	state_mob = state;
}

void Enemy::ChangeState_Boss(BossEnemyState* state)
{
	Destroy(state_boss);
	state_boss = state;
}
