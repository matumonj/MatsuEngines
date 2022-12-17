#include "GuardianAppearState.h"
#include"GuardianFollowState.h"
#include"Task.h"
void GuardianAppearState::Initialize(Enemy* enemy)
{
	
}

void GuardianAppearState::Update(Enemy* enemy)
{
	DirectX::XMFLOAT3 epos = enemy->GetPosition();
	if (Task::GetInstance()->TaskThirdClear() == false)return;

	enemy->SetPosition({ epos.x,epos.y += 0.5f,epos.z });

	if(enemy->GetPosition().y>-20)
	{
		enemy->ChangeState_Guardian(new GuardianFollowState());
	}
}
