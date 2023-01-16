#include "BossEnemyDeath.h"
#include"BossEnemyFollow.h"
#include"BossEnemy.h"
void BossEnemyDeath::Initialize(Enemy* enmey)
{
}

void BossEnemyDeath::Update(Enemy* enemy)
{
	enemy->SetAnimation(BossEnemy::NowAttackMotion::BDEATH, 1.f,false);
	enemy->Death();
	//enemy->ChangeState_Boss(new BossEnemyFollow());
}
