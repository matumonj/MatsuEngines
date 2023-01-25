#include "BossEnemyAttack.h"
#include"BossEnemyFollow.h"
#include"BossEnemyStay.h"
#include"CameraControl.h"
#include"PlayerControl.h"
#include"BossEnemy.h"
#include "BossEnemyDeath.h"
#include "BossEnemyEvasion.h"

void BossEnemyAttack::Initialize(Enemy* enmey)
{
}

void BossEnemyAttack::Update(Enemy* enemy)
{
	int Evaprobability;
	if (enemy->GetRecvDamage2())
	{
		Evaprobability = rand() % 100 + 1;
		if (Evaprobability > 30)
		{
			enemy->ChangeState_Boss(new BossEnemyFalter());
		} else
		{
			enemy->SetRecvDamage2(false);
		}
	}
	//enemy->ChangeState_Boss(new BossEnemyFollow());
	if(randmotion<40)
	{
		enemy->SetAnimation(BossEnemy::NowAttackMotion::BNORMAL, false, 1.0);
	}
	else if(randmotion<80)
	{
		enemy->SetAnimation(BossEnemy::NowAttackMotion::SWING, false, 1.0);
	}
	else
	{
		enemy->SetAnimation(BossEnemy::NowAttackMotion::BNORMAL2, false, 1.0);
	}
	//PlayerControl::GetInstance()->GetPlayer()->RecvDamage(2);
	if (CameraControl::GetInstance()->GetCameraState() == CameraControl::BOSSCUTSCENE)
	{
		enemy->ChangeState_Boss(new BossEnemyStay());
	}
	else
	{
		//if (enemy->GetNowMotion() == enemy->BNORMAL) {
		if (enemy->GetAnimationTime() >= enemy->GetFbxTimeEnd() - 0.1) {
			if (Collision::GetLength(enemy->GetPosition(), PlayerControl::GetInstance()->GetPlayer()->GetPosition()) > 17.f)
			{
				randmotion = 0;
					enemy->ChangeState_Boss(new BossEnemyFollow());
			}
			else
			{
				randmotion = 0;
					enemy->ChangeState_Boss(new BossEnemyStay());
			}
		}
		else {
			if (randmotion == 0) {
				randmotion = rand() % 100 + 1;
			}
		}
	}

	//Ž€–S
	if (enemy->GetHP() <= 0.f)
	{
		enemy->ChangeState_Boss(new BossEnemyDeath());
	}

}
