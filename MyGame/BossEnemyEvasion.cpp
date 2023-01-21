#include "BossEnemyEvasion.h"

#include "BossEnemy.h"
#include"PlayerControl.h"
#include"BossEnemyAttackThrow.h"
#include"ThrowRockAttack.h"
#include "PlayerAttackState.h"

void BossEnemyEvasion::Initialize(Enemy* enmey)
{
}

void BossEnemyFalter::Initialize(Enemy* enmey)
{
}

void BossEnemyEvasion::Update(Enemy* enemy)
{
	enemy->SetRecvDamage(false);

	enemy->SetAnimation(BossEnemy::NowAttackMotion::BJUMP, true, 1.2);
	//õ“G”ÍˆÍ
	const float DetectionRange = 25.0f;

	//ˆÚ“®ƒxƒNƒgƒ‹‚ðyŽ²Žü‚è‚ÌŠp“x‚Å‰ñ“]
	XMVECTOR move = {0.0f, 0.0f, 0.1f, 0.0f};

	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(enemy->GetRotation().y + (enemy->GetRotRadians() + 180.0f)));

	move = XMVector3TransformNormal(move, matRot);
	if (enemy->GetAnimationTime() >= enemy->GetFbxTimeEnd() - 2.1f&&
		enemy->GetAnimationTime() < enemy->GetFbxTimeEnd() - 0.9f)
	{
		enemy->SetPosition({
				enemy->GetPosition().x + move.m128_f32[0] * 9.f,
				enemy->GetPosition().y,
				enemy->GetPosition().z + move.m128_f32[2] * 9.f
			}
		);
	}

	if (enemy->GetAnimationTime() >= enemy->GetFbxTimeEnd() - 0.1f)
	{
		ThrowRockAttack::GetInstance()->SetAction(true);
		enemy->ChangeState_Boss(new BossEnemyAttackThrow());
	}
}
void BossEnemyFalter::Update(Enemy* enemy)
{
	enemy->SetRecvDamage2(false);
	enemy->SetAnimation(BossEnemy::NowAttackMotion::FALTER,false, 0.4);

	PlayerAttackState::GetInstance()->SetHitStopJudg(true,120);
	if(enemy->GetAnimationTime()>=enemy->GetFbxTimeEnd()-0.1f)
	{
		enemy->ChangeState_Boss(new BossEnemyEvasion());
	}
}
