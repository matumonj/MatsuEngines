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

	//アニメーション再生終わったら待機（今はミニゴーレムのみ）
	//複数アニメーション対応次第切り替えてく
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
