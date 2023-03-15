#include "BossEnemyAttack.h"
#include"BossEnemyFollow.h"
#include"BossEnemyStay.h"
#include"CameraControl.h"
#include"PlayerControl.h"
#include"BossEnemy.h"
#include "BossEnemyAttackCircle.h"
#include "BossEnemyDeath.h"
#include "BossEnemyEvasion.h"
#include "BossEnemyShieldGuard.h"
#include "UI.h"

void BossEnemyAttack::Initialize(Enemy* enmey)
{
}

void BossEnemyAttack::Update(Enemy* enemy)
{
	int Evaprobability;
	//...Damage2()ー＞２とかのマジックナンバーよくない
	if (enemy->GetRecvDamage2())
	{
		//被ダメージ時
		//乱数を生成し値が一定以上なら怯み状態
		Evaprobability = rand() % 100 + 1;
		
		 if(Evaprobability > 75)
		{
			enemy->ChangeState_Boss(new BossEnemyShieldGuard());
		}else if (Evaprobability > 40)
		{
			PlayerControl::GetIns()->GetPlayer()->SetKnockF(true);

			UI::GetIns()->SetRadBlur(true);
			enemy->ChangeState_Boss(new BossEnemyAttackCircle());

			//enemy->ChangeState_Boss(new BossEnemyFalter());
		}
		else
		{
			enemy->ChangeState_Boss(new BossEnemyFollow());
		}
	}
	enemy->SetGuardAction(false);
	//ランダムで攻撃方法を選出
	//ランダム生成処理も専用の関数作ったほうが。。
	const int SideNum = 30;
	const int HeightNum = 60;
	if (randmotion < HeightNum)
	{
		//縦振り
		enemy->SetAnimation(BossEnemy::NowAttackMotion::BNORMAL, false, 1.0);
	}
	else if (randmotion < SideNum)
	{
		//上振り
		enemy->SetAnimation(BossEnemy::NowAttackMotion::SWING, false, 1.0);
	}
	else
	{
		//コンボ攻撃
		enemy->SetAnimation(BossEnemy::NowAttackMotion::BNORMAL2, false, 1.0);
	}

	if (CameraControl::GetIns()->GetCameraState() == CameraControl::BOSSCUTSCENE)
	{
		enemy->ChangeState_Boss(new BossEnemyStay());
	}
	else
	{
		if (enemy->GetAnimationTime() >= enemy->GetFbxTimeEnd() - 0.1)
		{
			//一定以上離れたら状態を追跡に
			if (Collision::GetLength(enemy->GetPosition(), PlayerControl::GetIns()->GetPlayer()->GetPosition()) > 17.f)
			{
				randmotion = 0;
				enemy->ChangeState_Boss(new BossEnemyFollow());
			}
			//それ以外は待機
			else
			{
				randmotion = 0;
				enemy->ChangeState_Boss(new BossEnemyStay());
			}
		}
		else
		{
			//0のときだけ乱数を生成するように　正直やり方は変えたい
			if (randmotion == 0)
			{
				randmotion = rand() % 100 + 1;
			}
		}
	}

	//死亡
	if (enemy->GetHP() <= 0.f)
	{
		enemy->ChangeState_Boss(new BossEnemyDeath());
	}
}
