#include "EnemyAttackState.h"

#include "EnemyDeathState.h"
#include"EnemyStayState.h"
#include"PlayerControl.h"

void EnemyAttackState::Initialize(Enemy* enmey)
{
}

void EnemyAttackState::Update(Enemy* enemy)
{
	const int Damage = 10;
	//�v���C���[�Ƀ_���[�W
	//	PlayerControl::GetIns()->GetPlayer()->RecvDamage(Damage);
	//fbx���U���A�j���[�V������
	enemy->SetAnimeState(enemy->ATTACK1);
	enemy->SetAttackTime(true);
	if (enemy->GetHP() <= 0.0f)
	{
		enemy->ChangeState_Mob(new EnemyDeathState());
	}
	//�A�j���[�V�����Đ��I�������ҋ@�i���̓~�j�S�[�����̂݁j
	//�����A�j���[�V�����Ή�����؂�ւ��Ă�
	if (enemy->GetEnemyNumber() == enemy->MINIGOLEM)
	{
		if (enemy->GetAnimationTime() >= enemy->GetFbxTimeEnd() - 0.3f)
		{
			enemy->ChangeState_Mob(new EnemyStayState());
		}
	}
	else
	{
		enemy->ChangeState_Mob(new EnemyStayState());
	}
}
