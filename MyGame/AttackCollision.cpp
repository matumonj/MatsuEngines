#include "AttackCollision.h"
#include"PlayerControl.h"
#include"CameraControl.h"
#include"EnemyControl.h"
#include"Collision.h"
#include"CustomButton.h"
#include"SceneManager.h"
#include"SelectSword.h"
#include"AttackEffect.h"

AttackCollision* AttackCollision::GetInstance()
{
	static AttackCollision instance;
	return &instance;
}

void AttackCollision::Init()
{
	input = Input::GetInstance();
}

void AttackCollision::Finalize()
{
	EnemyOBB.clear();
	BossEnemyOBB.clear();
}

void AttackCollision::Update()
{
}


void AttackCollision::GetCol(int damage)
{
	HandObb.SetOBBParam_Pos(PlayerControl::GetInstance()->GetPlayer()->GetHanMat());
	HandObb.SetOBBParam_Rot(SelectSword::GetInstance()->GetSword()->GetMatrot());
	HandObb.SetOBBParam_Scl({ 5.0f, 10.0f, 5.0f });

	bool attackcolJudgTime_First = PlayerControl::GetInstance()->GetPlayer()->GetFbxTime() >
		PlayerControl::GetInstance()->GetPlayer()->GetFbxTime_FirstAtack() + 0.2f && PlayerControl::GetInstance()->
		GetPlayer()->GetAttackType() == PlayerControl::GetInstance()->GetPlayer()->FIRST;
	bool attackcolJudgTime_Second = PlayerControl::GetInstance()->GetPlayer()->GetFbxTime() >
		PlayerControl::GetInstance()->GetPlayer()->GetFbxTime_SecondAtack() + 0.2f && PlayerControl::GetInstance()->
		GetPlayer()->GetAttackType() == PlayerControl::GetInstance()->GetPlayer()->SECOND;
	bool attackcolJudgTime_Third = PlayerControl::GetInstance()->GetPlayer()->GetFbxTime() >
		PlayerControl::GetInstance()->GetPlayer()->GetFbxTime_ThirdAtack() + 0.5f && PlayerControl::GetInstance()->
		GetPlayer()->GetAttackType() == PlayerControl::GetInstance()->GetPlayer()->THIRD;

	if (PlayerControl::GetInstance()->GetPlayer()->GetAttackType() == PlayerControl::GetInstance()->GetPlayer()->RUN ||
		PlayerControl::GetInstance()->GetPlayer()->GetAttackType() == PlayerControl::GetInstance()->GetPlayer()->NON)
	{
		attackCol[0] = false;
		attackCol[1] = false;
		HitCol = false;
	}
	switch (SceneManager::GetInstance()->GetScene())
	{
	case SceneManager::TUTORIAL:

		ColOBB(TYTORIAL);

		if (attackcolJudgTime_First || attackcolJudgTime_Second || attackcolJudgTime_Third)
		{
			if (Collision::CheckOBBCollision(HandObb, EnemyOBB[0]) == true && !HitCol)
			{
				AttackEffect::GetIns()->SetParticle(
					EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0]->GetPosition());
				EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0]->RecvDamage(damage);
				HitCol = true;
			}
		}

		break;

	case SceneManager::PLAY:
		ColOBB(PLAY);

		if (CustomButton::GetInstance()->GetAttackAction() || CustomButton::GetInstance()->Get2AttackAction())
		{
			HitCol = false;
		}

		for (int i = 0; i < EnemyOBB.size(); i++)
		{
			if (EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE)[i] == nullptr || Collision::GetLength(
				PlayerControl::GetInstance()->GetPlayer()->GetPosition(),
				EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE)[i]->GetPosition()) > 100.0f)
			{
				continue;
			}

			if (attackcolJudgTime_First || attackcolJudgTime_Second || attackcolJudgTime_Third)
			{
				if (Collision::CheckOBBCollision(HandObb, EnemyOBB[i]) == true && !HitCol)
				{
					AttackEffect::GetIns()->SetParticle(
						EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE)[i]->GetPosition());
					EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE)[i]->RecvDamage(damage);
					HitCol = true;
				}
			}
		}
		break;

	case SceneManager::BOSS:
		ColOBB(BOSS);

		if (EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0] != nullptr)
		{
			if (attackcolJudgTime_First || attackcolJudgTime_Second || attackcolJudgTime_Third)
			{
				if (Collision::CheckOBBCollision(HandObb, BossEnemyOBB[0]) == true && !HitCol)
				{
					AttackEffect::GetIns()->SetParticle(
						EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->GetPosition());
					if (EnemyControl::GetInstance()->GetSummonEnemysApper() == false || EnemyControl::GetInstance()->GetSummonEnemysDeath() == true) {
						EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->RecvDamage(damage);
					} else if (EnemyControl::GetInstance()->GetSummonEnemysApper() == true) {
						EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->RecvDamage(0);
					}
					HitCol = true;
				}
				for (int i = 0; i < 2; i++) {
					if (EnemyControl::GetInstance()->GetSummonEnemy(i) == nullptr)continue;
					if (Collision::CheckOBBCollision(HandObb, SummonEnemyOBB[i]) == true && !attackCol[i]) {
						EnemyControl::GetInstance()->GetSummonEnemy(i)->RecvDamage(damage);
						attackCol[i] = true;
					}
				}
			}
		}
		break;
	default:
		break;
	}
}

void AttackCollision::ColOBB(ColType Enemytype)
{
	switch (Enemytype)
	{
	case TYTORIAL:
		if (EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0] != nullptr)
		{
			EnemyOBB.resize(1);
			EnemyOBB[0].
				SetOBBParam_Pos(EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0]->GetPosition());
			EnemyOBB[0].SetOBBParam_Rot(EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0]->GetMatrot());
			EnemyOBB[0].SetOBBParam_Scl({ 4.0f, 15.0f, 4.0f });
		}
		break;
	case PLAY:
		EnemyOBB.resize(EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE).size());
		for (int i = 0; i < EnemyOBB.size(); i++)
		{
			if (EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE)[i] == nullptr)
			{
				continue;
			}
			EnemyOBB[i].SetOBBParam_Pos(
				EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE)[i]->GetPosition());
			EnemyOBB[i].SetOBBParam_Rot(EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE)[i]->GetMatrot());
			EnemyOBB[i].SetOBBParam_Scl({ 4.0f, 15.0f, 4.0f });
		}
		break;

	case BOSS:
		BossEnemyOBB.resize(1);
		for (int i = 0; i < 2; i++) {
			if (EnemyControl::GetInstance()->GetSummonEnemy(i) == nullptr)continue;
			SummonEnemyOBB[i].
				SetOBBParam_Pos(EnemyControl::GetInstance()->GetSummonEnemy(i)->GetPosition());
			SummonEnemyOBB[i].SetOBBParam_Rot(EnemyControl::GetInstance()->GetSummonEnemy(i)->GetMatrot());
			SummonEnemyOBB[i].SetOBBParam_Scl({ 4.0f, 15.0f, 4.0f });
		}

		if (EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0] != nullptr)
		{
			//OBB ‰ñ“]ƒxƒNƒgƒ‹
			BossEnemyOBB[0].
				SetOBBParam_Pos(EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->GetPosition());
			BossEnemyOBB[0].SetOBBParam_Rot(EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->GetMatrot());
			BossEnemyOBB[0].SetOBBParam_Scl({ 4.0f, 2.0f, 4.0f });
		}
		break;
	default:
		break;
	}
}
