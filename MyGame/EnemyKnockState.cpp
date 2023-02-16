#include "EnemyKnockState.h"
#include"EnemyFollowState.h"
#include"PlayerControl.h"
#include <SelectSword.h>

#include "PlayerAttackState.h"

void EnemyKnockState::Initialize(Enemy* enmey)
{
}

void EnemyKnockState::Update(Enemy* enemy)
{
	enemy->SetRecvDamage2(false);
	//õ“G”ÍˆÍ
	const float DetectionRange = 10.0f;

	//ˆÚ“®ƒxƒNƒgƒ‹‚ðyŽ²Žü‚è‚ÌŠp“x‚Å‰ñ“]
	XMVECTOR move = {0.0f, 0.0f, 0.1f, 0.0f};

	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(enemy->GetRotation().y + enemy->GetRotRadians() + 180.0f));

	move = XMVector3TransformNormal(move, matRot);

	enemy->SetPosition({
			enemy->GetPosition().x + move.m128_f32[0] * 15,
			enemy->GetPosition().y,
			enemy->GetPosition().z + move.m128_f32[2] * 15
		}
	);
	//PlayerAttackState::GetIns()->SetHitStopJudg(true, 50);
	if (Collision::GetLength(enemy->GetPosition(), PlayerControl::GetIns()->GetPlayer()->GetPosition()) >
		SelectSword::GetIns()->GetSword()->GetKnockPower())
	{
		enemy->ChangeState_Mob(new EnemyFollowState());
	}
}
