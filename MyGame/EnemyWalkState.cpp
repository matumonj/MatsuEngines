#include "EnemyWalkState.h"
#include"EnemyAttackState.h"
#include"EnemyStayState.h"
#include"EnemyFollowState.h"
#include"Collision.h"
void EnemyWalkState::Initialize(Enemy* enmey)
{

}
void EnemyWalkState::Update(Enemy* enemy)
{
	
	//õ“G”ÍˆÍ
	const float DetectionRange = 10.0f;
	//ƒvƒŒƒCƒ„[‚ªõ“G”ÍˆÍ“ü‚Á‚½‚ç
	bool SearchPlayer = Collision::GetLength(enemy->GetPosition(), Player::GetInstance()->GetPosition()) < DetectionRange;
	if (Player::GetInstance() != nullptr) {
		SearchPlayer = false;
	}
	//ˆÚ“®ƒxƒNƒgƒ‹‚ðyŽ²Žü‚è‚ÌŠp“x‚Å‰ñ“]
	XMVECTOR move = { 0,0,0.1f,0 };
	
	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(enemy->GetRotation().y));
	
	move = XMVector3TransformNormal(move, matRot);

	//Œü‚¢‚Ä‚é•ûŒü‚ÉˆÚ“®
	//•à‚¢‚Ä‚¢‚é
	if (enemy->GetonFlag()==true) {
		tempx = enemy->GetPosition().x;
		tempz = enemy->GetPosition().z;
		if (enemy->GetMoveFlag()) {
			enemy->SetPosition({
				enemy->GetPosition().x + move.m128_f32[0],
				enemy->GetPosition().y,
				enemy->GetPosition().z + move.m128_f32[2] }
			);
			movement++;
		}
	
		if (movement > enemy->GetRandMoveMent()) {
			//enemy->ChangeState(new EnemyStayState());
			movement = 0;
		}
	}
	else if (enemy->GetonFlag() == false) {
		if (enemy->GetMoveFlag() != false) {
			enemy->SetPosition({ tempx,enemy->GetPosition().y,tempz });
		}
		else {
			enemy->SetPosition({
							enemy->GetPosition().x ,
							enemy->GetPosition().y,
							enemy->GetPosition().z }
			);

		}
	}
	//UŒ‚
	if (SearchPlayer) {
		//’ÇÕ
		
			//enemy->ChangeState(new EnemyFollowState());
	}
}