#include "WalkAction.h"
#include "Enemy.h"
#include"Collision.h"
#include"mHelper.h"
using namespace DirectX;

EnemyActionBase::STATE WalkAction::Run(Enemy* enemy)
{
	const float DetectionRange = 10.0f;
	bool SearchPlayer = Collision::GetLength(enemy->GetPosition(), Player::GetInstance()->GetPosition()) < DetectionRange;
	enemy->Walk();
	enemy->Stop();
	if (enemy->GetSearchFlag()) {
	return EnemyActionBase::STATE::COMPLETE;
	}
}
//
//void WalkAction::Walk(Enemy* enemy)
//{
//	//���G�͈�
//	//const float DetectionRange = 10.0f;
//	////�v���C���[�����G�͈͓�������
//	//bool SearchPlayer = Collision::GetLength(enemy->GetPosition(), Player::GetInstance()->GetPosition()) < DetectionRange;
//	//if (Player::GetInstance() != nullptr) {
//	//	SearchPlayer = false;
//	//}
//	//�ړ��x�N�g����y������̊p�x�ŉ�]
//	XMVECTOR move = { 0,0,0.1f,0 };
//
//	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(enemy->GetRotation().y));
//
//	move = XMVector3TransformNormal(move, matRot);
//
//	//�����Ă�����Ɉړ�
//	//�����Ă���
//	if (wf) {
//		if (enemy->GetonFlag() == true) {
//			tempx = enemy->GetPosition().x;
//			tempz = enemy->GetPosition().z;
//			if (enemy->GetMoveFlag()) {
//				enemy->SetPosition({
//					enemy->GetPosition().x + move.m128_f32[0],
//					enemy->GetPosition().y,
//					enemy->GetPosition().z + move.m128_f32[2] }
//				);
//				movement++;
//				//enemy->SetMovement(enemy->GetMovement() + 1);
//			}
//
//		} else if (enemy->GetonFlag() == false) {
//			if (enemy->GetMoveFlag() != false) {
//				enemy->SetPosition({ tempx,enemy->GetPosition().y,tempz });
//			} else {
//				enemy->SetPosition({
//								enemy->GetPosition().x ,
//								enemy->GetPosition().y,
//								enemy->GetPosition().z }
//				);
//
//
//			}
//		}
//		//if (enemy->GetMovement() > enemy->GetRandMoveMent()) {
//		if (movement > enemy->GetRandMoveMent()) {
//
//			wf = false;
//			movement = 0;
//			sf = true;
//			
//		}
//	}
//}
//
//void WalkAction::Stop(Enemy* enemy)
//{
//	//enemy->SetMovement(0);
//
//	if (sf) {
//		if (StayCount == 0) {
//			//�C�[�W���O�|����O�̓G�̌���
//			BeforeRot = enemy->GetRotation().y;
//			//�|������̓G�̌���
//			AfterRot = enemy->GetRotation().y + enemy->GetRandMove();
//		}
//		StayCount++;
//
//		if (StayCount > 190) {//��~����
//			RotTime += 0.001f;
//			enemy->SetRotation({
//				enemy->GetRotation().x,
//				//enemy->GetRotation().y+80,
//				Easing::EaseOut(RotTime,BeforeRot, AfterRot),
//				enemy->GetRotation().z
//				});
//
//			//enemy->ChangeState(new EnemyWalkState());
//		//}
//
//		}
//		
//		if (enemy->GetRotation().y >= AfterRot) {
//			RotTime = 0;
//			StayCount = 0;
//			sf = false;
//			wf = true;
//		}
//	}
//}