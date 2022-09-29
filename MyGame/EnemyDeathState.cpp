#include "EnemyDeathState.h"
#include"EnemyRespawnState.h"
void EnemyDeathState::Initialize(Enemy* enmey)
{

}

void EnemyDeathState::Update(Enemy* enemy)
{
	int RespawnCount = 0;
	enemy->Death();
	RespawnCount++;
	//if (RespawnCount > 180) {
		enemy->ChangeState_Mob(new EnemyRespawnState());
//	}
}