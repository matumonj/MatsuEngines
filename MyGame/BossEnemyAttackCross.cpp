#include "BossEnemyAttackCross.h"

#include <algorithm>

#include "BossEnemyFollow.h"
#include "EnemyFollowState.h"
#include"PlayerControl.h"
#include"mHelper.h"
#include"RushAttack.h"
#include"BossEnemy.h"
#include "LineCrossAttack.h"

void BossEnemyAttackCross::Initialize(Enemy* enmey)
{
}

void BossEnemyAttackCross::Update(Enemy* enemy)
{
	//モーションセット
	if (LineCrossAttack::GetIns()->GetPhase() == LineCrossAttack::PHASE_ONE)
	{
		AnimationSpeed = 0.2;
	}

	AnimationSpeed += 0.04;

	AnimationSpeed = std::clamp(AnimationSpeed, 0.0, 0.8);
	enemy->SetAnimation(BossEnemy::NowAttackMotion::SWING, true, AnimationSpeed);

	//追跡処理部分//////////

	//敵がプエレイヤーの方向く処理
	XMVECTOR positionA = {
		PlayerControl::GetIns()->GetPlayer()->GetPosition().x,
		PlayerControl::GetIns()->GetPlayer()->GetPosition().y,
		PlayerControl::GetIns()->GetPlayer()->GetPosition().z
	};
	XMVECTOR positionB = {enemy->GetPosition().x, enemy->GetPosition().y, enemy->GetPosition().z};

	//向きをプレイヤーに
	float Add_RotVal = FollowRot::FollowA_B(positionA, positionB);
	if (enemy->GetAnimationTime() <= 0.6)
	{
		enemy->SetRotation({
			enemy->GetRotation().x,
			Add_RotVal * 60.0f,
			enemy->GetRotation().z
		});
	}

	enemy->SetRecvDamage2(false);

	LineCrossAttack::GetIns()->SetAttackPhase(false);

	double EndTime = enemy->GetFbxTimeEnd();

	if (LineCrossAttack::GetIns()->GetPhase() == LineCrossAttack::PHASE_THREE)
	{
		enemy->ChangeState_Boss(new BossEnemyFollow());
	}
}
