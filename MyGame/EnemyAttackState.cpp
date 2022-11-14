#include "EnemyAttackState.h"
#include"EnemyStayState.h"
#include"PlayerControl.h"

void EnemyAttackState::Initialize(Enemy* enmey)
{
}

void EnemyAttackState::Update(Enemy* enemy)
{
	const int Damage = 10;
	//プレイヤーにダメージ
	//	PlayerControl::GetInstance()->GetPlayer()->RecvDamage(Damage);
	//fbxを攻撃アニメーションに
	enemy->SetAnimeState(enemy->ATTACK1);
	enemy->SetAttackTime(true);

	enemy->ChangeState_Mob(new EnemyStayState());
}
