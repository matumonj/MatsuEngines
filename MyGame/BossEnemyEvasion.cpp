#include "BossEnemyEvasion.h"
#include "BossEnemyEvasion.h"
#include"PlayerControl.h"
#include"CameraControl.h"
#include"BossEnemyAttack.h"
#include"BossEnemyAttackCircle.h"
#include"BossEnemyAttackknock.h"
#include"BossEnemyAttackhalf.h"
#include"mHelper.h"
#include"BossEnemyDeath.h"
#include"BossEnemyFollow.h"
void BossEnemyEvasion::Initialize(Enemy* enmey)
{

}

void BossEnemyEvasion::Update(Enemy* enemy)
{
	enemy->SetEvaMotionTime(true);
	//索敵範囲
	const float DetectionRange = 10.0f;
	//プレイヤーが索敵範囲入ったら
	bool SearchPlayer = Collision::GetLength(enemy->GetPosition(), PlayerControl::GetInstance()->GetPlayer()->GetPosition()) < DetectionRange;

	float angleX, angleZ, dis;
	//追跡スピード
	float centerSpeed = 0.01f;

	angleX = (PlayerControl::GetInstance()->GetPlayer()->GetPosition().x - enemy->GetPosition().x);
	angleZ = (PlayerControl::GetInstance()->GetPlayer()->GetPosition().z - enemy->GetPosition().z);

	//敵とプレイヤーの距離求め
	dis = sqrtf((enemy->GetPosition().x - PlayerControl::GetInstance()->GetPlayer()->GetPosition().x) * (enemy->GetPosition().x - PlayerControl::GetInstance()->GetPlayer()->GetPosition().x)
		+ (enemy->GetPosition().z - PlayerControl::GetInstance()->GetPlayer()->GetPosition().z) * (enemy->GetPosition().z - PlayerControl::GetInstance()->GetPlayer()->GetPosition().z));


	//敵がプエレイヤーの方向く処理
	XMVECTOR positionA = { PlayerControl::GetInstance()->GetPlayer()->GetPosition().x,PlayerControl::GetInstance()->GetPlayer()->GetPosition().y, PlayerControl::GetInstance()->GetPlayer()->GetPosition().z };
	XMVECTOR positionB = { enemy->GetPosition().x,enemy->GetPosition().y,enemy->GetPosition().z };
	//プレイヤーと敵のベクトルの長さ(差)を求める
	XMVECTOR SubVector = DirectX::XMVectorSubtract(positionB, positionA);// positionA - positionB;

	//角度の取得 プレイヤーが敵の索敵位置に入ったら向きをプレイヤーの方に
	RotY = atan2f(SubVector.m128_f32[0], SubVector.m128_f32[2]);
	//移動ベクトルをy軸周りの角度で回転
	XMVECTOR move = { 0.0f,0.0f,0.1f,0.0f };

	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(enemy->GetRotation().y-70));

	move = XMVector3TransformNormal(move, matRot);

	enemy->SetPosition({
					enemy->GetPosition().x + move.m128_f32[0]*3 ,
					enemy->GetPosition().y,
					enemy->GetPosition().z + move.m128_f32[2]*3 }
		);

	if (Collision::GetLength(enemy->GetPosition(), PlayerControl::GetInstance()->GetPlayer()->GetPosition()) > 30) {
		enemy->ChangeState_Boss(new BossEnemyFollow());
	}
	//////////////////////////////////////////////////////////////

}

