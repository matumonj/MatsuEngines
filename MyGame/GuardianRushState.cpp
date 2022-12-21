#include "GuardianRushState.h"

#include "GuardianFollowState.h"
#include "GuardianShotAttack.h"

void GuardianRushState::Initialize(Enemy* enemy)
{
}

void GuardianRushState::Update(Enemy* enemy)
{

	GuardianShotAttack::GetIns()->SetAction(true);
	enemy->ChangeState_Guardian(new GuardianFollowState());
	
}