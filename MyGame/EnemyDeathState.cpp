#include "EnemyDeathState.h"
#include"EnemyFollowState.h"
void EnemyDeathState::Initialize(Enemy* enmey)
{

}

void EnemyDeathState::Update(Enemy* enemy)
{
	enemy->Death();
	//enemy->ChangeState_Mob(new EnemyFollowState());
	
}