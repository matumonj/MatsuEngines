#include "GuardianFalterState.h"
#include "GuardianFollowState.h"

void GuardianFalterState::Initialize(Enemy* enemy)
{
	
}

void GuardianFalterState::Update(Enemy* enemy)
{

	enemy->SetRecvDamage2(false);
	//索敵範囲
	const float DetectionRange = 10.0f;

	//移動ベクトルをy軸周りの角度で回転
	DirectX::XMVECTOR move = { 0.0f, 0.0f, 0.1f, 0.0f };

	DirectX::XMMATRIX matRot = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(enemy->GetRotation().y + enemy->GetRotRadians() + 180.0f));

	move = XMVector3TransformNormal(move, matRot);

	

	Falter(enemy);

	
}

void GuardianFalterState::Falter(Enemy*enemy)
{
	enemy->SetRecvDamage2(false);
	if (shaketime <= 0)
	{
		shaketime = 20;
	}
	if (shaketime != 0)
	{
		shake = static_cast<float>(rand() % 2-1)/5.0f;
		shakex = static_cast<float>(rand() % 2 - 1)/5.0f;
		shakey = static_cast<float>(rand() % 2 - 1)/5.0f;
		shakex -= shake;
		shakey -= shake;
		shaketime--;
		if(shaketime<=0)
		{
			enemy->ChangeState_Guardian(new GuardianFollowState());
		}
		//}
		//シェイク値を０に
	} else if (shaketime == 0)
	{
		shakex = 0.0f;
		shakey = 0.0f;
	}

	DirectX::XMFLOAT3 epos=enemy->GetPosition();

	enemy->SetPosition({ epos.x + shakex,epos.y + shakey,epos.z });
}


