#include "EnemyRespawnState.h"
#include"EnemyStayState.h"
void EnemyRespawnState::Initialize(Enemy* enmey)
{
	
}

void EnemyRespawnState::Update(Enemy* enemy)
{
	//リスポーン
	enemy->isRespawn();
	//状態を待機状態に
	enemy->ChangeState_Mob(new EnemyStayState());
}