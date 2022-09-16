#include "AttackCollision.h"
#include"PlayerControl.h"
#include"CameraControl.h"
#include"EnemyControl.h"
#include"Collision.h"
#include"CustomButton.h"
#include"SceneManager.h"
#include"SelectSword.h"
AttackCollision* AttackCollision::GetInstance()
{
	static AttackCollision instance;
	return &instance;
}
void AttackCollision::Init()
{
	input = Input::GetInstance();

	ColObb = new OBBCollision();
}

void AttackCollision::Finalize()
{
	delete ColObb;
}
void AttackCollision::Update()
{
}

void AttackCollision::GetCol(int damage)
{
	
	HandObb.m_Pos.m128_f32[0] = PlayerControl::GetInstance()->GetPlayer()->GetHanMat().r[3].m128_f32[0];// GetPosition().x;
	HandObb.m_Pos.m128_f32[1] = PlayerControl::GetInstance()->GetPlayer()->GetHanMat().r[3].m128_f32[1];
	HandObb.m_Pos.m128_f32[2] = PlayerControl::GetInstance()->GetPlayer()->GetHanMat().r[3].m128_f32[2];

	//���̓v���C���[�̎�ɓ����蔻��@���Əo�C��
	HandObb.m_NormaDirect[0] = { PlayerControl::GetInstance()->GetPlayer()->GetMatrot().r[0].m128_f32[0],PlayerControl::GetInstance()->GetPlayer()->GetMatrot().r[0].m128_f32[1],PlayerControl::GetInstance()->GetPlayer()->GetMatrot().r[0].m128_f32[2] };
	HandObb.m_NormaDirect[1] = { PlayerControl::GetInstance()->GetPlayer()->GetMatrot().r[1].m128_f32[0], PlayerControl::GetInstance()->GetPlayer()->GetMatrot().r[1].m128_f32[1], PlayerControl::GetInstance()->GetPlayer()->GetMatrot().r[1].m128_f32[2] };
	HandObb.m_NormaDirect[2] = { PlayerControl::GetInstance()->GetPlayer()->GetMatrot().r[2].m128_f32[0], PlayerControl::GetInstance()->GetPlayer()->GetMatrot().r[2].m128_f32[1], PlayerControl::GetInstance()->GetPlayer()->GetMatrot().r[2].m128_f32[2] };
	HandObb.m_fLength[0] = 3;//x�����̒���
	HandObb.m_fLength[1] = 3;//y�����̒���
	HandObb.m_fLength[2] = 15;//z�����̒���

	switch (SceneManager::GetInstance()->GetScene())
	{
	case SceneManager::TUTORIAL:
		if (colf) {
			EnemyControl::GetInstance()->GetTutorialEnemyindex()[0]->RecvDamage(damage);
		}

		ColOBB(TYTORIAL);

		for (int i = 0; i < EnemyOBB.size(); i++) {
			if (ColObb->ColOBBs(HandObb, EnemyOBB[i])) {
				colf = true;
				break;
			}
			else {
				colf = false;
			}
		}
		break;

	case SceneManager::PLAY:
	
		break;

	case SceneManager::BOSS:
		
		break;
	default:
		break;
	}
}
void AttackCollision::Draw()
{
}

void AttackCollision::ColOBB(ColType Enemytype)
{
	switch (Enemytype)
	{
	case AttackCollision::TYTORIAL:
		EnemyOBB.resize(1);

		//OBB ��]�x�N�g��
		EnemyOBB[0].m_NormaDirect[0] = {EnemyControl::GetInstance()->GetTutorialEnemyindex()[0]->GetMatrot().r[0].m128_f32[0],EnemyControl::GetInstance()->GetTutorialEnemyindex()[0]->GetMatrot().r[0].m128_f32[1],EnemyControl::GetInstance()->GetTutorialEnemyindex()[0]->GetMatrot().r[0].m128_f32[2]};
		EnemyOBB[0].m_NormaDirect[1] = { EnemyControl::GetInstance()->GetTutorialEnemyindex()[0]->GetMatrot().r[1].m128_f32[0], EnemyControl::GetInstance()->GetTutorialEnemyindex()[0]->GetMatrot().r[1].m128_f32[1], EnemyControl::GetInstance()->GetTutorialEnemyindex()[0]->GetMatrot().r[1].m128_f32[2] };
		EnemyOBB[0].m_NormaDirect[2] = { EnemyControl::GetInstance()->GetTutorialEnemyindex()[0]->GetMatrot().r[2].m128_f32[0], EnemyControl::GetInstance()->GetTutorialEnemyindex()[0]->GetMatrot().r[2].m128_f32[1], EnemyControl::GetInstance()->GetTutorialEnemyindex()[0]->GetMatrot().r[2].m128_f32[2] };
		EnemyOBB[0].m_fLength[0] = 2;//x�����̒���
		EnemyOBB[0].m_fLength[1] = 2;//y�����̒���
		EnemyOBB[0].m_fLength[2] = 2;//z�����̒���

		EnemyOBB[0].m_Pos.m128_f32[0] = { EnemyControl::GetInstance()->GetTutorialEnemyindex()[0]->GetPosition().x };
		EnemyOBB[0].m_Pos.m128_f32[1] = { EnemyControl::GetInstance()->GetTutorialEnemyindex()[0]->GetPosition().y };
		EnemyOBB[0].m_Pos.m128_f32[2] = { EnemyControl::GetInstance()->GetTutorialEnemyindex()[0]->GetPosition().z };

		break;
	case AttackCollision::PLAY://��ʂ�OBB����@�y�ʉ��K�v����
		EnemyOBB.resize(EnemyControl::GetInstance()->GetEnemyindex(1).size());

		for (int i = 0; i < EnemyOBB.size(); i++) {
			if(Collision::GetLength(EnemyControl::GetInstance()->GetEnemyindex(0)[i]->GetPosition(), PlayerControl::GetInstance()->GetPlayer()->GetPosition()) < 15.0f) {
				//OBB ��]�x�N�g��
				EnemyOBB[i].m_NormaDirect[0] = { EnemyControl::GetInstance()->GetEnemyindex(0)[i]->GetMatrot().r[0].m128_f32[0],EnemyControl::GetInstance()->GetEnemyindex(0)[i]->GetMatrot().r[0].m128_f32[1],EnemyControl::GetInstance()->GetEnemyindex(0)[i]->GetMatrot().r[0].m128_f32[2] };
				EnemyOBB[i].m_NormaDirect[1] = { EnemyControl::GetInstance()->GetEnemyindex(0)[i]->GetMatrot().r[1].m128_f32[0], EnemyControl::GetInstance()->GetEnemyindex(0)[i]->GetMatrot().r[1].m128_f32[1], EnemyControl::GetInstance()->GetEnemyindex(0)[i]->GetMatrot().r[1].m128_f32[2] };
				EnemyOBB[i].m_NormaDirect[2] = { EnemyControl::GetInstance()->GetEnemyindex(0)[i]->GetMatrot().r[2].m128_f32[0], EnemyControl::GetInstance()->GetEnemyindex(0)[i]->GetMatrot().r[2].m128_f32[1], EnemyControl::GetInstance()->GetEnemyindex(0)[i]->GetMatrot().r[2].m128_f32[2] };
				EnemyOBB[i].m_fLength[0] = 2;//x�����̒���
				EnemyOBB[i].m_fLength[1] = 2;//y�����̒���
				EnemyOBB[i].m_fLength[2] = 2;//z�����̒���

				EnemyOBB[i].m_Pos.m128_f32[0] = { EnemyControl::GetInstance()->GetTutorialEnemyindex()[0]->GetPosition().x };
				EnemyOBB[i].m_Pos.m128_f32[1] = { EnemyControl::GetInstance()->GetTutorialEnemyindex()[0]->GetPosition().y };
				EnemyOBB[i].m_Pos.m128_f32[2] = { EnemyControl::GetInstance()->GetTutorialEnemyindex()[0]->GetPosition().z };
			}
		}

		break;
	case AttackCollision::BOSS:
		EnemyOBB.resize(1);

		//OBB ��]�x�N�g��
		EnemyOBB[0].m_NormaDirect[0] = { EnemyControl::GetInstance()->GetBossEnemyindex()[0]->GetMatrot().r[0].m128_f32[0],EnemyControl::GetInstance()->GetBossEnemyindex()[0]->GetMatrot().r[0].m128_f32[1],EnemyControl::GetInstance()->GetBossEnemyindex()[0]->GetMatrot().r[0].m128_f32[2] };
		EnemyOBB[0].m_NormaDirect[1] = { EnemyControl::GetInstance()->GetBossEnemyindex()[0]->GetMatrot().r[1].m128_f32[0], EnemyControl::GetInstance()->GetBossEnemyindex()[0]->GetMatrot().r[1].m128_f32[1], EnemyControl::GetInstance()->GetBossEnemyindex()[0]->GetMatrot().r[1].m128_f32[2] };
		EnemyOBB[0].m_NormaDirect[2] = { EnemyControl::GetInstance()->GetBossEnemyindex()[0]->GetMatrot().r[2].m128_f32[0], EnemyControl::GetInstance()->GetBossEnemyindex()[0]->GetMatrot().r[2].m128_f32[1], EnemyControl::GetInstance()->GetBossEnemyindex()[0]->GetMatrot().r[2].m128_f32[2] };
		EnemyOBB[0].m_fLength[0] = 2;//x�����̒���
		EnemyOBB[0].m_fLength[1] = 2;//y�����̒���
		EnemyOBB[0].m_fLength[2] = 2;//z�����̒���

		EnemyOBB[0].m_Pos.m128_f32[0] = { EnemyControl::GetInstance()->GetBossEnemyindex()[0]->GetPosition().x };
		EnemyOBB[0].m_Pos.m128_f32[1] = { EnemyControl::GetInstance()->GetBossEnemyindex()[0]->GetPosition().y };
		EnemyOBB[0].m_Pos.m128_f32[2] = { EnemyControl::GetInstance()->GetBossEnemyindex()[0]->GetPosition().z };


		break;
	default:
		break;
	}
}