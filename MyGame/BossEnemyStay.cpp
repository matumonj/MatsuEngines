#include "BossEnemyStay.h"
#include"BossEnemyFollow.h"
#include"BossEnemyAttack.h"
#include"CameraControl.h"
#include"BossEnemy.h"
#include "PlayerControl.h"

void BossEnemyStay::Initialize(Enemy* enmey)
{
}

void BossEnemyStay::Update(Enemy* enemy)
{
	
	if (CameraControl::GetInstance()->GetCameraState() == CameraControl::PLAYER) {
		if (Collision::GetLength(enemy->GetPosition(), PlayerControl::GetInstance()->GetPlayer()->GetPosition()) < 18.f) {
			if (enemy->GetCoolTime() == 0) {
				//enemy->ChangeState_Boss(new BossEnemyAttack());
			}
			
		}
		else{
			
		}
		enemy->ChangeState_Boss(new BossEnemyFollow());
	}
	
		if (CameraControl::GetInstance()->GetAttackSceneF() == false)
		{
			enemy->SetAnimation(BossEnemy::NowAttackMotion::BIDLE, true, 1.0);
		} else
		{
			enemy->SetAnimation(BossEnemy::NowAttackMotion::BROAR, false, 1.0);

			if (enemy->GetAnimationTime() >= 2.3f)
			{
				enemy->ChangeState_Boss(new BossEnemyFollow());

			}

		}
	
}
