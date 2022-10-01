#include "EnemyDeathState.h"
#include"EnemyRespawnState.h"
void EnemyDeathState::Initialize(Enemy* enmey)
{

}

void EnemyDeathState::Update(Enemy* enemy)
{
	enemy->Death();
	RespawnCount++;
	if (RespawnCount > 180) {
		enemy->ChangeState_Mob(new EnemyRespawnState());
		RespawnCount = 0;
	}
}