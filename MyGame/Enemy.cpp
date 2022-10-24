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


//初期化処理
void Enemy::Initialize(DebugCamera* camera)
{

}

//更新処理
void Enemy::Update(DebugCamera* camera)
{
}
#include"TargetMarker.h"
void Enemy::SearchPlayer(DebugCamera* camera)
{
	if (ArrowTex == nullptr) {
		ArrowAlpha = 1.0f;
		Texture::LoadTexture(64, L"Resources/targetArrow.png");
		Texture* l_tex = { Texture::Create(64, {0.0f ,0.0f ,0.0f}, {100.0f ,100.0f ,1.0f}, {1.0f ,1.0f ,1.0f ,1.0f}) };
		ArrowTex.reset(l_tex);
		ArrowTex->CreateTexture();
		ArrowTex->SetAnchorPoint({ 0.5f,1.0f });
	}
	float len = Collision::GetLength(PlayerControl::GetInstance()->GetPlayer()->GetPosition(), Position);
	XMVECTOR pPos = { PlayerControl::GetInstance()->GetPlayer()->GetPosition().x, PlayerControl::GetInstance()->GetPlayer()->GetPosition().y, PlayerControl::GetInstance()->GetPlayer()->GetPosition().z };
	XMVECTOR ePos = { Position.x,Position.y ,Position.z };
	//プレイヤーと敵のベクトルの長さ(差)を求める
	XMVECTOR SubVector = DirectX::XMVectorSubtract(ePos,pPos);// positionA - positionB;
	ArrowTex->SetAnchorPoint({ 0.5f,1.0f });
	//角度の取得 プレイヤーが敵の索敵位置に入ったら向きをプレイヤーの方に
	ArrowRot = atan2f(SubVector.m128_f32[0], SubVector.m128_f32[2]);
	if (len < 50) {
		SearchF = true;
	}
	if (SearchF) {
		EaseTime_Arrow.x += 1.0f / 120.0f;
		EaseTime_Arrow.y += 1.0f / 120.0f;
		EaseTime_Arrow.z += 1.0f / 120.0f;

		ArrowPos.x=Easing::EaseOut(EaseTime_Arrow.x, Position.x, pPos.m128_f32[0]);
		ArrowPos.y = Easing::EaseOut(EaseTime_Arrow.y, Position.y + 5.0f, pPos.m128_f32[1] + 5.0f);
		ArrowPos.z = Easing::EaseOut(EaseTime_Arrow.z, Position.z, pPos.m128_f32[2]);
	
		if (EaseTime_Arrow.x >= 1.0f) {
			ArrowAlpha -= 0.02f;
		}
	}
	ArrowTex->SetDisplayRadius(200);
	ArrowTex->SetBillboard(FALSE);
	ArrowTex->SetRotation({ 90,
		 ArrowRot * 60 + 180,0});
	ArrowTex->SetScale({ 3.0f,7.0f,1.0f });
	ArrowTex->SetPosition(ArrowPos);
	ArrowTex->SetColor({ 1.0f,1.0f,1.0f,ArrowAlpha });
	ArrowTex->Update(camera);
}

void Enemy::ArrowDraw()
{
	if (ArrowTex == nullptr)return;
	Texture::PreDraw();
	ArrowTex->Draw();
	Texture::PostDraw();
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
//描画処理
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