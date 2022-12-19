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
	DirectX::XMFLOAT4 color = {1.f, 1.f, 1.f, 1.f};

	color.x = 0.2f;

	color.y = 0.2f;
	color.z = 0.2f;
	enemy->SetColor({color.x, color.y, color.z, 1});

	enemy->SetisAlive(FALSE);

	if (pos.y < -32.0f)
	{
		
	}
	else
	{
		pos.y -= 0.04f;
	}
	enemy->SetPosition(pos);
}
