#include "EnemyWalkState.h"
#include"EnemyAttackState.h"
#include"EnemyStayState.h"
#include"EnemyFollowState.h"
#include"Collision.h"
#include"PlayerControl.h"
#include"EnemyDeathState.h"

void EnemyWalkState::Initialize(Enemy* enmey)
{
}

void EnemyWalkState::do_work(Enemy* enemy)
{
	float RandMove = static_cast<float>(rand() % 90 + 20);
	float RandMovement = static_cast<float>(rand() % 180 + 120);
	//õ“G”ÍˆÍ
	const float DetectionRange = 50.0f;

	//ƒvƒŒƒCƒ„[‚ªõ“G”ÍˆÍ“ü‚Á‚½‚ç
	if (PlayerControl::GetInstance()->GetPlayer() != nullptr)
	{
		SearchPlayer = Collision::GetLength(enemy->GetPosition(),
		                                    PlayerControl::GetInstance()->GetPlayer()->GetPosition()) < DetectionRange;
	}
	//ˆÚ“®ƒxƒNƒgƒ‹‚ðyŽ²Žü‚è‚ÌŠp“x‚Å‰ñ“]
	XMVECTOR move = {0, 0, 0.1f, 0};

	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(enemy->GetRotation().y + enemy->GetRotRadians()));

	move = XMVector3TransformNormal(move, matRot);

	//Œü‚¢‚Ä‚é•ûŒü‚ÉˆÚ“®
	//•à‚¢‚Ä‚¢‚é
	if (enemy->GetonFlag() == true)
	{
		tempx = enemy->GetPosition().x;
		tempz = enemy->GetPosition().z;
		if (!enemy->GetMoveStop()&&PlayerControl::GetInstance()->GetPlayer()->GetStopFlag() == false)
		{
			enemy->SetPosition({
					enemy->GetPosition().x + move.m128_f32[0],
					enemy->GetPosition().y,
					enemy->GetPosition().z + move.m128_f32[2]
				}
			);
			movement++;
		}

		if (movement > RandMovement)
		{
			enemy->ChangeState_Mob(new EnemyStayState());
			movement = 0;
		}
	}
}

void EnemyWalkState::Update(Enemy* enemy)
{
	do_work(enemy);

	BacktoGround(enemy);
	//enemy->SetAnimeState(enemy->WALK);
	//UŒ‚
	if (SearchPlayer)
	{
		//’ÇÕ
		enemy->ChangeState_Mob(new EnemyFollowState());
	}
	if (enemy->GetHP() <= 0.0f)
	{
		enemy->ChangeState_Mob(new EnemyDeathState());
	}
}

void EnemyWalkState::BacktoGround(Enemy* enemy)
{
	
}
