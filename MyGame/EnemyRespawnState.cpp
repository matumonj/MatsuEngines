#include "EnemyRespawnState.h"
#include"EnemyStayState.h"
void EnemyRespawnState::Initialize(Enemy* enmey)
{
	
}

void EnemyRespawnState::Update(Enemy* enemy)
{
	//���X�|�[��
	enemy->isRespawn();
	//��Ԃ�ҋ@��Ԃ�
	enemy->ChangeState_Mob(new EnemyStayState());
}