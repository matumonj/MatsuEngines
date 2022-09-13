#include "BossEnemyStay.h"
#include"BossEnemyFollow.h"
#include"BossEnemyAttack.h"
#include"CameraControl.h"
void BossEnemyStay::Initialize(Enemy* enmey)
{

}

void BossEnemyStay::Update(Enemy* enemy)
{

	if (CameraControl::GetInstance()->GetAttackSceneF()) {
		enemy->ChangeState_Boss(new BossEnemyAttack());
	}
	if (enemy->GetRecvDamage()) {
		enemy->ChangeState_Boss(new BossEnemyFollow());
	}
}