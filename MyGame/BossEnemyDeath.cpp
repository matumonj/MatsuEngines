#include "BossEnemyDeath.h"
#include"BossEnemyFollow.h"
#include"BossEnemy.h"
void BossEnemyDeath::Initialize(Enemy* enmey)
{
}

void BossEnemyDeath::Update(Enemy* enemy)
{
	enemy->SetAnimation(BossEnemy::NowAttackMotion::BDEATH,false,0.5);
	enemy->Death();
}
