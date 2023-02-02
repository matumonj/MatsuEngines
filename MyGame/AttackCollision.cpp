#include "AttackCollision.h"
#include"PlayerControl.h"
#include"EnemyControl.h"
#include"Collision.h"
#include"SceneManager.h"
#include"SelectSword.h"
#include"AttackEffect.h"
#include "HalfAttack.h"
#include "PlayerAttackState.h"
#include "UltAttack.h"

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
	//剣のOBBパラメータ
	HandObb.SetOBBParam_Pos(PlayerControl::GetInstance()->GetPlayer()->GetHanMat());
	HandObb.SetOBBParam_Rot(SelectSword::GetInstance()->GetSword()->GetMatrot());
	HandObb.SetOBBParam_Scl(SelectSword::GetInstance()->GetSword()->GetSwordObbScl());

	//プレイヤーのインスタンス取得
	Player* l_player = PlayerControl::GetInstance()->GetPlayer();
	bool CheckColDis_Tutorial;

	bool attackcolJudgTime_First = l_player->GetAttackType() == PlayerControl::GetInstance()->GetPlayer()->FIRST &&
		l_player->GetAnimationTime() > 0.4f;

	bool attackcolJudgTime_Second = l_player->GetAttackType() == PlayerControl::GetInstance()->GetPlayer()->SECOND &&
		l_player->GetAnimationTime() > 0.6f;

	bool attackcolJudgTime_Third = l_player->GetAttackType() == PlayerControl::GetInstance()->GetPlayer()->THIRD &&
		l_player->GetAnimationTime() > 0.9f;

	if (
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
				CheckColDis_Tutorial =
					Collision::GetLength(EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0]->GetPosition(), l_player->GetPosition()) < 20.f;

				if (CheckColDis_Tutorial&& Collision::CheckOBBCollision(HandObb, EnemyOBB[0]) == true && !HitCol)
				{
					AttackEffect::GetIns()->SetParticle(
						EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0]->GetPosition());
					EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0]->RecvDamage(
						damage + rand() % 8 + 1);
					HitCol = true;
				}
			}
		}
		break;

	case SceneManager::PLAY:
		ColOBB(PLAY);
		XMFLOAT3 ppos = l_player->GetPosition();
		for (int i = 0; i < EnemyOBB.size(); i++)
		{
			if (EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE)[i] == nullptr)
			{
				continue;
			}
			if (Collision::GetLength(EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE)[i]->GetPosition(),
			                         ppos) > 80)
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
						damage + rand() % 8+ 1);
					HitCol = true;
				}
			}
		}

		if (attackcolJudgTime_First || attackcolJudgTime_Second || attackcolJudgTime_Third)
		{
			if (EnemyControl::GetInstance()->GetGuardianEnemy() != nullptr)
			{
				XMFLOAT3 epos = EnemyControl::GetInstance()->GetGuardianEnemy()->GetPosition();
				if (Collision::CheckOBBCollision(HandObb, GuardianEnemyOBB) == true && !HitCol)
				{
					AttackEffect::GetIns()->SetParticle({epos.x, epos.y - 10.f, epos.z});
					EnemyControl::GetInstance()->GetGuardianEnemy()->RecvDamage(
						damage + rand() % 8 + 1);
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
					if(EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->GetHP()<= EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->GetMaxHP()/2)
					{
						if(UltAttack::GetIns()->GetPhase()==UltAttack::END)
						{
							AttackEffect::GetIns()->SetParticle(
								EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->GetPosition());
							EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->RecvDamage(
								damage + rand() % 8 + 1);

						}
					}
					else {
						AttackEffect::GetIns()->SetParticle(
							EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->GetPosition());
						EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->RecvDamage(
							damage + rand() % 8 + 1);
					}
					HitCol = true;
				}
				for (int i = 0; i < 2; i++)
				{
					if (HalfAttack::GetInstance()->GetSummonEnemy(i) == nullptr)
					{
						continue;
					}
					if (Collision::CheckOBBCollision(HandObb, SummonEnemyOBB[i]) == true && !attackCol[i])
					{
						HalfAttack::GetInstance()->GetSummonEnemy(i)->RecvDamage(
							damage + rand() % 8 + 1);
						
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
			EnemyOBB[0].SetOBBParam_Scl({2.0f, 5.0f, 2.0f});
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
			if (EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE)[i]->GetEnemyNumber() == 3)
			{

				EnemyOBB[i].SetOBBParam_Scl({11.0f, 15.0f, 11.0f });
			} else {
				EnemyOBB[i].SetOBBParam_Scl({ 7.0f, 15.0f, 7.0f });
			}
		}

		if (EnemyControl::GetInstance()->GetGuardianEnemy() != nullptr)
		{
			GuardianEnemyOBB.SetOBBParam_Pos(EnemyControl::GetInstance()->GetGuardianEnemy()->GetPosition());
			GuardianEnemyOBB.SetOBBParam_Rot(EnemyControl::GetInstance()->GetGuardianEnemy()->GetMatrotObj());
			GuardianEnemyOBB.SetOBBParam_Scl({4, 15, 4});
		}
		break;

	case BOSS:
		BossEnemyOBB.resize(1);
		for (int i = 0; i < 2; i++)
		{
			if (HalfAttack::GetInstance()->GetSummonEnemy(i) == nullptr|| HalfAttack::GetInstance()->GetSummonEnemy(i)->GetPosition().y < 15)
			{
				continue;
			}
			SummonEnemyOBB[i].
				SetOBBParam_Pos(HalfAttack::GetInstance()->GetSummonEnemy(i)->GetPosition());
			SummonEnemyOBB[i].SetOBBParam_Rot(HalfAttack::GetInstance()->GetSummonEnemy(i)->GetMatrot());
			SummonEnemyOBB[i].SetOBBParam_Scl({4.0f, 15.0f, 4.0f});
		}

		if (EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0] != nullptr)
		{
			//OBB 回転ベクトル
			BossEnemyOBB[0].
				SetOBBParam_Pos(EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->GetPosition());
			BossEnemyOBB[0].SetOBBParam_Rot(EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->GetMatrot());
			BossEnemyOBB[0].SetOBBParam_Scl({8.0f, 20.0f, 8.0f});
		}
		break;
	default:
		break;
	}
}
