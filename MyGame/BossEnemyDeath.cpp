#include "BossEnemyDeath.h"
#include"BossEnemyFollow.h"
#include"BossEnemy.h"
#include "PlayerAttackState.h"

void BossEnemyDeath::Initialize(Enemy* enmey)
{
}

void BossEnemyDeath::Update(Enemy* enemy)
{
	enemy->SetAnimation(BossEnemy::NowAttackMotion::BDEATH,false,0.5);
	PlayerAttackState::GetIns()->SetHitStopJudg(true, 120);
	enemy->Death();

}
