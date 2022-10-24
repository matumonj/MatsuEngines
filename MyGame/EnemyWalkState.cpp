#include "EnemyWalkState.h"
#include"EnemyAttackState.h"
#include"EnemyStayState.h"
#include"EnemyFollowState.h"
#include"Collision.h"
#include"PlayerControl.h"
void EnemyWalkState::Initialize(Enemy* enmey)
{

}
void EnemyWalkState::Update(Enemy* enemy)
{

	float RandMove = rand() % 90 + 20;
	float RandMovement = rand() % 180 + 120;
	//索敵範囲
	const float DetectionRange = 30.0f;

	//プレイヤーが索敵範囲入ったら
	if (PlayerControl::GetInstance()->GetPlayer() != nullptr) {
		SearchPlayer = Collision::GetLength(enemy->GetPosition(), PlayerControl::GetInstance()->GetPlayer()->GetPosition()) < DetectionRange;
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
		if (enemy->GetMoveFlag()&&!enemy->GetMoveStop()) {
			enemy->SetPosition({
				enemy->GetPosition().x + move.m128_f32[0],
				enemy->GetPosition().y,
				enemy->GetPosition().z + move.m128_f32[2] }
			);
			movement++;
		}
	
		if (movement > RandMovement) {
			enemy->ChangeState_Mob(new EnemyStayState());
			movement = 0;
		}
	}
	
	BacktoGround(enemy);
	//攻撃
	if (SearchPlayer) {
		//追跡
		enemy->ChangeState_Mob(new EnemyFollowState());
}
}

void EnemyWalkState::BacktoGround(Enemy*enemy)
{
	//接地状態でなく且つ動いているとき
	if (enemy->GetonFlag())return;
	if (!enemy->GetMoveFlag()) return;

	enemy->SetPosition({ tempx,enemy->GetPosition().y,tempz });
}