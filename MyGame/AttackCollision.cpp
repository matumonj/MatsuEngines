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
	HandObb.SetOBBParam_Scl({ 5.0f,10.0f,5.0f });

	bool attackcolJudgTime_First = PlayerControl::GetInstance()->GetPlayer()->GetFbxTime() > 1.2f&& PlayerControl::GetInstance()->GetPlayer()->GetAttackType() == PlayerControl::GetInstance()->GetPlayer()->FIRST;
	bool attackcolJudgTime_Second = PlayerControl::GetInstance()->GetPlayer()->GetFbxTime() > 3.116f&& PlayerControl::GetInstance()->GetPlayer()->GetAttackType() == PlayerControl::GetInstance()->GetPlayer()->THIRD;
	bool attackcolJudgTime_Third = PlayerControl::GetInstance()->GetPlayer()->GetFbxTime() >7.2f && PlayerControl::GetInstance()->GetPlayer()->GetAttackType() == PlayerControl::GetInstance()->GetPlayer()->SECOND;
	switch (SceneManager::GetInstance()->GetScene())
	{
	case SceneManager::TUTORIAL:
		
		ColOBB(TYTORIAL);
		if (CustomButton::GetInstance()->GetAttackAction() || CustomButton::GetInstance()->Get2AttackAction()) {
				HitCol = false;
			}

		for (int i = 0; i < EnemyOBB.size(); i++) {
			if (EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0]==nullptr|| Collision::GetLength(PlayerControl::GetInstance()->GetPlayer()->GetPosition(), EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0]->GetPosition()) > 100.0f) {
				continue;
			}
			if (PlayerControl::GetInstance()->GetPlayer()->GetAttackType() != PlayerControl::GetInstance()->GetPlayer()->NON && PlayerControl::GetInstance()->GetPlayer()->GetAttackType() != PlayerControl::GetInstance()->GetPlayer()->RUN) {
				if (attackcolJudgTime_First || attackcolJudgTime_Second|| attackcolJudgTime_Third) {
					if (Collision::CheckOBBCollision(HandObb, EnemyOBB[0]) == true && !HitCol) {
						AttackEffect::GetIns()->SetParticle(EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0]->GetPosition());
						EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0]->RecvDamage(damage);
						HitCol = true;
					}
				}
			}
		}
			
		break;

	case SceneManager::PLAY:
		ColOBB(PLAY);

		if (CustomButton::GetInstance()->GetAttackAction() || CustomButton::GetInstance()->Get2AttackAction()) {
			HitCol = false;
		}

		for (int i = 0; i < EnemyOBB.size(); i++) {
			if (EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE)[i] == nullptr || Collision::GetLength(PlayerControl::GetInstance()->GetPlayer()->GetPosition(), EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE)[i]->GetPosition()) > 100.0f) {
				continue;
			}
			if (PlayerControl::GetInstance()->GetPlayer()->GetAttackType() != PlayerControl::GetInstance()->GetPlayer()->NON && PlayerControl::GetInstance()->GetPlayer()->GetAttackType() != PlayerControl::GetInstance()->GetPlayer()->RUN) {
				if (attackcolJudgTime_First || attackcolJudgTime_Second || attackcolJudgTime_Third) {
					if (Collision::CheckOBBCollision(HandObb, EnemyOBB[i]) == true && !HitCol) {
						AttackEffect::GetIns()->SetParticle(EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE)[i]->GetPosition());
						EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE)[i]->RecvDamage(damage);
						HitCol = true;
					}
				}
			}
		}
		break;

	case SceneManager::BOSS:
		ColOBB(BOSS);

		if (PlayerControl::GetInstance()->GetPlayer()->GetFbxTime() < 0.48f) {
			HitCol = false;
		}
		if (EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0] != nullptr) {

			if (PlayerControl::GetInstance()->GetPlayer()->GetAttackType() != PlayerControl::GetInstance()->GetPlayer()->NON && PlayerControl::GetInstance()->GetPlayer()->GetAttackType() != PlayerControl::GetInstance()->GetPlayer()->RUN) {
				if (attackcolJudgTime_First || attackcolJudgTime_Second || attackcolJudgTime_Third) {
					if (Collision::CheckOBBCollision(HandObb, BossEnemyOBB[0]) == true && !HitCol) {
						AttackEffect::GetIns()->SetParticle(EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->GetPosition());
						EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->RecvDamage(damage);
						HitCol = true;
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
	case AttackCollision::TYTORIAL:
		if (EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0] != nullptr) {
			EnemyOBB.resize(1);
			EnemyOBB[0].SetOBBParam_Pos(EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0]->GetPosition());
			EnemyOBB[0].SetOBBParam_Rot(EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0]->GetMatrot());
			EnemyOBB[0].SetOBBParam_Scl({ 4.0f,15.0f,4.0f });
		}
		break;
	case AttackCollision::PLAY:
		EnemyOBB.resize(EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE).size());
		for (int i = 0; i < EnemyOBB.size(); i++) {
			if (EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE)[i] == nullptr) {
				continue;
			}
			EnemyOBB[i].SetOBBParam_Pos(EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE)[i]->GetPosition());
			EnemyOBB[i].SetOBBParam_Rot(EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE)[i]->GetMatrot());
			EnemyOBB[i].SetOBBParam_Scl({ 4.0f,15.0f,4.0f });
		}
			break;

	case AttackCollision::BOSS:
		BossEnemyOBB.resize(1);
		if (EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0] != nullptr) {
			//OBB ��]�x�N�g��
			BossEnemyOBB[0].SetOBBParam_Pos(EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->GetPosition());
			BossEnemyOBB[0].SetOBBParam_Rot(EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->GetMatrot());
			BossEnemyOBB[0].SetOBBParam_Scl({ 4.0f,2.0f,4.0f });
		}
		break;
	default:
		break;
	}
}