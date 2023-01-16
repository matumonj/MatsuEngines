#include "GuardianFollowState.h"
#include"GuardianNAttackState.h"
#include "PlayerControl.h"
#include"GuardianBomState.h"
#include"GuardianDeathState.h"

#include"GuardianRushState.h"
#include"GuardianBomAttack.h"
#include"GuardianShotAttack.h"
#include"GuardianNAttack.h"
#include"mHelper.h"
#include"GuardianFalterState.h"
#define PI 3.14f

void GuardianFollowState::Initialize(Enemy* enemy)
{
}

void GuardianFollowState::Update(Enemy* enemy)
{
	GuardianBomAttack::GetIns()->SetActionNon();
	Follow(enemy);
}

void GuardianFollowState::Follow(Enemy* enemy)
{
	enemy->Move();
	//敵がプエレイヤーの方向く処理
	XMVECTOR positionA = {
		PlayerControl::GetInstance()->GetPlayer()->GetPosition().x,
		PlayerControl::GetInstance()->GetPlayer()->GetPosition().y,
		PlayerControl::GetInstance()->GetPlayer()->GetPosition().z
	};
	XMVECTOR positionB = {enemy->GetPosition().x, enemy->GetPosition().y, enemy->GetPosition().z};
	//プレイヤーと敵のベクトルの長さ(差)を求める
	XMVECTOR SubVector = XMVectorSubtract(positionB, positionA); // positionA - positionB;

	//角度の取得 プレイヤーが敵の索敵位置に入ったら向きをプレイヤーの方に
	RotY = atan2f(SubVector.m128_f32[0], SubVector.m128_f32[2]);
	//移動ベクトルをy軸周りの角度で回転
	XMVECTOR move = {0.f, 0.f, 0.1f, 0};

	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(enemy->GetRotation().y + enemy->GetRotRadians()));

	move = XMVector3TransformNormal(move, matRot);
	//	if (enemy->GetFbxTime()< 510.000f / 60.000f)
	XMFLOAT3 rot = enemy->GetRotation();
	enemy->SetRotation({
		rot.x += 1.5f,
		RotY * 60.f + 180.f,
		rot.z
	});

	PosYMovingT++;
	bool dis = Collision::GetLength({positionA.m128_f32[0], positionA.m128_f32[1], positionA.m128_f32[2]},
	                                enemy->GetPosition()) > 10.f;

	if (dis && !enemy->GetMoveStop() && PlayerControl::GetInstance()->GetPlayer()->GetStopFlag() == false)
	{
		enemy->SetPosition({
				enemy->GetPosition().x + move.m128_f32[0] * 2.f,
				enemy->GetPosition().y,
				enemy->GetPosition().z + move.m128_f32[2] * 2.f
			}
		);
	}

	if (Percent::GetParcent(static_cast<float>(enemy->GetMaxHP()), static_cast<float>(enemy->GetHP())) < 70.f)
	{
		if (GuardianShotAttack::GetIns()->GetisEndAttack() == false)
		{
			enemy->ChangeState_Guardian(new GuardianRushState());
		}
	}
	else
	{
		GuardianShotAttack::GetIns()->SetisEndAttack(false);
	}
	//
	if (enemy->GetRecvDamage2())
	{
		Evaprobability = rand() % 100 + 1;
		if (Evaprobability <= 30) {
			enemy->ChangeState_Guardian(new GuardianFalterState());
		}
		
	}
	if (Percent::GetParcent(static_cast<float>(enemy->GetMaxHP()), static_cast<float>(enemy->GetHP())) < 90.f)
	{
		if (GuardianBomAttack::GetIns()->GetisEndAttack() == false)
		{
			enemy->ChangeState_Guardian(new GuardianBomState());
		}
	}
	NormalAttackCount++;
	if (NormalAttackCount % 120 == 0)
	{
		if (GuardianNAttack::GetIns()->GetisEndAttack() == false)
		{
			enemy->ChangeState_Guardian(new GuardianNAttackState());
		}
	}
	else
	{
		GuardianNAttack::GetIns()->SetActionNon();
		GuardianNAttack::GetIns()->SetisEndAttack(false);
	}
	if (enemy->GetHP() <= 0)
	{
		enemy->ChangeState_Guardian(new GuardianDeathState());
	}
}
