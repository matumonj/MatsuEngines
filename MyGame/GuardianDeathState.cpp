#include "GuardianDeathState.h"


void GuardianDeathState::Initialize(Enemy* enemy)
{
}

void GuardianDeathState::Update(Enemy* enemy)
{
	Death(enemy);
}

void GuardianDeathState::Death(Enemy* enemy)
{
	DirectX::XMFLOAT3 pos = enemy->GetPosition();


	if (pos.y < -32.0f)
	{
		enemy->Death();
	}
	else
	{
		pos.y -= 0.04f;
	}
	enemy->SetPosition(pos);
}
