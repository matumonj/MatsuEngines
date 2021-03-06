#include "EnemyWalkState.h"
#include"EnemyAttackState.h"
#include"EnemyStayState.h"
#include"EnemyFollowState.h"
#include"Collision.h"
void EnemyWalkState::Initialize(Enemy* enmey)
{

}
void EnemyWalkState::Update(Enemy* enemy)
{
	
	//索敵範囲
	const float DetectionRange = 10.0f;
	//プレイヤーが索敵範囲入ったら
	bool SearchPlayer = Collision::GetLength(enemy->GetPosition(), Player::GetInstance()->GetPosition()) < DetectionRange;
	if (Player::GetInstance() != nullptr) {
		SearchPlayer = false;
	}
	//移動ベクトルをy軸周りの角度で回転
	XMVECTOR move = { 0,0,0.1f,0 };
	
	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(enemy->GetRotation().y));
	
	move = XMVector3TransformNormal(move, matRot);

	//向いてる方向に移動
	//歩いている
	if (enemy->GetonFlag()==true) {
		tempx = enemy->GetPosition().x;
		tempz = enemy->GetPosition().z;
		if (enemy->GetMoveFlag()) {
			enemy->SetPosition({
				enemy->GetPosition().x + move.m128_f32[0],
				enemy->GetPosition().y,
				enemy->GetPosition().z + move.m128_f32[2] }
			);
			movement++;
		}
	
		if (movement > enemy->GetRandMoveMent()) {
			//enemy->ChangeState(new EnemyStayState());
			movement = 0;
		}
	}
	else if (enemy->GetonFlag() == false) {
		if (enemy->GetMoveFlag() != false) {
			enemy->SetPosition({ tempx,enemy->GetPosition().y,tempz });
		}
		else {
			enemy->SetPosition({
							enemy->GetPosition().x ,
							enemy->GetPosition().y,
							enemy->GetPosition().z }
			);

		}
	}
	//攻撃
	if (SearchPlayer) {
		//追跡
		
			//enemy->ChangeState(new EnemyFollowState());
	}
}