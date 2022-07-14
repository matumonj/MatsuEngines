#include "EnemyFollowState.h"

void EnemyFollowState::Initialize(Enemy* enmey)
{

}

void EnemyFollowState::Update(Enemy* enemy)
{
	float angleX, angleZ,dis;
	//追跡スピード
	float centerSpeed = 0.01f;
	
	angleX = (Player::GetInstance()->GetPosition().x - enemy->GetPosition().x);
	angleZ = (Player::GetInstance()->GetPosition().z - enemy->GetPosition().z);

	//敵とプレイヤーの距離求め
	dis = sqrtf((enemy->GetPosition().x - Player::GetInstance()->GetPosition().x) * (enemy->GetPosition().x - Player::GetInstance()->GetPosition().x)
		+ (enemy->GetPosition().z - Player::GetInstance()->GetPosition().z) * (enemy->GetPosition().z - Player::GetInstance()->GetPosition().z));
	
	
	//敵がプエレイヤーの方向く処理
	XMVECTOR positionA = { Player::GetInstance()->GetPosition().x,Player::GetInstance()->GetPosition().y, Player::GetInstance()->GetPosition().z };
	XMVECTOR positionB = { enemy->GetPosition().x,enemy->GetPosition().y,enemy->GetPosition().z };
	//プレイヤーと敵のベクトルの長さ(差)を求める
	XMVECTOR SubVector = DirectX::XMVectorSubtract(positionB, positionA);// positionA - positionB;

	//角度の取得 プレイヤーが敵の索敵位置に入ったら向きをプレイヤーの方に
	RotY = atan2f(SubVector.m128_f32[0], SubVector.m128_f32[2]);

	enemy->SetRotation({ enemy->GetRotation().x,RotY * 60 + 180,enemy->GetRotation().z });
	//座標のセット
	enemy->SetPosition({ enemy->GetPosition().x + (angleX / dis) * centerSpeed,enemy->GetPosition().y,enemy->GetPosition().z + (angleZ / dis) * centerSpeed });
	
}