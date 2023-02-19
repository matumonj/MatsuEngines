#include "BossEnemyFollow.h"
#include "EnemyFollowState.h"
#include"PlayerControl.h"
#include"CameraControl.h"
#include"BossEnemyAttack.h"
#include"mHelper.h"
#include"RushAttack.h"
#include"FrontCircleAttack.h"
#include"BossEnemyDeath.h"
#include"BossEnemyAttackRush.h"
#include"Feed.h"
#include"BossEnemy.h"
#include "BossEnemyAttackBrzBeam.h"
#include "BossEnemyAttackCircle.h"
#include "BossEnemyAttackHalf.h"
#include "BossEnemyAttackUlt.h"
#include "BossEnemyEvasion.h"
#include "BossEnemyShieldGuard.h"
#include "BronzeAttack.h"
#include "HalfAttack.h"
#include "UltAttack.h"

void BossEnemyFollow::Initialize(Enemy* enmey)
{
}

void BossEnemyFollow::Update(Enemy* enemy)
{

	if (Feed::GetIns()->GetAlpha() > 0.0f)
	{
		return;
	}
	//モーションセット
	enemy->SetAnimation(BossEnemy::NowAttackMotion::BWALK, 1.f, true);
	//追跡処理部分//////////

	//敵がプエレイヤーの方向く処理
	XMVECTOR positionA = {
		PlayerControl::GetIns()->GetPlayer()->GetPosition().x,
		PlayerControl::GetIns()->GetPlayer()->GetPosition().y,
		PlayerControl::GetIns()->GetPlayer()->GetPosition().z
	};
	XMVECTOR positionB = {enemy->GetPosition().x, enemy->GetPosition().y, enemy->GetPosition().z};

	//向きをプレイヤーに
	float Add_RotVal = FollowRot::FollowA_B(positionA, positionB);

	enemy->SetRotation({
		enemy->GetRotation().x,
		Add_RotVal * 60.0f,
		enemy->GetRotation().z
	});

	const float FollowSpeed = 4.f;

	//移動ベクトルをy軸周りの角度で回転
	XMVECTOR move = {0.0f, 0.0f, 0.1f, 0.0f};

	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(enemy->GetRotation().y + enemy->GetRotRadians()));

	move = XMVector3TransformNormal(move, matRot);

	if (Collision::GetLength(enemy->GetPosition(), PlayerControl::GetIns()->GetPlayer()->GetPosition()) > 15.f)
	{
		enemy->SetPosition({
				enemy->GetPosition().x + move.m128_f32[0] * FollowSpeed,
				enemy->GetPosition().y,
				enemy->GetPosition().z + move.m128_f32[2] * FollowSpeed
			}
		);
	}

	//////////////////////////////////////////////////////////////


	//普通の攻撃
	const bool attackJudg = Collision::GetLength(enemy->GetPosition(),
	                                             PlayerControl::GetIns()->GetPlayer()->GetPosition()) < 17.f;
	if (attackJudg)
	{
		//クールタイムが０の時のみ
		if (enemy->GetCoolTime() == 0)
		{
			enemy->ChangeState_Boss(new BossEnemyAttack());
		}
	}
	//マジックナンバー後々消す
	if (enemy->GetRecvDamage2())
	{
		//乱数を生成->一定以上なら状態を怯みに
		Evaprobability = rand() % 100 + 1;
		if (Evaprobability > 69)
		{
			enemy->ChangeState_Boss(new BossEnemyFalter());
		}
		else if (Evaprobability > 40)
			{
				enemy->ChangeState_Boss(new BossEnemyShieldGuard());
				enemy->SetRecvDamage2(false);
			}
		else
		{
			enemy->SetRecvDamage2(false);
			
		}
	}

	//死亡
	if (enemy->GetHP() <= 0.f)
	{
		enemy->ChangeState_Boss(new BossEnemyDeath());
	}

	/*-----------攻撃遷移部分------------*/
	ActionSequence(enemy,90.f,FrontCircleAttack::GetIns(), enemy->CIRCLE_1,
		new BossEnemyAttackCircle());

	ActionSequence(enemy, 80.f, FrontCircleAttack::GetIns(), enemy->CIRCLE_2,
		new BossEnemyAttackCircle());

	ActionSequence(enemy, 70.f,RushAttack::GetIns(), enemy->Beam,
		new BossEnemyAttackRush());

	ActionSequence(enemy, 50.f, UltAttack::GetIns(), enemy->ULT,
		new BossEnemyAttackUlt());

	ActionSequence(enemy, 20.f, BronzeAttack::GetIns(), enemy->BRONZEATTACK_W,
		new BossEnemyAttackBrzBeam());

	ActionSequence(enemy, 30.f, BronzeAttack::GetIns(), enemy->BRONZEATTACK_H,
		new BossEnemyAttackBrzBeam());

	ActionSequence(enemy, 85.f, HalfAttack::GetIns(), enemy->HALF_1,
		new BossEnemyAttackHalf());

	//KnockAttack::GetIns()->SetAttackPhase(false);
}

void BossEnemyFollow::ActionSequence(Enemy* enemy, float percent,BossAttackActionManager*action, int actionnum, BossEnemyState* state)
{
	if (Percent::GetParcent(static_cast<float>(enemy->GetMaxHP()), static_cast<float>(enemy->GetHP())) <= percent)
	{
		if (action->GetPhase() !=action->PHASE_FOUR)
		{
			if (enemy->GetAttack_End(actionnum) == false)
			{
				action->SetAttackPhase(true);
				enemy->ChangeState_Boss(state);
			}
		}
	}

	
}

