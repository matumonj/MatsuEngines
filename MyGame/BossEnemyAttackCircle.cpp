#include "BossEnemyAttackCircle.h"
#include"BossEnemyFollow.h"
#include"FrontCircleAttack.h"
#include"BossEnemy.h"
#include "mHelper.h"
#include "PlayerControl.h"

void BossEnemyAttackCircle::Initialize(Enemy* enmey)
{
}

void BossEnemyAttackCircle::Update(Enemy* enemy)
{
	enemy->SetAnimation(BossEnemy::NowAttackMotion::BROAR, false, 1.f);
	enemy->SetRecvDamage2(false);
	PlayerControl::GetIns()->GetPlayer()->KnockBack(enemy->GetPosition());

	if (enemy->GetAnimationTime() >= enemy->GetFbxTimeEnd() - 0.1f)
	{
		enemy->SetRecvDamage2(false);
		enemy->ChangeState_Boss(new BossEnemyFollow());
	}
}
