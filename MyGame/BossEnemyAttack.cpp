#include "BossEnemyAttack.h"
#include"BossEnemyFollow.h"
#include"BossEnemyStay.h"
#include"CameraControl.h"
void BossEnemyAttack::Initialize(Enemy* enmey)
{

}

void BossEnemyAttack::Update(Enemy* enemy)
{
	enemy->SetAttackTime(true);
	if (CameraControl::GetInstance()->GetCameraState() == CameraControl::BOSSCUTSCENE) {
		enemy->ChangeState_Boss(new BossEnemyStay());
	} else {
		enemy->ChangeState_Boss(new BossEnemyFollow());
	}
}