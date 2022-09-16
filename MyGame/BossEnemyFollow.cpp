#include "BossEnemyFollow.h"
#include "EnemyFollowState.h"
#include"PlayerControl.h"
#include"CameraControl.h"
#include"BossEnemyAttack.h"
#include"BossEnemyAttackCircle.h"
#include"BossEnemyAttackknock.h"
#include"BossEnemyAttackhalf.h"
#include"mHelper.h"
#include"Input.h"
#include"HalfAttack.h"
#include"CircleAttack.h"
#include"BossEnemyDeath.h"
#include"KnockAttack.h"
void BossEnemyFollow::Initialize(Enemy* enmey)
{

}

void BossEnemyFollow::Update(Enemy* enemy)
{
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
	XMVECTOR move = { 0,0,0.1f,0 };

	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(enemy->GetRotation().y));

	move = XMVector3TransformNormal(move, matRot);

	enemy->SetRotation({ enemy->GetRotation().x,
		 RotY * 60 + 180,
		enemy->GetRotation().z });
	if (Collision::GetLength(enemy->GetPosition(), PlayerControl::GetInstance()->GetPlayer()->GetPosition()) > 10) {
		enemy->SetPosition({
					enemy->GetPosition().x + move.m128_f32[0] ,
					enemy->GetPosition().y,
					enemy->GetPosition().z + move.m128_f32[2] }
		);
	}

	if ( Collision::GetLength(enemy->GetPosition(), PlayerControl::GetInstance()->GetPlayer()->GetPosition()) < 20) {
		if (enemy->GetCoolTime() == 0) {
			enemy->ChangeState_Boss(new BossEnemyAttack());
		}
	}

	AttackSelect(enemy, Percent::GetParcent(enemy->GetMaxHP(), enemy->GetHP()) <= 60.0f, enemy->HALF_1);
	AttackSelect(enemy, Percent::GetParcent(enemy->GetMaxHP(), enemy->GetHP()) <= 40.0f, enemy->CIRCLE_1);
	AttackSelect(enemy, Percent::GetParcent(enemy->GetMaxHP(), enemy->GetHP()) <= 20.0f, enemy->HALF_2);
	AttackSelect(enemy, Percent::GetParcent(enemy->GetMaxHP(), enemy->GetHP()) <= 50.0f, enemy->KNOCK);
	if (enemy->GetHP() <= 0.0f) {
		enemy->ChangeState_Boss(new BossEnemyDeath());
	}
}

void BossEnemyFollow::AttackSelect(Enemy* enemy, bool judg, int num)
{
	if (judg) {
		AttackStart(enemy, num);
		if (enemy->GetAttack_End(num) == false) {
			enemy->SetAttack_Start(num, true);
		} else {
			enemy->SetAttack_Start(num, false);
		}
		if (enemy->GetAttack_Start(num)) {
			AttackType(enemy, num);
		}
	}
}

void BossEnemyFollow::AttackStart(Enemy* enemy, int num)
{
	switch (num)
	{
	case enemy->CIRCLE_1:
		CircleAttack::GetInstance()->SetAttackFase(true);
		break;
	case enemy->CIRCLE_2:
		CircleAttack::GetInstance()->SetAttackFase(true);
		break;
	case enemy->KNOCK:
		KnockAttack::GetInstance()->SetAttackFase(true);
		break;
	case enemy->HALF_1:
		HalfAttack::GetInstance()->SetAttackFase(true);
		break;
	case enemy->HALF_2:
		HalfAttack::GetInstance()->SetAttackFase(true);
		break;
	default:
		break;
	}
}

void BossEnemyFollow::AttackType(Enemy* enemy, int num)
{
	switch (num)
	{
	case enemy->CIRCLE_1:
		if (CircleAttack::GetInstance()->GetFaseEnd() != CircleAttack::FASEFOUR) {
			enemy->ChangeState_Boss(new BossEnemyAttackCircle());
		}
		break;
	case enemy->CIRCLE_2:
		if (CircleAttack::GetInstance()->GetFaseEnd() != CircleAttack::FASEFOUR) {
			enemy->ChangeState_Boss(new BossEnemyAttackCircle());
		}
		break;
	case enemy->KNOCK:
		if (KnockAttack::GetInstance()->GetFase() != KnockAttack::FASETHREE) {
			enemy->ChangeState_Boss(new BossEnemyAttackKnock());
		}
		break;
	case enemy->HALF_1:
		if (HalfAttack::GetInstance()->GetFaseEnd() != HalfAttack::FASEFOUR) {
			enemy->ChangeState_Boss(new BossEnemyAttackHalf());
		}
		break;

	case enemy->HALF_2:
		if (HalfAttack::GetInstance()->GetFaseEnd() != HalfAttack::FASEFOUR) {
			enemy->ChangeState_Boss(new BossEnemyAttackHalf());
		}
		break;
	default:
		break;
	}

}