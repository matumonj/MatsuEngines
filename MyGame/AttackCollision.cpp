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

void AttackCollision::Update()
{
}


void AttackCollision::GetCol(int damage)
{
	//剣のOBBパラメータ
	HandObb.SetOBBParam_Pos(PlayerControl::GetIns()->GetPlayer()->GetHanMat());
	HandObb.SetOBBParam_Rot(SelectSword::GetIns()->GetSword()->GetMatrot());
	HandObb.SetOBBParam_Scl(SelectSword::GetIns()->GetSword()->GetSwordObbScl());

	//プレイヤーのインスタンス取得
	Player* l_player = PlayerControl::GetIns()->GetPlayer();
	AttackEffect::GetIns()->GuarEffect(l_player->GetPosition());

	bool CheckColDis_Tutorial;

	bool attackcolJudgTime_First = l_player->GetAttackType() == PlayerControl::GetIns()->GetPlayer()->FIRST &&
		l_player->GetAnimationTime() > 0.4f;

	bool attackcolJudgTime_Second = l_player->GetAttackType() == PlayerControl::GetIns()->GetPlayer()->SECOND &&
		l_player->GetAnimationTime() > 0.6f;

	bool attackcolJudgTime_Third = l_player->GetAttackType() == PlayerControl::GetIns()->GetPlayer()->THIRD &&
		l_player->GetAnimationTime() > 0.9f;

	if (
		PlayerControl::GetIns()->GetPlayer()->GetAttackType() == PlayerControl::GetIns()->GetPlayer()->NON)
	{
		attackCol[0] = false;
		attackCol[1] = false;
		HitCol = false;
		for(int i=0;i< EnemyControl::GetIns()->GetEnemy(EnemyControl::PLAYSCENE).size();i++)
		{
			Play_colf[i] = false;
		}
		HelpJudg = false;
	}
	switch (SceneManager::GetIns()->GetScene())
	{
	case SceneManager::TUTORIAL:

		ColOBB(TYTORIAL);

		if (attackcolJudgTime_First || attackcolJudgTime_Second || attackcolJudgTime_Third)
		{
			if (EnemyControl::GetIns()->GetEnemy(EnemyControl::TUTORIAL)[0] != nullptr)
			{
				CheckColDis_Tutorial =
					Collision::GetLength(EnemyControl::GetIns()->GetEnemy(EnemyControl::TUTORIAL)[0]->GetPosition(), l_player->GetPosition()) < 20.f;

				if (CheckColDis_Tutorial&& Collision::CheckOBBCollision(HandObb, EnemyOBB[0]) == true && !HitCol)
				{
					AttackEffect::GetIns()->SetParticle(
						EnemyControl::GetIns()->GetEnemy(EnemyControl::TUTORIAL)[0]->GetPosition());
					EnemyControl::GetIns()->GetEnemy(EnemyControl::TUTORIAL)[0]->RecvDamage(
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
			if (EnemyControl::GetIns()->GetEnemy(EnemyControl::PLAYSCENE)[i] == nullptr)
			{
				continue;
			}
			if (Collision::GetLength(EnemyControl::GetIns()->GetEnemy(EnemyControl::PLAYSCENE)[i]->GetPosition(),
			                         ppos) > 80)
			{
				continue;
			}

			if (attackcolJudgTime_First || attackcolJudgTime_Second || attackcolJudgTime_Third)
			{
				if (Collision::CheckOBBCollision(HandObb, EnemyOBB[i]) == true && !Play_colf[i])
				{

					HelpJudg = true;
					AttackEffect::GetIns()->SetParticle(
						EnemyControl::GetIns()->GetEnemy(EnemyControl::PLAYSCENE)[i]->GetPosition());
					EnemyControl::GetIns()->GetEnemy(EnemyControl::PLAYSCENE)[i]->RecvDamage(
						damage + rand() % 8+ 1);
					Play_colf[i] = true;
				}
			}
		}

		if (attackcolJudgTime_First || attackcolJudgTime_Second || attackcolJudgTime_Third)
		{
			if (EnemyControl::GetIns()->GetGuardianEnemy() != nullptr)
			{
				XMFLOAT3 epos = EnemyControl::GetIns()->GetGuardianEnemy()->GetPosition();
				if (Collision::CheckOBBCollision(HandObb, GuardianEnemyOBB) == true && !HitCol)
				{
					AttackEffect::GetIns()->SetParticle({epos.x, epos.y - 10.f, epos.z});
					EnemyControl::GetIns()->GetGuardianEnemy()->RecvDamage(
						damage + rand() % 8 + 1);
					HitCol = true;
				}
			}
		}
		break;

	case SceneManager::BOSS:
		ColOBB(BOSS);

		BossCol(damage);
		break;
	default:
		break;
	}
}

void AttackCollision::BossCol(int damage)
{
	if (EnemyControl::GetIns()->GetEnemy(EnemyControl::BOSS)[0] == nullptr)return;
	OBB ob = EnemyControl::GetIns()->GetEnemy(EnemyControl::BOSS)[0]->GetShieldOBB();
	
	Player* l_player = PlayerControl::GetIns()->GetPlayer();
	//bool CheckColDis_Tutorial;
	
	bool attackcolJudgTime_First = l_player->GetAttackType() == PlayerControl::GetIns()->GetPlayer()->FIRST &&
		l_player->GetAnimationTime() > 0.4f;

	bool attackcolJudgTime_Second = l_player->GetAttackType() == PlayerControl::GetIns()->GetPlayer()->SECOND &&
		l_player->GetAnimationTime() > 0.6f;

	bool attackcolJudgTime_Third = l_player->GetAttackType() == PlayerControl::GetIns()->GetPlayer()->THIRD &&
		l_player->GetAnimationTime() > 0.9f;


	if (attackcolJudgTime_First || attackcolJudgTime_Second || attackcolJudgTime_Third)
	{
		if (Collision::CheckOBBCollision(HandObb, BossEnemyOBB[0]) == true && !HitCol)
		{
			if (EnemyControl::GetIns()->GetEnemy(EnemyControl::BOSS)[0]->GetHP() <= EnemyControl::GetIns()->GetEnemy(EnemyControl::BOSS)[0]->GetMaxHP() / 2)
			{
				if (UltAttack::GetIns()->GetPhase() == UltAttack::END)
				{
					if(EnemyControl::GetIns()->GetEnemy(EnemyControl::BOSS)[0]->GetGuardAction())
					{
						AttackEffect::GetIns()->SetGuadJudg(true);
						EnemyControl::GetIns()->GetEnemy(EnemyControl::BOSS)[0]->RecvDamage(
						0);
					}
					else {
						AttackEffect::GetIns()->SetParticle(
							EnemyControl::GetIns()->GetEnemy(EnemyControl::BOSS)[0]->GetPosition());
						EnemyControl::GetIns()->GetEnemy(EnemyControl::BOSS)[0]->RecvDamage(
							damage + rand() % 8 + 1);
					}
				}
			} else {
				if (EnemyControl::GetIns()->GetEnemy(EnemyControl::BOSS)[0]->GetGuardAction())
				{
					//if (input->TriggerButton(input->Y)) {
						AttackEffect::GetIns()->SetGuadJudg(true);
					
					EnemyControl::GetIns()->GetEnemy(EnemyControl::BOSS)[0]->RecvDamage(
						0);
				} else {
					AttackEffect::GetIns()->SetParticle(
						EnemyControl::GetIns()->GetEnemy(EnemyControl::BOSS)[0]->GetPosition());
					EnemyControl::GetIns()->GetEnemy(EnemyControl::BOSS)[0]->RecvDamage(
						damage + rand() % 8 + 1);
				}
			}
			HitCol = true;
		}


		for (int i = 0; i < 2; i++)
		{
			if (HalfAttack::GetIns()->GetSummonEnemy(i) == nullptr)
			{
				continue;
			}
			if (Collision::CheckOBBCollision(HandObb, SummonEnemyOBB[i]) == true && !attackCol[i])
			{
				HalfAttack::GetIns()->GetSummonEnemy(i)->RecvDamage(
					damage + rand() % 8 + 1);

				attackCol[i] = true;
			}
		}
	}
	XMFLOAT3 rot = EnemyControl::GetIns()->GetEnemy(EnemyControl::BOSS)[0]->GetRotation();
	XMFLOAT3 spos = EnemyControl::GetIns()->GetEnemy(EnemyControl::BOSS)[0]->HandRightPos2();

	AttackEffect::GetIns()->SetGuardRot(rot);
	AttackEffect::GetIns()->GuarEffect(spos);

}

void AttackCollision::ColOBB(ColType Enemytype)
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

				EnemyOBB[i].SetOBBParam_Scl({11.0f, 15.0f, 11.0f });
			} else {
				EnemyOBB[i].SetOBBParam_Scl({ 7.0f, 15.0f, 7.0f });
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
			if (HalfAttack::GetIns()->GetSummonEnemy(i) == nullptr|| HalfAttack::GetIns()->GetSummonEnemy(i)->GetPosition().y < 15)
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
			//OBB 回転ベクトル
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
