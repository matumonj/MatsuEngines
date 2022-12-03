#include "BossEnemyFollow.h"
#include "EnemyFollowState.h"
#include"PlayerControl.h"
#include"CameraControl.h"
#include"BossEnemyAttack.h"
#include"BossEnemyAttackCircle.h"
#include"BossEnemyAttackknock.h"
#include"BossEnemyAttackhalf.h"
#include"mHelper.h"
#include"AltAttack.h"
#include"FrontCircleAttack.h"
#include"HalfAttack.h"
#include"CircleAttack.h"
#include"BossEnemyDeath.h"
#include"KnockAttack.h"
#include"BossEnemyAttackSlam.h"
#include"BossEnemyAttackBeam.h"
#include"BossEnemyEvasion.h"

void BossEnemyFollow::Initialize(Enemy* enmey)
{
}

#include"CustomButton.h"

void BossEnemyFollow::Update(Enemy* enemy)
{
	//’ÇÕˆ—•”•ª//////////
	//õ“G”ÍˆÍ
	const float DetectionRange = 10.0f;
	//ƒvƒŒƒCƒ„[‚ªõ“G”ÍˆÍ“ü‚Á‚½‚ç
	bool SearchPlayer = Collision::GetLength(enemy->GetPosition(),
		PlayerControl::GetInstance()->GetPlayer()->GetPosition()) < DetectionRange;


	if (enemy->GetFbxTime() < 51.000f / 60.000f) {
	//“G‚ªƒvƒGƒŒƒCƒ„[‚Ì•ûŒü‚­ˆ—
	XMVECTOR positionA = {
		PlayerControl::GetInstance()->GetPlayer()->GetPosition().x,
		PlayerControl::GetInstance()->GetPlayer()->GetPosition().y,
		PlayerControl::GetInstance()->GetPlayer()->GetPosition().z
	};
	XMVECTOR positionB = { enemy->GetPosition().x, enemy->GetPosition().y, enemy->GetPosition().z };
	//ƒvƒŒƒCƒ„[‚Æ“G‚ÌƒxƒNƒgƒ‹‚Ì’·‚³(·)‚ð‹‚ß‚é
	XMVECTOR SubVector = XMVectorSubtract(positionB, positionA); // positionA - positionB;

	//Šp“x‚ÌŽæ“¾ ƒvƒŒƒCƒ„[‚ª“G‚Ìõ“GˆÊ’u‚É“ü‚Á‚½‚çŒü‚«‚ðƒvƒŒƒCƒ„[‚Ì•û‚É
	RotY = atan2f(SubVector.m128_f32[0], SubVector.m128_f32[2]);
	//ˆÚ“®ƒxƒNƒgƒ‹‚ðyŽ²Žü‚è‚ÌŠp“x‚Å‰ñ“]
	XMVECTOR move = { 0.0f, 0.0f, 0.1f, 0.0f };

	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(enemy->GetRotation().y + enemy->GetRotRadians()));

	move = XMVector3TransformNormal(move, matRot);
	enemy->SetRotation({
		enemy->GetRotation().x,
		RotY * 60.0f + enemy->GetRotCorrect(),
		enemy->GetRotation().z
		});
	if (Collision::GetLength(enemy->GetPosition(), PlayerControl::GetInstance()->GetPlayer()->GetPosition()) > 10)
	{
		enemy->SetPosition({
				enemy->GetPosition().x + move.m128_f32[0]*5,
				enemy->GetPosition().y,
				enemy->GetPosition().z + move.m128_f32[2]*5
			}
		);
	}
}
	//////////////////////////////////////////////////////////////

	//•’Ê‚ÌUŒ‚
	if (Collision::GetLength(enemy->GetPosition(), PlayerControl::GetInstance()->GetPlayer()->GetPosition()) < 15)
	{
		if (enemy->GetCoolTime() == 0)
		{
			enemy->ChangeState_Boss(new BossEnemyAttack());
		}
	}
	if(CustomButton::GetInstance()->GetAttackAction())
	if (PlayerControl::GetInstance()->GetPlayer()->GetAttackType()!= PlayerControl::GetInstance()->GetPlayer()->GetAttackTypeOld())
	{
		Evaprobability = rand() % 100 + 1;
		if (Evaprobability > 80)
		{
			enemy->ChangeState_Boss(new BossEnemyEvasion());
		}
		else
		{
			enemy->SetRecvDamage(false);
		}
	}


	/*2ˆø”F‘Ì—ÍÝ’è(Ý’è’lˆÈ‰º‚È‚Á‚½‚ç‚Rˆø”‚ÌUŒ‚‚Ö)*/
	AttackSelect(enemy, Percent::GetParcent(enemy->GetMaxHP(), enemy->GetHP()) <= 30.0f, enemy->HALF_1);
	AttackSelect(enemy, Percent::GetParcent(enemy->GetMaxHP(), enemy->GetHP()) <= 60.0f, enemy->CIRCLE_1);
	
	//Ž€–S
	if (enemy->GetHP() <= 0.f)
	{
		enemy->ChangeState_Boss(new BossEnemyDeath());
	}
}

void BossEnemyFollow::AttackSelect(Enemy* enemy, bool judg, int num)
{
	if (judg)
	{
		AttackStart(enemy, num);
		if (enemy->GetAttack_End(num) == false)
		{
			enemy->SetAttack_Start(num, true);
		}
		else
		{
			enemy->SetAttack_Start(num, false);
		}
		if (enemy->GetAttack_Start(num))
		{
			AttackType(enemy, num);
		}
	}
}

void BossEnemyFollow::AttackStart(Enemy* enemy, int num)
{
	switch (num)
	{
	case enemy->CIRCLE_1:
		CircleAttack::GetInstance()->SetAttackFase(true);
		break;
	case enemy->CIRCLE_2:
		CircleAttack::GetInstance()->SetAttackFase(true);
		break;
	case enemy->KNOCK:
		KnockAttack::GetInstance()->SetAttackFase(true);
		break;
	case enemy->HALF_1:
		HalfAttack::GetInstance()->SetAttackFase(true);
		break;
	case enemy->HALF_2:
		HalfAttack::GetInstance()->SetAttackFase(true);
		break;
	case enemy->Beam:
		AltAttack::GetInstance()->SetAttackFase(true);
		break;
	case enemy->Slam:
		FrontCircleAttack::GetInstance()->SetAttackFase(true);
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
		if (CircleAttack::GetInstance()->GetFaseEnd() != CircleAttack::FASEFOUR)
		{
			enemy->ChangeState_Boss(new BossEnemyAttackCircle());
		}
		break;
	case enemy->CIRCLE_2:
		if (CircleAttack::GetInstance()->GetFaseEnd() != CircleAttack::FASEFOUR)
		{
			enemy->ChangeState_Boss(new BossEnemyAttackCircle());
		}
		break;
	case enemy->KNOCK:
		if (KnockAttack::GetInstance()->GetFase() != KnockAttack::FASETHREE)
		{
			enemy->ChangeState_Boss(new BossEnemyAttackKnock());
		}
		break;
	case enemy->HALF_1:
		if (HalfAttack::GetInstance()->GetFaseEnd() != HalfAttack::FASEFOUR)
		{
			enemy->ChangeState_Boss(new BossEnemyAttackHalf());
		}
		break;

	case enemy->HALF_2:
		if (HalfAttack::GetInstance()->GetFaseEnd() != HalfAttack::FASEFOUR)
		{
			enemy->ChangeState_Boss(new BossEnemyAttackHalf());
		}
		break;

	case enemy->Beam:
		if (AltAttack::GetInstance()->GetFaseEnd() != AltAttack::FASEFOUR)
		{
			enemy->ChangeState_Boss(new BossEnemyAttackBeam());
		}
		break;

	case enemy->Slam:
		if (FrontCircleAttack::GetInstance()->GetFaseEnd() != FrontCircleAttack::FASEFOUR)
		{
			enemy->ChangeState_Boss(new BossEnemyAttackSlam());
		}
		break;
	default:
		break;
	}
}
