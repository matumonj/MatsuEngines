#include "EnemyWalkState.h"

#include "AttackCollision.h"
#include"EnemyAttackState.h"
#include"EnemyStayState.h"
#include"EnemyFollowState.h"
#include"Collision.h"
#include"PlayerControl.h"
#include"EnemyDeathState.h"

#include"EnemyKnockState.h"
#include "SceneManager.h"

void EnemyWalkState::Initialize(Enemy* enmey)
{
}

void EnemyWalkState::do_work(Enemy* enemy)
{
	float RandMove = static_cast<float>(rand() % 90 + 20);
	float RandMovement = static_cast<float>(rand() % 180 + 120);
	//索敵範囲
	const float DetectionRange = 50.0f;

	//プレイヤーが索敵範囲入ったら
	if (PlayerControl::GetIns()->GetPlayer() != nullptr)
	{
		SearchPlayer = Collision::GetLength(enemy->GetPosition(),
		                                    PlayerControl::GetIns()->GetPlayer()->GetPosition()) < DetectionRange;
	}
	//移動ベクトルをy軸周りの角度で回転
	XMVECTOR move = {0, 0, 0.1f, 0};

	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(enemy->GetRotation().y + enemy->GetRotRadians()));

	move = XMVector3TransformNormal(move, matRot);

	//向いてる方向に移動
	//歩いている

	if (enemy->GetRecvDamage2())
	{
		enemy->ChangeState_Mob(new EnemyKnockState());
	}
	if (enemy->GetonFlag() == true)
	{
		tempx = enemy->GetPosition().x;
		tempz = enemy->GetPosition().z;
		if (!enemy->GetMoveStop() && PlayerControl::GetIns()->GetPlayer()->GetStopFlag() == false)
		{
			enemy->SetPosition({
					enemy->GetPosition().x + move.m128_f32[0],
					enemy->GetPosition().y,
					enemy->GetPosition().z + move.m128_f32[2]
				}
			);
			movement++;
		}

		if (movement > RandMovement)
		{
			enemy->ChangeState_Mob(new EnemyStayState());
			movement = 0;
		}
	}
}

void EnemyWalkState::Update(Enemy* enemy)
{
	if (SceneManager::GetIns()->GetScene() == SceneManager::MAPCREATE)
	{
		return;
	}
	do_work(enemy);

	BacktoGround(enemy);
	enemy->SetAnimeState(enemy->WALK);

	bool SearchHelp = Collision::GetLength(enemy->GetPosition(),
	                                       PlayerControl::GetIns()->GetPlayer()->GetPosition()) < 150.f &&
		std::get<0>(AttackCollision::GetIns()->GetHelpJudg());
	//攻撃
	if (SearchPlayer)
	{
		enemy->ChangeState_Mob(new EnemyFollowState());
		//追跡
	}
	if (SearchHelp && enemy->GetEnemyNumber() == std::get<1>(AttackCollision::GetIns()->GetHelpJudg()))
	{
		if (Collision::GetLength(enemy->GetPosition(),
		                         PlayerControl::GetIns()->GetPlayer()->GetPosition()) > 40.f)
		{
			enemy->SetHelpIcon(true);

			enemy->ChangeState_Mob(new EnemyFollowState());
		}
	}
	if (enemy->GetHP() <= 0.0f)
	{
		enemy->ChangeState_Mob(new EnemyDeathState());
	}
}

void EnemyWalkState::BacktoGround(Enemy* enemy)
{
}
