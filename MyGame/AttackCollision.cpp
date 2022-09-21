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
	BossColObb = new OBBCollision();
	ColObb = new OBBCollision();
	//PlayColOBB.resize()
	Play_colf.resize(EnemyControl::GetInstance()->GetEnemyindex(1).size());
}

void AttackCollision::Finalize()
{
	EnemyOBB.clear();
	BossEnemyOBB.clear();
	PlayColOBB.clear();
	Play_colf.clear();
	delete ColObb;
}
void AttackCollision::Update()
{
	PlayColOBB.resize(EnemyControl::GetInstance()->GetEnemyindex(1).size());
	Play_colf.resize(EnemyControl::GetInstance()->GetEnemyindex(1).size());

	PlayEnemyOBB.resize(EnemyControl::GetInstance()->GetEnemyindex(1).size());
	for (int i = 0; i < PlayColOBB.size(); i++) {
		//if (PlayColOBB[i] == nullptr) {
		PlayColOBB[i] = new OBBCollision();
		//}
	}



	//for (int i = 0; i < PlayEnemyOBB.size(); i++) {
		//OBB 回転ベクトル
	if(SceneManager::GetInstance()->GetScene()== SceneManager::PLAY) {
	
	}
}


void AttackCollision::GetCol(int damage)
{
	
	HandObb.m_Pos.m128_f32[0] = PlayerControl::GetInstance()->GetPlayer()->GetHanMat().r[3].m128_f32[0];// GetPosition().x;
	HandObb.m_Pos.m128_f32[1] = PlayerControl::GetInstance()->GetPlayer()->GetHanMat().r[3].m128_f32[1];
	HandObb.m_Pos.m128_f32[2] = PlayerControl::GetInstance()->GetPlayer()->GetHanMat().r[3].m128_f32[2];

	//今はプレイヤーの手に当たり判定　あと出修正
	HandObb.m_NormaDirect[0] = { PlayerControl::GetInstance()->GetPlayer()->GetMatrot().r[0].m128_f32[0],PlayerControl::GetInstance()->GetPlayer()->GetMatrot().r[0].m128_f32[1],PlayerControl::GetInstance()->GetPlayer()->GetMatrot().r[0].m128_f32[2] };
	HandObb.m_NormaDirect[1] = { PlayerControl::GetInstance()->GetPlayer()->GetMatrot().r[1].m128_f32[0], PlayerControl::GetInstance()->GetPlayer()->GetMatrot().r[1].m128_f32[1], PlayerControl::GetInstance()->GetPlayer()->GetMatrot().r[1].m128_f32[2] };
	HandObb.m_NormaDirect[2] = { PlayerControl::GetInstance()->GetPlayer()->GetMatrot().r[2].m128_f32[0], PlayerControl::GetInstance()->GetPlayer()->GetMatrot().r[2].m128_f32[1], PlayerControl::GetInstance()->GetPlayer()->GetMatrot().r[2].m128_f32[2] };
	HandObb.m_fLength[0] = 3;//x方向の長さ
	HandObb.m_fLength[1] = 3;//y方向の長さ
	HandObb.m_fLength[2] = 15;//z方向の長さ

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
		for (int i = 0; i < Play_colf.size(); i++) {
			if (EnemyControl::GetInstance()->GetEnemyindex(0)[i] != nullptr) {
				PlayEnemyOBB[i].m_NormaDirect[0] = { EnemyControl::GetInstance()->GetEnemyindex(0)[i]->GetMatrot().r[0].m128_f32[0],EnemyControl::GetInstance()->GetEnemyindex(0)[i]->GetMatrot().r[0].m128_f32[1],EnemyControl::GetInstance()->GetEnemyindex(0)[i]->GetMatrot().r[0].m128_f32[2] };
				PlayEnemyOBB[i].m_NormaDirect[1] = { EnemyControl::GetInstance()->GetEnemyindex(0)[i]->GetMatrot().r[1].m128_f32[0], EnemyControl::GetInstance()->GetEnemyindex(0)[i]->GetMatrot().r[1].m128_f32[1], EnemyControl::GetInstance()->GetEnemyindex(0)[i]->GetMatrot().r[1].m128_f32[2] };
				PlayEnemyOBB[i].m_NormaDirect[2] = { EnemyControl::GetInstance()->GetEnemyindex(0)[i]->GetMatrot().r[2].m128_f32[0], EnemyControl::GetInstance()->GetEnemyindex(0)[i]->GetMatrot().r[2].m128_f32[1], EnemyControl::GetInstance()->GetEnemyindex(0)[i]->GetMatrot().r[2].m128_f32[2] };
				PlayEnemyOBB[i].m_fLength[0] = 2;//x方向の長さ
				PlayEnemyOBB[i].m_fLength[1] = 2;//y方向の長さ
				PlayEnemyOBB[i].m_fLength[2] = 2;//z方向の長さ

				PlayEnemyOBB[i].m_Pos.m128_f32[0] = { EnemyControl::GetInstance()->GetEnemyindex(0)[i]->GetPosition().x };
				PlayEnemyOBB[i].m_Pos.m128_f32[1] = { EnemyControl::GetInstance()->GetEnemyindex(0)[i]->GetPosition().y };
				PlayEnemyOBB[i].m_Pos.m128_f32[2] = { EnemyControl::GetInstance()->GetEnemyindex(0)[i]->GetPosition().z };

				if (PlayColOBB[i]->ColOBBs(HandObb, PlayEnemyOBB[i])) {
					EnemyControl::GetInstance()->GetEnemyindex(1)[i]->RecvDamage(10);

					Play_colf[i] = true;
					//break;
				//	break;
				} else {
					Play_colf[i] = false;
				}
				if (Play_colf[i]) {
					//break;
				}
			}
		}
		break;

	case SceneManager::BOSS:
		if (boss_colf) {
			EnemyControl::GetInstance()->GetBossEnemyindex()[0]->RecvDamage(damage);
		}

		ColOBB(BOSS);

		for (int i = 0; i < BossEnemyOBB.size(); i++) {
			if (BossColObb->ColOBBs(HandObb, BossEnemyOBB[i])) {
				boss_colf = true;
				//break;
			} else {
				boss_colf = false;
			}
		}
		break;
	default:
		break;
	}
}
void AttackCollision::Draw()
{
	ImGui::Begin("size");
	ImGui::Text("%d", PlayEnemyOBB.size());
	ImGui::End();
}

void AttackCollision::ColOBB(ColType Enemytype)
{
	switch (Enemytype)
	{
	case AttackCollision::TYTORIAL:
		EnemyOBB.resize(1);

		//OBB 回転ベクトル
		EnemyOBB[0].m_NormaDirect[0] = {EnemyControl::GetInstance()->GetTutorialEnemyindex()[0]->GetMatrot().r[0].m128_f32[0],EnemyControl::GetInstance()->GetTutorialEnemyindex()[0]->GetMatrot().r[0].m128_f32[1],EnemyControl::GetInstance()->GetTutorialEnemyindex()[0]->GetMatrot().r[0].m128_f32[2]};
		EnemyOBB[0].m_NormaDirect[1] = { EnemyControl::GetInstance()->GetTutorialEnemyindex()[0]->GetMatrot().r[1].m128_f32[0], EnemyControl::GetInstance()->GetTutorialEnemyindex()[0]->GetMatrot().r[1].m128_f32[1], EnemyControl::GetInstance()->GetTutorialEnemyindex()[0]->GetMatrot().r[1].m128_f32[2] };
		EnemyOBB[0].m_NormaDirect[2] = { EnemyControl::GetInstance()->GetTutorialEnemyindex()[0]->GetMatrot().r[2].m128_f32[0], EnemyControl::GetInstance()->GetTutorialEnemyindex()[0]->GetMatrot().r[2].m128_f32[1], EnemyControl::GetInstance()->GetTutorialEnemyindex()[0]->GetMatrot().r[2].m128_f32[2] };
		EnemyOBB[0].m_fLength[0] = 2;//x方向の長さ
		EnemyOBB[0].m_fLength[1] = 2;//y方向の長さ
		EnemyOBB[0].m_fLength[2] = 2;//z方向の長さ

		EnemyOBB[0].m_Pos.m128_f32[0] = { EnemyControl::GetInstance()->GetTutorialEnemyindex()[0]->GetPosition().x };
		EnemyOBB[0].m_Pos.m128_f32[1] = { EnemyControl::GetInstance()->GetTutorialEnemyindex()[0]->GetPosition().y };
		EnemyOBB[0].m_Pos.m128_f32[2] = { EnemyControl::GetInstance()->GetTutorialEnemyindex()[0]->GetPosition().z };

		break;
	case AttackCollision::PLAY://大量のOBB判定　軽量化必要かも
	
		break;
	case AttackCollision::BOSS:
		BossEnemyOBB.resize(1);
		//OBB 回転ベクトル
		BossEnemyOBB[0].m_NormaDirect[0] = { EnemyControl::GetInstance()->GetBossEnemyindex()[0]->GetMatrot().r[0].m128_f32[0],EnemyControl::GetInstance()->GetBossEnemyindex()[0]->GetMatrot().r[0].m128_f32[1],EnemyControl::GetInstance()->GetBossEnemyindex()[0]->GetMatrot().r[0].m128_f32[2] };
		BossEnemyOBB[0].m_NormaDirect[1] = { EnemyControl::GetInstance()->GetBossEnemyindex()[0]->GetMatrot().r[1].m128_f32[0], EnemyControl::GetInstance()->GetBossEnemyindex()[0]->GetMatrot().r[1].m128_f32[1], EnemyControl::GetInstance()->GetBossEnemyindex()[0]->GetMatrot().r[1].m128_f32[2] };
		BossEnemyOBB[0].m_NormaDirect[2] = { EnemyControl::GetInstance()->GetBossEnemyindex()[0]->GetMatrot().r[2].m128_f32[0], EnemyControl::GetInstance()->GetBossEnemyindex()[0]->GetMatrot().r[2].m128_f32[1], EnemyControl::GetInstance()->GetBossEnemyindex()[0]->GetMatrot().r[2].m128_f32[2] };
		BossEnemyOBB[0].m_fLength[0] = 2;//x方向の長さ
		BossEnemyOBB[0].m_fLength[1] = 2;//y方向の長さ
		BossEnemyOBB[0].m_fLength[2] = 2;//z方向の長さ

		BossEnemyOBB[0].m_Pos.m128_f32[0] = { EnemyControl::GetInstance()->GetBossEnemyindex()[0]->GetPosition().x };
		BossEnemyOBB[0].m_Pos.m128_f32[1] = { EnemyControl::GetInstance()->GetBossEnemyindex()[0]->GetPosition().y };
		BossEnemyOBB[0].m_Pos.m128_f32[2] = { EnemyControl::GetInstance()->GetBossEnemyindex()[0]->GetPosition().z };


		break;
	default:
		break;
	}
}