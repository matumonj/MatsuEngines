#include "BossEnemyEvasion.h"
#include "BossEnemyEvasion.h"
#include"PlayerControl.h"
#include"CameraControl.h"
#include"BossEnemyAttack.h"
#include"BossEnemyAttackCircle.h"
#include"BossEnemyAttackknock.h"
#include"BossEnemyAttackhalf.h"
#include"mHelper.h"
#include"BossEnemyDeath.h"
#include"BossEnemyFollow.h"
void BossEnemyEvasion::Initialize(Enemy* enmey)
{

}
void BossEnemyFalter::Initialize(Enemy* enmey)
{

}
void BossEnemyEvasion::Update(Enemy* enemy)
{
	enemy->SetEvaMotionTime(true);
	//õ“G”ÍˆÍ
	const float DetectionRange = 10.0f;

	//ˆÚ“®ƒxƒNƒgƒ‹‚ðyŽ²Žü‚è‚ÌŠp“x‚Å‰ñ“]
	XMVECTOR move = { 0.0f,0.0f,0.1f,0.0f };

	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(enemy->GetRotation().y - 70));

	move = XMVector3TransformNormal(move, matRot);

	enemy->SetPosition({
				enemy->GetPosition().x + move.m128_f32[0] * 3 ,
				enemy->GetPosition().y,
				enemy->GetPosition().z + move.m128_f32[2] * 3 }
	);

	if (Collision::GetLength(enemy->GetPosition(), PlayerControl::GetInstance()->GetPlayer()->GetPosition()) > 20) {
		enemy->ChangeState_Boss(new BossEnemyFollow());
	}
}



void BossEnemyFalter::Update(Enemy* enemy)
{
	enemy->SetFalterMotion(true);
	if (enemy->GetFbxTime() > enemy->GetFalterTime_End()-0.3f) {
		enemy->ChangeState_Boss(new BossEnemyFollow());
	}
}