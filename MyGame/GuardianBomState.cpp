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

	if (GuardianBomAttack::GetIns()->GetPhaseEnd())
	{
		DownMove(enemy);
	}
	else
	{
		UpMove(enemy);
	}
}

void GuardianBomState::UpMove(Enemy* enemy)
{
	DirectX::XMFLOAT3 epos = enemy->GetPosition();
	DirectX::XMFLOAT3 erot = enemy->GetRotation();

	if (epos.y > 0.f)
	{
		return;
	}
	enemy->SetPosition({epos.x, epos.y += EnemyPosYCorrVal, epos.z});
	enemy->SetRotation({erot.x, erot.y += EnemyPosYCorrVal, erot.z});
}

void GuardianBomState::DownMove(Enemy* enemy)
{
	DirectX::XMFLOAT3 epos = enemy->GetPosition();

	constexpr float l_YposMin = -24.f;

	enemy->SetPosition({epos.x, epos.y -= EnemyPosYCorrVal, epos.z});

	if (epos.y < l_YposMin)
	{
		GuardianBomAttack::GetIns()->SetisEndAttack(TRUE);
		enemy->ChangeState_Guardian(new GuardianFollowState());
	}
}
