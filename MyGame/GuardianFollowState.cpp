#include "GuardianFollowState.h"
#include"GuardianNAttackState.h"
#include "PlayerControl.h"
#include"GuardianBomState.h"
void GuardianFollowState::Initialize(Enemy* enemy)
{
	
}

void GuardianFollowState::Update(Enemy* enemy)
{
	Follow(enemy);
}

void GuardianFollowState::Follow(Enemy* enemy)
{

	//敵がプエレイヤーの方向く処理
	DirectX::XMVECTOR positionA = {
		PlayerControl::GetInstance()->GetPlayer()->GetPosition().x,
		PlayerControl::GetInstance()->GetPlayer()->GetPosition().y,
		PlayerControl::GetInstance()->GetPlayer()->GetPosition().z
	};
	XMVECTOR positionB = { enemy->GetPosition().x, enemy->GetPosition().y, enemy->GetPosition().z };
	//プレイヤーと敵のベクトルの長さ(差)を求める
	XMVECTOR SubVector = XMVectorSubtract(positionB, positionA); // positionA - positionB;

	//角度の取得 プレイヤーが敵の索敵位置に入ったら向きをプレイヤーの方に
	RotY = atan2f(SubVector.m128_f32[0], SubVector.m128_f32[2]);
	//移動ベクトルをy軸周りの角度で回転
	XMVECTOR move = { 0, 0, 0.1f, 0 };

	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(enemy->GetRotation().y + enemy->GetRotRadians()));

	move = XMVector3TransformNormal(move, matRot);
	//	if (enemy->GetFbxTime()< 510.000f / 60.000f)
	XMFLOAT3 rot = enemy->GetRotation();
	enemy->SetRotation({
		rot.x+=1.5f,
		RotY * 60 + 180.f,
		rot.z
		});

	PosYMovingT++;
	 
	if (!enemy->GetMoveStop() && PlayerControl::GetInstance()->GetPlayer()->GetStopFlag() == false)
	{
			enemy->SetPosition({
				enemy->GetPosition().x + move.m128_f32[0] * 2,
				 -20 + sinf(3.14f * 2.f / 120.f * PosYMovingT) * 2.f,
				enemy->GetPosition().z + move.m128_f32[2] * 2
				}
			);
		}
	if (Collision::GetLength(enemy->GetPosition(), PlayerControl::GetInstance()->GetPlayer()->GetPosition()) > 10)
	{
		//enemy->ChangeState_Guardian(new GuardianBomState());
	}
}
