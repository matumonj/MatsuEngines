#include "BossEnemyAttack.h"
#include"BossEnemyFollow.h"
#include"BossEnemyStay.h"
#include"CameraControl.h"
#include"PlayerControl.h"
#include"BossEnemy.h"
void BossEnemyAttack::Initialize(Enemy* enmey)
{
}

void BossEnemyAttack::Update(Enemy* enemy)
{
	enemy->SetAnimation(BossEnemy::NowAttackMotion::BNORMAL , false, 1.0);
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
				enemy->ChangeState_Boss(new BossEnemyFollow());
			}
			else
			{
				enemy->ChangeState_Boss(new BossEnemyStay());
			}
		}

	//	}
		
	}
}
