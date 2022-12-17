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
	HandObb.SetOBBParam_Scl({2.0f, 10.0f, 2.0f});

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
			if (EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0] != nullptr)
			{
				if (Collision::CheckOBBCollision(HandObb, EnemyOBB[0]) == true && !HitCol)
				{
					AttackEffect::GetIns()->SetParticle(
						EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0]->GetPosition());
					EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0]->RecvDamage(
						damage + rand() % 20 + 10);
					HitCol = true;
				}
			}
		}
		break;

	case SceneManager::PLAY:
		ColOBB(PLAY);
		XMFLOAT3 ppos = PlayerControl::GetInstance()->GetPlayer()->GetPosition();
		for (int i = 0; i < EnemyOBB.size(); i++)
		{
			if (EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE)[i] == nullptr)
			{
				continue;
			}
			if (Collision::GetLength(EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE)[i]->GetPosition(),
			                         ppos) > 40)
			{
				continue;
			}

			if (attackcolJudgTime_First || attackcolJudgTime_Second || attackcolJudgTime_Third)
			{
				if (Collision::CheckOBBCollision(HandObb, EnemyOBB[i]) == true && !HitCol)
				{
					AttackEffect::GetIns()->SetParticle(
						EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE)[i]->GetPosition());
					EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE)[i]->RecvDamage(
						damage + rand() % 20 + 10);
					HitCol = true;
				}
			
			}
		}

		if (attackcolJudgTime_First || attackcolJudgTime_Second || attackcolJudgTime_Third)
		{
			if (EnemyControl::GetInstance()->GetGuardianEnemy() != nullptr) {
				XMFLOAT3 epos = EnemyControl::GetInstance()->GetGuardianEnemy()->GetPosition();
				if (Collision::CheckOBBCollision(HandObb, GuardianEnemyOBB) == true && !HitCol)
				{
					AttackEffect::GetIns()->SetParticle({epos.x,epos.y-10.f,epos.z});
					EnemyControl::GetInstance()->GetGuardianEnemy()->RecvDamage(
						damage + rand() % 20 + 10);
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
					if (EnemyControl::GetInstance()->GetSummonEnemysApper() == false || EnemyControl::GetInstance()->
						GetSummonEnemysDeath() == true)
					{
						EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->RecvDamage(
							damage + rand() % 20 + 10);
					}
					else if (EnemyControl::GetInstance()->GetSummonEnemysApper() == true)
					{
						EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->RecvDamage(0);
					}
					HitCol = true;
				}
				for (int i = 0; i < 2; i++)
				{
					if (EnemyControl::GetInstance()->GetSummonEnemy(i) == nullptr)
					{
						continue;
					}
					if (Collision::CheckOBBCollision(HandObb, SummonEnemyOBB[i]) == true && !attackCol[i])
					{
						EnemyControl::GetInstance()->GetSummonEnemy(i)->RecvDamage(damage + rand() % 20 + 10);
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
			EnemyOBB[0].SetOBBParam_Scl({4.0f, 15.0f, 4.0f});
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
			EnemyOBB[i].SetOBBParam_Scl({4.0f, 15.0f, 4.0f});
		}

		if (EnemyControl::GetInstance()->GetGuardianEnemy() != nullptr) {
			GuardianEnemyOBB.SetOBBParam_Pos(EnemyControl::GetInstance()->GetGuardianEnemy()->GetPosition());
			GuardianEnemyOBB.SetOBBParam_Rot(EnemyControl::GetInstance()->GetGuardianEnemy()->GetMatrotObj());
			GuardianEnemyOBB.SetOBBParam_Scl({ 4,15,4 });
		}
			break;

	case BOSS:
		BossEnemyOBB.resize(1);
		for (int i = 0; i < 2; i++)
		{
			if (EnemyControl::GetInstance()->GetSummonEnemy(i) == nullptr)
			{
				continue;
			}
			SummonEnemyOBB[i].
				SetOBBParam_Pos(EnemyControl::GetInstance()->GetSummonEnemy(i)->GetPosition());
			SummonEnemyOBB[i].SetOBBParam_Rot(EnemyControl::GetInstance()->GetSummonEnemy(i)->GetMatrot());
			SummonEnemyOBB[i].SetOBBParam_Scl({4.0f, 15.0f, 4.0f});
		}

		if (EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0] != nullptr)
		{
			//OBB ‰ñ“]ƒxƒNƒgƒ‹
			BossEnemyOBB[0].
				SetOBBParam_Pos(EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->GetPosition());
			BossEnemyOBB[0].SetOBBParam_Rot(EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->GetMatrot());
			BossEnemyOBB[0].SetOBBParam_Scl({4.0f, 2.0f, 4.0f});
		}
		break;
	default:
		break;
	}
}
