#include "BossEnemyFollow.h"
#include "EnemyFollowState.h"
#include"PlayerControl.h"
#include"CameraControl.h"
#include"BossEnemyAttack.h"
#include"BossEnemyAttackknock.h"
#include"mHelper.h"
#include"RushAttack.h"
#include"FrontCircleAttack.h"
#include"HalfAttack.h"
#include"CircleAttack.h"
#include"BossEnemyDeath.h"
#include"KnockAttack.h"
#include"BossEnemyAttackSlam.h"
#include"BossEnemyAttackRush.h"
#include"Feed.h"
#include"BossEnemy.h"
#include "BossEnemyAttackBrzBeam.h"
#include "BossEnemyAttackHalf.h"
#include "BossEnemyAttackUlt.h"
#include "BossEnemyEvasion.h"
#include "BossEnemyShieldGuard.h"
#include "BronzeAttack.h"
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
	enemy->SetAnimation(BossEnemy::NowAttackMotion::BWALK , 1.f, true);
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
		Add_RotVal* 60.0f ,
		enemy->GetRotation().z
	});

	const float FollowSpeed = 4.f;

	//移動ベクトルをy軸周りの角度で回転
	XMVECTOR move = { 0.0f, 0.0f, 0.1f, 0.0f };

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
	const bool attackJudg = Collision::GetLength(enemy->GetPosition(), PlayerControl::GetIns()->GetPlayer()->GetPosition()) < 17.f;
	if (attackJudg)
	{
		//クールタイムが０の時のみ
		if (enemy->GetCoolTime() == 0) {
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
		else
		{
			if (Evaprobability > 20) {
				enemy->ChangeState_Boss(new BossEnemyShieldGuard());
				enemy->SetRecvDamage2(false);
			}
		}
	}

	//死亡
	if (enemy->GetHP() <= 0.f)
	{
		enemy->ChangeState_Boss(new BossEnemyDeath());
	}

	/*-----------攻撃遷移部分------------*/

	if (Percent::GetParcent(static_cast<float>(enemy->GetMaxHP()), static_cast<float>(enemy->GetHP())) <= 70.0f)
	{
		if (RushAttack::GetIns()->GetPhaseEnd() != RushAttack::PHASEFOUR)
		{
			if (enemy->GetAttack_End(enemy->Beam) == false) {
				RushAttack::GetIns()->SetAttackPhase(true);
				enemy->ChangeState_Boss(new BossEnemyAttackRush());
			}
		}
	}
	//
	/*if (Percent::GetParcent(static_cast<float>(enemy->GetMaxHP()), static_cast<float>(enemy->GetHP())) <= 60.0f)
	{
		if (KnockAttack::GetIns()->GetPhase() != KnockAttack::PHASEFOUR)
		{
			if (enemy->GetAttack_End(enemy->KNOCK) == false) {
				KnockAttack::GetIns()->SetAttackPhase(true);
				enemy->ChangeState_Boss(new BossEnemyAttackKnock());
			}
		}
	}*/

	if (Percent::GetParcent(static_cast<float>(enemy->GetMaxHP()), static_cast<float>(enemy->GetHP())) <= 50.0f)
	{
		if (UltAttack::GetIns()->GetPhase() != UltAttack::END)
		{
			if (enemy->GetAttack_End(enemy->ULT) == false) {
				UltAttack::GetIns()->SetAction(true);
				enemy->ChangeState_Boss(new BossEnemyAttackUlt());
			}
		}
	}

	if (Percent::GetParcent(static_cast<float>(enemy->GetMaxHP()), static_cast<float>(enemy->GetHP())) <= 20.0f)
	{
		if (BronzeAttack::GetIns()->GetPhase() != BronzeAttack::AREASET)
		{
			if (enemy->GetAttack_End(enemy->BRONZEATTACK_W) == false) {
				BronzeAttack::GetIns()->SetAction(true,BronzeAttack::WIDTH);
				enemy->ChangeState_Boss(new BossEnemyAttackBrzBeam());
			}
		}
	}


	if (Percent::GetParcent(static_cast<float>(enemy->GetMaxHP()), static_cast<float>(enemy->GetHP())) <=30.0f)
	{
		if (BronzeAttack::GetIns()->GetPhase() != BronzeAttack::AREASET)
		{
			if (enemy->GetAttack_End(enemy->BRONZEATTACK_H) == false) {
				BronzeAttack::GetIns()->SetAction(true, BronzeAttack::HEIGHT);
				enemy->ChangeState_Boss(new BossEnemyAttackBrzBeam());
			}
		}
	}
	if (Percent::GetParcent(static_cast<float>(enemy->GetMaxHP()), static_cast<float>(enemy->GetHP())) <= 85.0f)
	{
		if (HalfAttack::GetIns()->GetPhase() != HalfAttack::PHASEFOUR)
		{
			if (enemy->GetAttack_End(enemy->HALF_1) == false) {
				//HalfAttack::GetIns()->SetAction(true, BronzeAttack::HEIGHT);
				enemy->ChangeState_Boss(new BossEnemyAttackHalf());
			}
		}
	}
	//KnockAttack::GetIns()->SetAttackPhase(false);
}

void BossEnemyFollow::AttackSelect(Enemy* enemy, bool judg, int num)
{
	if (judg)
	{
		AttackType(enemy, num);
	}
}

void BossEnemyFollow::AttackStart(Enemy* enemy, int num)
{
	switch (num)
	{
	case enemy->CIRCLE_1:
		enemy->SetRecvDamage2(false);
		
		break;

	case enemy->KNOCK:
		enemy->SetRecvDamage2(false);
		if (enemy->GetAttack_End(enemy->KNOCK) == false)
		{
			KnockAttack::GetIns()->SetAttackPhase(true);
		}
			break;

	case enemy->ULT:
		UltAttack::GetIns()->SetAction(true);
		break;
	case enemy->Beam:
		enemy->SetRecvDamage2(false);
		RushAttack::GetIns()->SetAttackPhase(true);
		break;
	case enemy->Slam:
		FrontCircleAttack::GetIns()->SetAttackPhase(true);
		break;
	default:
		break;
	}
}

void BossEnemyFollow::AttackType(Enemy* enemy, int num)
{
	switch (num)
	{
	case enemy->CIRCLE_1:
		
		break;
	case enemy->CIRCLE_2:
		if (CircleAttack::GetIns()->GetPhaseEnd() != CircleAttack::PHASEFOUR)
		{
			//5enemy->ChangeState_Boss(new BossEnemyAttackCircle());
		}
		break;
	case enemy->KNOCK:
		if (KnockAttack::GetIns()->GetPhase() != KnockAttack::PHASETHREE)
		{
			enemy->ChangeState_Boss(new BossEnemyAttackKnock());
		}
		break;
	case enemy->ULT:
		if (UltAttack::GetIns()->GetPhase() != UltAttack::END)
		{
			enemy->ChangeState_Boss(new BossEnemyAttackUlt());
		}
		break;

	case enemy->Beam:
		if (RushAttack::GetIns()->GetPhaseEnd() != RushAttack::PHASEFOUR)
		{
			enemy->ChangeState_Boss(new BossEnemyAttackRush());
		}
		break;

	case enemy->Slam:
		if (FrontCircleAttack::GetIns()->GetPhaseEnd() != FrontCircleAttack::PHASEFOUR)
		{
			enemy->ChangeState_Boss(new BossEnemyAttackSlam());
		}
		break;
	default:
		break;
	}
}
