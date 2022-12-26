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
	
	if (GuardianNAttack::GetIns()->GetPhaseEnd())
	{
		GuardianNAttack::GetIns()->SetisEndAttack(TRUE);
		enemy->ChangeState_Guardian(new GuardianFollowState());
	}
}
