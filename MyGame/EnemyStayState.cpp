#include "EnemyStayState.h"
#include "EnemyWalkState.h"
#include"EnemyAttackState.h"
#include"EnemyFollowState.h"
#include"mHelper.h"
void EnemyStayState::Initialize(Enemy* enemy)
{

}
void EnemyStayState::Update(Enemy* enemy)
{
	if (StayCount == 0) {
		//�C�[�W���O�|����O�̓G�̌���
		BeforeRot = enemy->GetRotation().y;
		//�|������̓G�̌���
		AfterRot = enemy->GetRotation().y + enemy->GetRandMove();
	}
	
	StayCount++;
	if (StayCount > 90) {//��~����
		RotTime += 0.01f;
		enemy->SetRotation({
			enemy->GetRotation().x,
			Easing::EaseOut(RotTime,BeforeRot, AfterRot),
			enemy->GetRotation().z
			});
		if (RotTime>0.6) {
			enemy->ChangeState(new EnemyWalkState());
		}
	}
}
void EnemyStayState(Enemy* enemy)
{

}