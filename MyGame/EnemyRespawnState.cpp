#include "EnemyRespawnState.h"
#include"EnemyStayState.h"
void EnemyRespawnState::Initialize(Enemy* enmey)
{
	
}

void EnemyRespawnState::Update(Enemy* enemy)
{
	enemy->isRespawn();
	enemy->ChangeState_Mob(new EnemyStayState());
}