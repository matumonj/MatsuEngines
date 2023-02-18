#include "AttackCollision.h"
#include"PlayerControl.h"
#include"EnemyControl.h"
#include"Collision.h"
#include"SceneManager.h"
#include"SelectSword.h"
#include"AttackEffect.h"
#include "CameraControl.h"
#include "HalfAttack.h"
#include "PlayerAttackState.h"
#include "UltAttack.h"

AttackCollision* AttackCollision::GetIns()
{
	static AttackCollision instance;
	return &instance;
}

void AttackCollision::Init()
{
	input = Input::GetIns();
}

void AttackCollision::Finalize()
{
	EnemyOBB.clear();
	BossEnemyOBB.clear();
}


void AttackCollision::GetCol(int damage)
{
	
	//�v���C���[�̃C���X�^���X�擾
	Player* l_player = PlayerControl::GetIns()->GetPlayer();
		//����OBB�p�����[�^
	HandObb.SetOBBParam_Pos(PlayerControl::GetIns()->GetPlayer()->GetHanMat());
	HandObb.SetOBBParam_Rot(SelectSword::GetIns()->GetSword()->GetMatrot());
	HandObb.SetOBBParam_Scl(SelectSword::GetIns()->GetSword()->GetSwordObbScl());

	//�A�j���[�V���������t���[����������(�U�肩�Ԃ����u�Ԕ�������̖h��)
	attackcolJudgTime_First = l_player->GetAttackType() == PlayerControl::GetIns()->GetPlayer()->FIRST &&
		l_player->GetAnimationTime() > 0.4f;

	attackcolJudgTime_Second = l_player->GetAttackType() == PlayerControl::GetIns()->GetPlayer()->SECOND &&
		l_player->GetAnimationTime() > 0.6f;

	attackcolJudgTime_Third = l_player->GetAttackType() == PlayerControl::GetIns()->GetPlayer()->THIRD &&
		l_player->GetAnimationTime() > 0.9f;

	//����t���O�؂�
	if (PlayerControl::GetIns()->GetPlayer()->GetAttackType() == PlayerControl::GetIns()->GetPlayer()->NON)
	{
		attackCol[0] = false;
		attackCol[1] = false;
		HitCol = false;
		if (Play_colf.size() == 0)return;
		for (int i = 0; i < EnemyControl::GetIns()->GetEnemy(EnemyControl::PLAYSCENE).size(); i++)
		{
			Play_colf[i] = false;
		}
		HelpJudg = false;
	}

	//�e�V�[�����Ƃ̓����蔻��
	switch (SceneManager::GetIns()->GetScene())
	{
	case SceneManager::TUTORIAL:

		OBBParamSet(TYTORIAL);
		TutorialCol(damage);
		if (PlayerControl::GetIns()->GetPlayer()->GetStopFlag() == false) {
			if (input->TiltPushStick(Input::L_UP, 0.0f) ||
				input->TiltPushStick(Input::L_DOWN, 0.0f) ||
				input->TiltPushStick(Input::L_RIGHT, 0.0f) ||
				input->TiltPushStick(Input::L_LEFT, 0.0f))
			{
				CameraControl::GetIns()->SetZoomF(false);
			}
		}
		break;

	case SceneManager::PLAY:
		OBBParamSet(PLAY);
		ExplorationCol(damage);
		if (PlayerControl::GetIns()->GetPlayer()->GetStopFlag() == false) {
			if (input->TiltPushStick(Input::L_UP, 0.0f) ||
				input->TiltPushStick(Input::L_DOWN, 0.0f) ||
				input->TiltPushStick(Input::L_RIGHT, 0.0f) ||
				input->TiltPushStick(Input::L_LEFT, 0.0f))
			{
				CameraControl::GetIns()->SetZoomF(false);
			}
		}
		break;

	case SceneManager::BOSS:
		OBBParamSet(BOSS);
		BossCol(damage);
		if (PlayerControl::GetIns()->GetPlayer()->GetStopFlag() == false) {
			if (input->TiltPushStick(Input::L_UP, 0.0f) ||
				input->TiltPushStick(Input::L_DOWN, 0.0f) ||
				input->TiltPushStick(Input::L_RIGHT, 0.0f) ||
				input->TiltPushStick(Input::L_LEFT, 0.0f))
			{
				CameraControl::GetIns()->SetZoomF(false);
			}
		}
		break;
	default:
		break;
	}
}

void AttackCollision::BossCol(int damage)
{
	Player* l_player = PlayerControl::GetIns()->GetPlayer();

	//��O�ݒ�
	if (EnemyControl::GetIns()->GetEnemy(EnemyControl::BOSS)[0] == nullptr) { return; }

	if (!attackcolJudgTime_First && !attackcolJudgTime_Second && !attackcolJudgTime_Third)
	{
		return;
	}
	if (EnemyControl::GetIns()->GetEnemy(EnemyControl::BOSS)[0]->GetHP() <= EnemyControl::GetIns()->
		GetEnemy(EnemyControl::BOSS)[0]->GetMaxHP() / 2)
	{
		if (UltAttack::GetIns()->GetPhase() != UltAttack::PHASE_FOUR)
		{
			return;
		}
	}

	//�����蔻�蕔��
	if (Collision::CheckOBBCollision(HandObb, BossEnemyOBB[0]) == true && !HitCol)
	{
		PlayerAttackState::GetIns()->SetHitStopJudg(true, 20);
		CameraControl::GetIns()->SetZoomF(true);
		CameraControl::GetIns()->SetZoomTarget(EnemyControl::GetIns()->GetEnemy(EnemyControl::BOSS)[0]->GetPosition());

		//�K�[�h���̓_���[�W�󂯂Ȃ�
		if (EnemyControl::GetIns()->GetEnemy(EnemyControl::BOSS)[0]->GetGuardAction())
		{
			AttackEffect::GetIns()->SetGuadJudg(true);
			EnemyControl::GetIns()->GetEnemy(EnemyControl::BOSS)[0]->RecvDamage(0);
		}
		//�ʏ�̔�_���[�W����
		else
		{
			PlayerAttackState::GetIns()->SetHitStopJudg(true, 30);
			CameraControl::GetIns()->SetZoomF(true);
			CameraControl::GetIns()->SetZoomTarget(EnemyControl::GetIns()->GetEnemy(EnemyControl::BOSS)[0]->GetPosition());
			AttackEffect::GetIns()->SetParticle(
				EnemyControl::GetIns()->GetEnemy(EnemyControl::BOSS)[0]->GetPosition());
			EnemyControl::GetIns()->GetEnemy(EnemyControl::BOSS)[0]->RecvDamage(damage);
		}
		HitCol = true;
	}


	//�����I�Ƃ̓����蔻��
	for (int i = 0; i < 2; i++)
	{
		if (HalfAttack::GetIns()->GetSummonEnemy(i) == nullptr)
		{
			continue;
		}
		if (Collision::CheckOBBCollision(HandObb, SummonEnemyOBB[i]) == true && !attackCol[i])
		{
			PlayerAttackState::GetIns()->SetHitStopJudg(true, 30);
			HalfAttack::GetIns()->GetSummonEnemy(i)->RecvDamage(damage);

			attackCol[i] = true;
		}
	}

}
#include"TutorialSprite.h"
void AttackCollision::TutorialCol(int damage)
{
	//��O�ݒ�
	if (!attackcolJudgTime_First && !attackcolJudgTime_Second && !attackcolJudgTime_Third)
	{
		return;
	}
	if (TutorialSprite::GetIns()->GetClearMove()==false||EnemyControl::GetIns()->GetEnemy(EnemyControl::TUTORIAL)[0] == nullptr)
	{
		return;
	}

	//���蕔��
	if (Collision::CheckOBBCollision(HandObb, EnemyOBB[0]) == true && !HitCol)
	{
		CameraControl::GetIns()->SetZoomF(true);
		CameraControl::GetIns()->SetZoomTarget(EnemyControl::GetIns()->GetEnemy(EnemyControl::TUTORIAL)[0]->GetPosition());

		PlayerAttackState::GetIns()->SetHitStopJudg(true, 40);
		AttackEffect::GetIns()->SetParticle(
			EnemyControl::GetIns()->GetEnemy(EnemyControl::TUTORIAL)[0]->GetPosition());
		EnemyControl::GetIns()->GetEnemy(EnemyControl::TUTORIAL)[0]->RecvDamage(damage);

		HitCol = true;
	}
}


void AttackCollision::ExplorationCol(int damage)
{
	if (!attackcolJudgTime_First && !attackcolJudgTime_Second && !attackcolJudgTime_Third)
	{
		return;
	}

	Player* l_player = PlayerControl::GetIns()->GetPlayer();
	XMFLOAT3 ppos = l_player->GetPosition();

	//���������v���C���[�����苗������Ă��画����Ȃ�
	constexpr float NotColDistance = 30.f;

	for (int i = 0; i < EnemyOBB.size(); i++)
	{
		if (EnemyControl::GetIns()->GetEnemy(EnemyControl::PLAYSCENE)[i] == nullptr)
		{
			continue;
		}
		if (Collision::GetLength(EnemyControl::GetIns()->GetEnemy(EnemyControl::PLAYSCENE)[i]->GetPosition(),
			ppos) > NotColDistance)
		{
			continue;
		}

		//�G���G
		if (Collision::CheckOBBCollision(HandObb, EnemyOBB[i]) == true && !Play_colf[i])
		{
			CameraControl::GetIns()->SetZoomF(true);
			CameraControl::GetIns()->SetZoomTarget(EnemyControl::GetIns()->GetEnemy(EnemyControl::PLAYSCENE)[i]->GetPosition());
			HelpJudg = true;
			PlayerAttackState::GetIns()->SetHitStopJudg(true, 40);
			AttackEffect::GetIns()->SetParticle(
				EnemyControl::GetIns()->GetEnemy(EnemyControl::PLAYSCENE)[i]->GetPosition());
			EnemyControl::GetIns()->GetEnemy(EnemyControl::PLAYSCENE)[i]->RecvDamage(
				damage);
			Play_colf[i] = true;
		}
	}


	//�K�[�f�B�A��
	if (EnemyControl::GetIns()->GetGuardianEnemy() != nullptr)
	{
		XMFLOAT3 epos = EnemyControl::GetIns()->GetGuardianEnemy()->GetPosition();
		if (Collision::CheckOBBCollision(HandObb, GuardianEnemyOBB) == true && !HitCol)
		{
			PlayerAttackState::GetIns()->SetHitStopJudg(true, 30);
			AttackEffect::GetIns()->SetParticle({ epos.x, epos.y - 10.f, epos.z });
			EnemyControl::GetIns()->GetGuardianEnemy()->RecvDamage(
				damage);
			HitCol = true;
		}
	}
	
}
void AttackCollision::OBBParamSet(ColType Enemytype)
{
	switch (Enemytype)
	{
	case TYTORIAL:
		if (EnemyControl::GetIns()->GetEnemy(EnemyControl::TUTORIAL)[0] != nullptr)
		{
			EnemyOBB.resize(1);
			EnemyOBB[0].
				SetOBBParam_Pos(EnemyControl::GetIns()->GetEnemy(EnemyControl::TUTORIAL)[0]->GetPosition());
			EnemyOBB[0].SetOBBParam_Rot(EnemyControl::GetIns()->GetEnemy(EnemyControl::TUTORIAL)[0]->GetMatrot());
			EnemyOBB[0].SetOBBParam_Scl({2.0f, 5.0f, 2.0f});
		}
		break;
	case PLAY:
		Play_colf.resize(EnemyControl::GetIns()->GetEnemy(EnemyControl::PLAYSCENE).size());
		EnemyOBB.resize(EnemyControl::GetIns()->GetEnemy(EnemyControl::PLAYSCENE).size());
		for (int i = 0; i < EnemyOBB.size(); i++)
		{
			if (EnemyControl::GetIns()->GetEnemy(EnemyControl::PLAYSCENE)[i] == nullptr)
			{
				continue;
			}
			EnemyOBB[i].SetOBBParam_Pos(
				EnemyControl::GetIns()->GetEnemy(EnemyControl::PLAYSCENE)[i]->GetPosition());
			EnemyOBB[i].SetOBBParam_Rot(EnemyControl::GetIns()->GetEnemy(EnemyControl::PLAYSCENE)[i]->GetMatrot());
			if (EnemyControl::GetIns()->GetEnemy(EnemyControl::PLAYSCENE)[i]->GetEnemyNumber() == 3)
			{
				EnemyOBB[i].SetOBBParam_Scl({11.0f, 15.0f, 11.0f});
			}
			else
			{
				EnemyOBB[i].SetOBBParam_Scl({7.0f, 15.0f, 7.0f});
			}
		}

		if (EnemyControl::GetIns()->GetGuardianEnemy() != nullptr)
		{
			GuardianEnemyOBB.SetOBBParam_Pos(EnemyControl::GetIns()->GetGuardianEnemy()->GetPosition());
			GuardianEnemyOBB.SetOBBParam_Rot(EnemyControl::GetIns()->GetGuardianEnemy()->GetMatrotObj());
			GuardianEnemyOBB.SetOBBParam_Scl({4, 15, 4});
		}
		break;

	case BOSS:
		BossEnemyOBB.resize(1);
		for (int i = 0; i < 2; i++)
		{
			if (HalfAttack::GetIns()->GetSummonEnemy(i) == nullptr || HalfAttack::GetIns()->GetSummonEnemy(i)->
				GetPosition().y < 15)
			{
				continue;
			}
			SummonEnemyOBB[i].
				SetOBBParam_Pos(HalfAttack::GetIns()->GetSummonEnemy(i)->GetPosition());
			SummonEnemyOBB[i].SetOBBParam_Rot(HalfAttack::GetIns()->GetSummonEnemy(i)->GetMatrot());
			SummonEnemyOBB[i].SetOBBParam_Scl({4.0f, 15.0f, 4.0f});
		}

		if (EnemyControl::GetIns()->GetEnemy(EnemyControl::BOSS)[0] != nullptr)
		{
			//OBB ��]�x�N�g��
			BossEnemyOBB[0].
				SetOBBParam_Pos(EnemyControl::GetIns()->GetEnemy(EnemyControl::BOSS)[0]->GetPosition());
			BossEnemyOBB[0].SetOBBParam_Rot(EnemyControl::GetIns()->GetEnemy(EnemyControl::BOSS)[0]->GetMatrot());
			BossEnemyOBB[0].SetOBBParam_Scl({8.0f, 20.0f, 8.0f});
		}
		break;
	default:
		break;
	}
}
