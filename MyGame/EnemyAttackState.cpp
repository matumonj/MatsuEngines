#include "EnemyAttackState.h"
#include"EnemyStayState.h"
#include"PlayerControl.h"

void EnemyAttackState::Initialize(Enemy* enmey)
{
}

void EnemyAttackState::Update(Enemy* enemy)
{
	const int Damage = 10;
	//�v���C���[�Ƀ_���[�W
	//	PlayerControl::GetInstance()->GetPlayer()->RecvDamage(Damage);
	//fbx���U���A�j���[�V������
	enemy->SetAnimeState(enemy->ATTACK1);
	enemy->SetAttackTime(true);

	enemy->ChangeState_Mob(new EnemyStayState());
}
