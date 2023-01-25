#include "GuardianAppearState.h"
#include"GuardianFollowState.h"
#include "PlayerControl.h"
#include"Task.h"

void GuardianAppearState::Initialize(Enemy* enemy)
{
}

void GuardianAppearState::Update(Enemy* enemy)
{
	DirectX::XMFLOAT3 epos = enemy->GetPosition();
	if (Task::GetInstance()->TaskFourClear()== false)
	{
		return;
	}
	if (UpF && !DownF)
	{
		PlayerControl::GetInstance()->GetPlayer()->SetStopFlag(true);
		if (enemy->GetPosition().y > 10.f)
		{
			DownF = true;
		}
		enemy->SetPosition({epos.x, epos.y += 0.5f, epos.z});
	}
	if (DownF)
	{
		DownMove(enemy);
	}
	if (epos.y < -30.f)
	{
		EaseTime = 0.f;
		UpF = true;
		DownF = false;
	}
}

#include"mHelper.h"

void GuardianAppearState::DownMove(Enemy* enemy)
{
	EaseTime += 0.01f;
	DirectX::XMFLOAT3 epos = enemy->GetPosition();

	if (EaseTime >= 1.f)
	{
		PlayerControl::GetInstance()->GetPlayer()->SetStopFlag(false);
		SmokeF = true;
		enemy->Smoke(SmokeF);
		enemy->SetisAlive(TRUE);
		enemy->ChangeState_Guardian(new GuardianFollowState());
	}
	else
	{
		PlayerControl::GetInstance()->GetPlayer()->SetStopFlag(true);
	}
	enemy->SetPosition({epos.x, Easing::EaseOut(EaseTime, 30.f, -20.f), epos.z});
}