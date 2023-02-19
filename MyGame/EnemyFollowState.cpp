#include "EnemyFollowState.h"
#include"PlayerControl.h"
#include"EnemyAttackState.h"
#include"EnemyDeathState.h"
#include"EnemyKnockState.h"
#include "EnemyReturnCreatePos.h"

void EnemyFollowState::Initialize(Enemy* enmey)
{
}

void EnemyFollowState::Update(Enemy* enemy)
{
	//索敵範囲
	const float DetectionRange = 15.0f;
	//プレイヤーが索敵範囲入ったら
	bool SearchPlayer = Collision::GetLength(enemy->GetPosition(),
	                                         PlayerControl::GetIns()->GetPlayer()->GetPosition()) < DetectionRange;

	float dis;
	//追跡スピード
	float centerSpeed = 0.01f;


	//敵とプレイヤーの距離求め
	dis = sqrtf(
		(enemy->GetPosition().x - PlayerControl::GetIns()->GetPlayer()->GetPosition().x) * (enemy->GetPosition().x
			- PlayerControl::GetIns()->GetPlayer()->GetPosition().x)
		+ (enemy->GetPosition().z - PlayerControl::GetIns()->GetPlayer()->GetPosition().z) * (enemy->GetPosition().
			z - PlayerControl::GetIns()->GetPlayer()->GetPosition().z));


	//敵がプエレイヤーの方向く処理
	XMVECTOR positionA = {
		PlayerControl::GetIns()->GetPlayer()->GetPosition().x,
		PlayerControl::GetIns()->GetPlayer()->GetPosition().y,
		PlayerControl::GetIns()->GetPlayer()->GetPosition().z
	};
	XMVECTOR positionB = {enemy->GetPosition().x, enemy->GetPosition().y, enemy->GetPosition().z};
	//プレイヤーと敵のベクトルの長さ(差)を求める
	XMVECTOR SubVector = XMVectorSubtract(positionB, positionA); // positionA - positionB;

	//角度の取得 プレイヤーが敵の索敵位置に入ったら向きをプレイヤーの方に
	RotY = atan2f(SubVector.m128_f32[0], SubVector.m128_f32[2]);
	//移動ベクトルをy軸周りの角度で回転
	XMVECTOR move = {0, 0, 0.1f, 0};

	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(enemy->GetRotation().y + enemy->GetRotRadians()));

	move = XMVector3TransformNormal(move, matRot);
	//	if (enemy->GetFbxTime()< 510.000f / 60.000f) {
	enemy->SetRotation({
		enemy->GetRotation().x,
		RotY * 60 + enemy->GetRotCorrect(),
		enemy->GetRotation().z
	});

	if (!enemy->GetMoveStop() && PlayerControl::GetIns()->GetPlayer()->GetStopFlag() == false)
	{
		if (Collision::GetLength(enemy->GetPosition(), PlayerControl::GetIns()->GetPlayer()->GetPosition()) > 10)
		{
			enemy->SetPosition({
					enemy->GetPosition().x + move.m128_f32[0] * 4.f,
					enemy->GetPosition().y,
					enemy->GetPosition().z + move.m128_f32[2] * 4.f
				}
			);
			//}
		}

		enemy->SetAnimeState(enemy->WALK);
	}
	//movement++;
	if (enemy->GetRecvDamage2())
	{
		enemy->ChangeState_Mob(new EnemyKnockState());
	}
	if (SearchPlayer && enemy->GetCoolTime() == 0)
	{
		//追跡
		enemy->ChangeState_Mob(new EnemyAttackState());
	}
	if (enemy->GetHP() <= 0.0f)
	{
		enemy->ChangeState_Mob(new EnemyDeathState());
	}
	if (enemy->GetReturnCreatePos())
	{
		enemy->ChangeState_Mob(new EnemyReturnCreatePos());
	}
}
