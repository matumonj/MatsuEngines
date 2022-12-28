#include "Enemy.h"
#include"mHelper.h"
#include"EnemyWalkState.h"
#include"EnemyAttackState.h"
#include"EnemyStayState.h"
#include"EnemyFollowState.h"
#include"EnemyFollowState.h"
#include"BossEnemyStay.h"
#include"PlayerControl.h"
#include"GuardianAppearState.h"
using namespace DirectX;

Enemy::Enemy()
{
	state_boss = new BossEnemyStay();
	state_mob = new EnemyWalkState();
	state_guardian = new GuardianAppearState();
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
	//PlayerAttackState::GetInstance()->SetHitStopJudg(TRUE);
	std::unique_ptr<DamageManager> newdTex;
	if (ENumber == GUARDIAN)
	{
		newdTex = std::make_unique<DamageManager>(
			XMFLOAT3(Position.x + rand() % 10 - 5, -10 + Position.y + rand() % 10 - 5, Position.z), Damage);
	}
	else
	{
		newdTex = std::make_unique<DamageManager>(
			XMFLOAT3(Position.x + rand() % 10 - 5, Position.y + rand() % 10 - 5, Position.z), Damage);
	}
	dMans_.push_back(std::move(newdTex));

	EnemyHP = EnemyHP - Damage;
	DamageParticleCreateF = true;
}

void Enemy::HPFrameUpda()
{
	DebugCamera* camera = CameraControl::GetInstance()->GetCamera();
	FrameScl.x = max(FrameScl.x, 0.0f);
	FrameScl_Inner.x = max(FrameScl_Inner.x, 0.0f);
	XMVECTOR tex2DPos[4];
	for (int i = 0; i < 4; i++)
	{
		tex2DPos[i] = { Position.x, Position.y + 13.0f, Position.z };
		tex2DPos[i] = MatCal::PosDivi(tex2DPos[i], camera->GetViewMatrix(), false);
		tex2DPos[i] = MatCal::PosDivi(tex2DPos[i], camera->GetProjectionMatrix(), true);
		tex2DPos[i] = MatCal::WDivi(tex2DPos[i], false);
		tex2DPos[i] = MatCal::PosDivi(tex2DPos[i], camera->GetViewPort(), false);

		HPFrame[i]->SetPosition({ tex2DPos[i].m128_f32[0] - 80.0f, tex2DPos[i].m128_f32[1] });
	}
	if (RecvDamagef)
	{
		FrameScalingETime_Inner = 0.0f;
		if (!InnerFrameScalingF)
		{
			OldFrameX_Inner = OldFrameX;
		}
		NowFrameX = Percent::GetParcent(static_cast<float>(MaxHP), static_cast<float>(EnemyHP)) * 2.0f;
		FrameScalingETime += 0.05f;
		if (FrameScl.x > 0.0f)
		{
			FrameScl.x = Easing::EaseOut(FrameScalingETime, OldFrameX, NowFrameX);
		}
		if (FrameScalingETime >= 1.0f)
		{
			InnerFrameScalingF = true;
			RecvDamagef = false;
		}
	}

	else
	{
		OldFrameX = Percent::GetParcent(static_cast<float>(MaxHP), static_cast<float>(EnemyHP)) * 2.0f;

		FrameScalingETime = 0.0f;
	}

	if (InnerFrameScalingF)
	{
		FrameScalingETime_Inner += 0.02f;
		//体力が０なったときだけEaseの終わりを０に
		if (EnemyHP <= 0)
		{
			FrameScl_Inner.x = Easing::EaseOut(FrameScalingETime_Inner, OldFrameX_Inner, 0.f);
			InnerFrameScalingF = false;
		} else
		{
			FrameScl_Inner.x = Easing::EaseOut(FrameScalingETime_Inner, OldFrameX_Inner, NowFrameX);
		}
		//ゲージの減りが止まったらフラグ切る
		if (FrameScalingETime_Inner >= 1.0f)
		{
			InnerFrameScalingF = false;
		}
	} else
	{
		FrameScalingETime_Inner = 0.0f;
	}
	HPFrame[3]->SetSize({ FrameScl.x, 15 });
	HPFrame[2]->SetSize({ FrameScl_Inner.x, 15.0f });
	HPFrame[1]->SetSize({ 200.0f, 15.0f });
	HPFrame[0]->SetSize({ 200.0f, 15.0f });

	EnemyName->SetPosition({ tex2DPos[0].m128_f32[0] - 80.0f, tex2DPos[0].m128_f32[1] - 30.f });
	EnemyName->SetSize({ 200.0f, 15.0f });
}

void Enemy::DamageTexDisplay()
{
	//ダメージスプライト生成
	for (std::unique_ptr<DamageManager>& dTex : dMans_)
	{
		dTex->DamageDisPlay(1, {1, 1, 1, 1});
	}

	//アルファ値一定以下なったら破棄
	dMans_.remove_if([](std::unique_ptr<DamageManager>& dTex)
	{
		return dTex->GetAlpha() <= 0.1f;
	});
}

void Enemy::DamageTexDisplay_Draw()
{
	if (this == nullptr)
	{
		return;
	}
	for (std::unique_ptr<DamageManager>& dTex : dMans_)
	{
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

void Enemy::ChangeState_Guardian(GuardianState* state)
{
	Destroy(state_guardian);
	state_guardian = state;
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
