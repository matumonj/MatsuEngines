#include "BossEnemyStay.h"
#include"BossEnemyFollow.h"
#include"BossEnemyAttack.h"
#include"CameraControl.h"
void BossEnemyStay::Initialize(Enemy* enmey)
{

}

void BossEnemyStay::Update(Enemy* enemy)
{
	if (CameraControl::GetInstance()->GetAttackSceneF()==false) {
		enemy->SetIdleMotion(true);	
	}
	else {
		enemy->SetRoarMotion(true);
		enemy->ChangeState_Boss(new BossEnemyFollow());
	}
	if (enemy->GetRecvDamage()) {
		enemy->ChangeState_Boss(new BossEnemyFollow());
	}
}