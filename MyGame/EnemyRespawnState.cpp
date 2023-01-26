#include "EnemyRespawnState.h"

#include "EnemyFollowState.h"
#include"EnemyStayState.h"
#include "PlayerControl.h"

void EnemyRespawnState::Initialize(Enemy* enmey)
{
}

void EnemyRespawnState::Update(Enemy* enemy)
{
	enemy->SetRecvDamage2(false);
	//リスポーン
	enemy->Respawn();
	//状態を待機状態に
	//if (enemy->RespawnJudg() == true) {
		enemy->ChangeState_Mob(new EnemyFollowState());
	//}
}
