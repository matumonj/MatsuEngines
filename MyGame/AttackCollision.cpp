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
	//PlayColOBB.resize()
	Play_colf.resize(EnemyControl::GetInstance()->GetEnemyindex(1).size());
}

void AttackCollision::Finalize()
{
	EnemyOBB.clear();
	BossEnemyOBB.clear();
	Play_colf.clear();
}
void AttackCollision::Update()
{
	PlayEnemyOBB.resize(EnemyControl::GetInstance()->GetEnemyindex(1).size());
	for (int i = 0; i < PlayEnemyOBB.size(); i++) {
		//if (PlayColOBB[i] == nullptr) {
		
		//}
	}



	//for (int i = 0; i < PlayEnemyOBB.size(); i++) {
		//OBB 回転ベクトル
	if(SceneManager::GetInstance()->GetScene()== SceneManager::PLAY) {
	
	}
}


void AttackCollision::GetCol(int damage)
{
	HandObb.SetOBBParam_Pos(PlayerControl::GetInstance()->GetPlayer()->GetHanMat());
	HandObb.SetOBBParam_Rot(SelectSword::GetInstance()->GetSword()->GetMatrot());
	HandObb.SetOBBParam_Scl({ 3.0f,5.0f,2.0f });

	
	switch (SceneManager::GetInstance()->GetScene())
	{
	case SceneManager::TUTORIAL:
		
		ColOBB(TYTORIAL);
		if (PlayerControl::GetInstance()->GetPlayer()->GetNowAttack()) {
			for (int i = 0; i < EnemyOBB.size(); i++) {
				if (Collision::CheckOBBCollision(HandObb, EnemyOBB[0])==true && !Hit_colf) {
					EnemyControl::GetInstance()->GetTutorialEnemyindex()[0]->RecvDamage(damage);
					Hit_colf = true;
				} 
			}
		}
		else {
			Hit_colf = false;
		}

		break;

	case SceneManager::PLAY:
		
		break;

	case SceneManager::BOSS:
		ColOBB(BOSS);
		if (PlayerControl::GetInstance()->GetPlayer()->GetNowAttack()) {
			for (int i = 0; i < BossEnemyOBB.size(); i++) {
				if (Collision::CheckOBBCollision(HandObb, BossEnemyOBB[0])==true && !Hit_colf2) {
					EnemyControl::GetInstance()->GetBossEnemyindex()[0]->RecvDamage(damage);
					Hit_colf2 = true;
				}
			}
		} else {
			Hit_colf2 = false;
		}
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
		EnemyOBB[0].SetOBBParam_Pos(EnemyControl::GetInstance()->GetTutorialEnemyindex()[0]->GetPosition());
		EnemyOBB[0].SetOBBParam_Rot(EnemyControl::GetInstance()->GetTutorialEnemyindex()[0]->GetMatrot());
		EnemyOBB[0].SetOBBParam_Scl({ 4.0f,2.0f,4.0f });
		break;
	case AttackCollision::PLAY://大量のOBB判定　軽量化必要かも
	
		break;
	case AttackCollision::BOSS:
		BossEnemyOBB.resize(1);
		//OBB 回転ベクトル
		BossEnemyOBB[0].SetOBBParam_Pos(EnemyControl::GetInstance()->GetBossEnemyindex()[0]->GetPosition());
		BossEnemyOBB[0].SetOBBParam_Rot(EnemyControl::GetInstance()->GetBossEnemyindex()[0]->GetMatrot());
		BossEnemyOBB[0].SetOBBParam_Scl({ 4.0f,2.0f,4.0f });

		break;
	default:
		break;
	}
}