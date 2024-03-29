#include "EnemyStayState.h"
#include "EnemyWalkState.h"
#include"EnemyAttackState.h"
#include"EnemyFollowState.h"
#include"mHelper.h"
#include"EnemyDeathState.h"

#include"EnemyKnockState.h"
#include "SceneManager.h"

void EnemyStayState::Initialize(Enemy* enemy)
{
}

void EnemyStayState::Update(Enemy* enemy)
{
	//待機時間
	constexpr int StayMaxCount = 90;

	enemy->ResetRespawnCount();
	enemy->SetAnimeState(enemy->WALK);
	float RandMove = static_cast<float>(rand() % 90 + 40);


	if (SceneManager::GetIns()->GetScene() == SceneManager::BOSS)
	{
		enemy->ChangeState_Mob(new EnemyFollowState());
	}
	else
	{
		if (StayCount == 0)
		{
			//イージング掛ける前の敵の向き
			BeforeRot = enemy->GetRotation().y;
			//掛けた後の敵の向き
			AfterRot = enemy->GetRotation().y + RandMove;
		}

		StayCount++;

		if (StayCount > StayMaxCount)
		{
			//停止時間
			RotTime += 0.01f;
			enemy->SetRotation({
				enemy->GetRotation().x,
				Easing::EaseOut(RotTime, BeforeRot, AfterRot),
				enemy->GetRotation().z
			});
			if (RotTime > 0.6)
			{
				enemy->ChangeState_Mob(new EnemyWalkState());
			}
		}
	}
	if (enemy->GetRecvDamage2())
	{
		enemy->ChangeState_Mob(new EnemyKnockState());
	}
	if (enemy->GetHP() <= 0.0f)
	{
		enemy->ChangeState_Mob(new EnemyDeathState());
	}
}
