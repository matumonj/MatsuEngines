#include "EnemyDeathState.h"
#include"EnemyStayState.h"
#include "EnemyRespawnState.h"
#include "PlayerAttackState.h"

void EnemyDeathState::Initialize(Enemy* enmey)
{
}

void EnemyDeathState::Update(Enemy* enemy)
{
	enemy->SetAnimeState(enemy->DEATH);
	enemy->Death();
	if (enemy->GetObjAlpha() <= 0.0f)
	{
		enemy->ChangeState_Mob(new EnemyRespawnState());
	}
}
