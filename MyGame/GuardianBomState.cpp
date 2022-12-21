#include "GuardianBomState.h"
#include"GuardianBomAttack.h"
#include"GuardianFollowState.h"

void GuardianBomState::Initialize(Enemy* enemy)
{
}

void GuardianBomState::Update(Enemy* enemy)
{
	PosYMovingT++;
	GuardianBomAttack::GetIns()->SetAction(true);
	enemy->SetPosition({
			enemy->GetPosition().x,
			-20.f + sinf(3.14f * 2.f / 120.f * PosYMovingT) * 2.f,
			enemy->GetPosition().z
		}
	);
	if (GuardianBomAttack::GetIns()->GetPhaseEnd())
	{
		GuardianBomAttack::GetIns()->SetisEndAttack(TRUE);
		enemy->ChangeState_Guardian(new GuardianFollowState());
	}
}
