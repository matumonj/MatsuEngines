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
	enemy->SetRecvDamage(false);
	enemy->SetEvaMotionTime(true);
	//õGĶĶ
	const float DetectionRange = 10.0f;

	//Ś®xNgšy²üčĢpxÅń]
	XMVECTOR move = { 0.0f,0.0f,0.1f,0.0f };

	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(enemy->GetRotation().y + (enemy->GetRotRadians()+180.0f)));

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

#include"PlayerAttackState.h"

void BossEnemyFalter::Update(Enemy* enemy)
{
	enemy->SetRecvDamage(false);
	PlayerAttackState::GetInstance()->SetHitStopJudg(true);
	enemy->SetFalterMotion(true);
	if (enemy->GetFbxTime() > enemy->GetFalterTime_End()-0.3f) {
		enemy->ChangeState_Boss(new BossEnemyFollow());
	}
}