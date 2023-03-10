#include "PlayerControl.h"
#include "BossEnemyShieldGuard.h"
#include "AttackEffect.h"
#include "BossEnemy.h"
#include "BossEnemyEvasion.h"
#include "BossEnemyFollow.h"
#include "mHelper.h"
#include "UI.h"

using namespace DirectX;

void BossEnemyShieldGuard::Initialize(Enemy* enemy)
{
}

void BossEnemyShieldGuard::Update(Enemy* enemy)
{
	//Rotation補正値
	constexpr float l_RotCorreVal = 40.f;

	//敵がプエレイヤーの方向く処理
	XMVECTOR positionA = {
		PlayerControl::GetIns()->GetPlayer()->GetPosition().x,
		PlayerControl::GetIns()->GetPlayer()->GetPosition().y,
		PlayerControl::GetIns()->GetPlayer()->GetPosition().z
	};
	XMVECTOR positionB = {enemy->GetPosition().x, enemy->GetPosition().y, enemy->GetPosition().z};

	//向きをプレイヤーに
	float Add_RotVal = FollowRot::FollowA_B(positionA, positionB) * 60.f;

	enemy->SetRotation({
		enemy->GetRotation().x,
		Add_RotVal + l_RotCorreVal,
		enemy->GetRotation().z
	});

	const float FollowSpeed = 2.f;

	//移動ベクトルをy軸周りの角度で回転
	XMVECTOR move = {0.0f, 0.0f, 0.1f, 0.0f};

	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(enemy->GetRotation().y + 20));

	move = XMVector3TransformNormal(move, matRot);


	timer++;

	enemy->SetAnimation(BossEnemy::NowAttackMotion::SHIELDWALK_SIDE, true, 1.f);

	enemy->SetGuardAction(true);
	if (Collision::GetLength(enemy->GetPosition(), PlayerControl::GetIns()->GetPlayer()->GetPosition()) > 60.f)
	{
		enemy->ChangeState_Boss(new BossEnemyFollow());
	}
	if (enemy->GetRecvDamage2())
	{
		knockpower = 4.f;
		enemy->SetGuardPointAdd();
		enemy->SetRecvDamage2(false);
	}

	if (knockpower > 0.f)
	{
		Knock(enemy);
	}
	else
	{
		enemy->SetPosition({
				enemy->GetPosition().x + move.m128_f32[0] * FollowSpeed,
				enemy->GetPosition().y,
				enemy->GetPosition().z + move.m128_f32[2] * FollowSpeed
			}
		);
	}
	if (enemy->GetGuardPoint() >= 4)
	{
		AttackEffect::GetIns()->SetParticle(
			enemy->GetPosition());
		enemy->ChangeState_Boss(new BossEnemyFalter());
	}
}

void BossEnemyShieldGuard::Knock(Enemy* enemy)
{
	if (knockpower > 0.f)
	{
		//移動ベクトルをy軸周りの角度で回転
		XMVECTOR move = {0.0f, 0.0f, 0.1f, 0.0f};

		XMMATRIX matRot = XMMatrixRotationY(
			XMConvertToRadians(enemy->GetRotation().y + enemy->GetRotRadians() + 180.0f));

		move = XMVector3TransformNormal(move, matRot);

		enemy->SetPosition({
				enemy->GetPosition().x + move.m128_f32[0] * knockpower,
				enemy->GetPosition().y,
				enemy->GetPosition().z + move.m128_f32[2] * knockpower
			}
		);
		knockpower -= 0.1f;
	}
}
