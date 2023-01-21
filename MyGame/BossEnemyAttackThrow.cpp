#include "BossEnemyAttackThrow.h"

#include "BossEnemy.h"
#include "BossEnemyFollow.h"
#include"ThrowRockAttack.h"
void BossEnemyAttackThrow::Initialize(Enemy* enemy)
{
	
}

void BossEnemyAttackThrow::Update(Enemy* enemy)
{

	enemy->SetAnimation(BossEnemy::NowAttackMotion::THROW, false,1.f);
	if(ThrowRockAttack::GetInstance()->GetPhase()==ThrowRockAttack::END)
	{
		enemy->ChangeState_Boss(new BossEnemyFollow());
	}
}

