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
	//���X�|�[��
	enemy->Respawn();
	//��Ԃ�ҋ@��Ԃ�
	enemy->ChangeState_Mob(new EnemyFollowState());
	
}
