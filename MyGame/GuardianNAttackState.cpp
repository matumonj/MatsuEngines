#include "GuardianNAttackState.h"
#include"GuardianBomAttack.h"
#include"GuardianFollowState.h"
#include "GuardianNAttack.h"

void GuardianNAttackState::Initialize(Enemy* enemy)
{
}

void GuardianNAttackState::Update(Enemy* enemy)
{
	PosYMovingT++;
	GuardianNAttack::GetIns()->SetAction(true);
	enemy->SetPosition({
			enemy->GetPosition().x,
			-20.f + sinf(3.14f * 2.f / 120.f * PosYMovingT) * 2.f,
			enemy->GetPosition().z
		}
	);
	if (GuardianNAttack::GetIns()->GetPhaseEnd())
	{
		GuardianNAttack::GetIns()->SetisEndAttack(TRUE);
		enemy->ChangeState_Guardian(new GuardianFollowState());
	}
}
