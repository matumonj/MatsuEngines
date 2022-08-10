#include"EnemyControl.h"
#include"MobEnemy.h"
#include"BossEnemy.h"
#include "WalkJudgement.h"
#include"WalkAction.h"
#include "FollowJudgement.h"
#include"FollowAction.h"

#include"DeathJudgment.h"
#include"DeathAction.h"
#include"EnemyAttackAction.h"
#include"SistemConfig.h"
#include"EnemyAttackJudgement.h"
#include"SceneManager.h"
EnemyControl* EnemyControl::GetInstance()
{
	static EnemyControl instance;

	return&instance;
}
void EnemyControl::Load(DebugCamera*camera)
{
	if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY) {

		file.open("EnemyParam_CSV/open.csv");

		popcom << file.rdbuf();

		file.close();
		/*流れとしては
		敵の数読み込み->
		読み込んだ敵の数分エネミーのパラメータ配列の要素数増やす->
		敵の数分作ったら配列の中身をロードしたものに->
		敵の番号が1だったらα,2だったらβでインスタンス生成、初期化
		*/
		while (std::getline(popcom, line)) {
			std::istringstream line_stream(line);
			std::string word;
			std::getline(line_stream, word, ',');

			if (word.find("//") == 0) {
				continue;
			}
			if (word.find("Enemy_Quantity") == 0) {
				std::getline(line_stream, word, ',');
				int quantity = (int)std::atof(word.c_str());
				Quantity = quantity;
				break;
			}
		}
		Num.resize(Quantity);
		pos.resize(Quantity);
		for (int i = 0; i < Quantity; i++) {
			while (std::getline(popcom, line)) {
				std::istringstream line_stream(line);
				std::string word;
				std::getline(line_stream, word, ',');

				if (word.find("//") == 0) {
					continue;
				}
				if (word.find("Number") == 0) {
					std::getline(line_stream, word, ',');
					int number = (int)std::atof(word.c_str());
					Num[i] = number;
				} else if (word.find("POP") == 0) {
					std::getline(line_stream, word, ',');
					float x = (float)std::atof(word.c_str());

					std::getline(line_stream, word, ',');
					float y = (float)std::atof(word.c_str());

					std::getline(line_stream, word, ',');
					float z = (float)std::atof(word.c_str());

					pos[i] = { x,y,z };
					break;
				}
			}
		}
		enemys.resize(Quantity);

		Load_EnemyPosition.resize(Quantity);

		for (int i = 0; i < Quantity; i++) {

			//初期化処理
			if (Num[i] == ALPHAENEMY) {
				enemys[i] = std::make_unique<MobEnemy>(&behavior, 100.0f, 100.0f, 30.0f, 10.0f);
			}
			if (Num[i] == BETAENEMY) {
				enemys[i] = std::make_unique<BossEnemy>(&behavior, 100.0f, 100.0f, 30.0f, 10.0f);
			}

			enemys[i]->Initialize(camera);
			enemys[i]->SetPosition(pos[i]);
			enemys[i]->SearchInit();
		}
	}
	if (SceneManager::GetInstance()->GetScene() == SceneManager::TUTORIAL) {
		tutorial_enemy.resize(1);
		Quantity = 1;
		tutorial_enemy[0] = std::make_unique<MobEnemy>(&behavior, 100.0f, 100.0f, 30.0f, 10.0f);
		tutorial_enemy[0]->Initialize(camera);
		tutorial_pos = { 89.137,-27.5045,-707.987 };
		tutorial_enemy[0]->SetPosition(tutorial_pos);
		tutorial_enemy[0]->SearchInit();
	}
}

void EnemyControl::Initialize(DebugCamera* camera)
{
	behavior.AddNode("", "Root", 0, BehaviorTree::SELECT_RULE::PRIORITY, NULL, NULL);
	behavior.AddNode("Root", "Walk", 1, BehaviorTree::SELECT_RULE::NON, WalkJudgment::GetInstance(), WalkAction::GetInstance());
//	
	behavior.AddNode("Root", "Attack", 2, BehaviorTree::SELECT_RULE::NON, EnemyAttackJudgement::GetInstance(), EnemyAttackAction::GetInstance());
	behavior.AddNode("Root", "Death", 3, BehaviorTree::SELECT_RULE::NON, DeathJudgment::GetInstance(), DeathAction::GetInstance());
	
//	behavior.AddNode("Root", "Follow", 3, BehaviorTree::SELECT_RULE::NON, FollowJudgement::GetInstance(), FollowAction::GetInstance());
}

void EnemyControl::Update(DebugCamera* camera)
{
	if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY) {
		for (int i = 0; i < Quantity; i++) {
			if (enemys[i] != nullptr) {
				enemys[i]->SetMoveFlag(true);
				enemys[i]->Update(camera);
				enemys[i]->SearchAction(camera);
			}
		}
	}
	if (SceneManager::GetInstance()->GetScene() == SceneManager::TUTORIAL) {
		if (tutorial_enemy[0] != nullptr) {
			tutorial_enemy[0]->SetMoveFlag(true);
			tutorial_enemy[0]->Update(camera);
			tutorial_enemy[0]->SearchAction(camera);
		}
	}
}

void EnemyControl::Draw()
{
	if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY) {
		for (int i = 0; i < Quantity; i++) {
			if (enemys[i] != nullptr) {
				enemys[i]->Draw();
			//	enemys[i]->SearchDraw();
			}
		}
	}
	if (SceneManager::GetInstance()->GetScene() == SceneManager::TUTORIAL) {
		if (tutorial_enemy[0] != nullptr) {
			tutorial_enemy[0]->Draw();
		}
	}
}

void EnemyControl::ImGuiDraw()
{

}

std::vector<std::unique_ptr<Enemy>>& EnemyControl::GetEnemyindex(int index)
{
	if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY) {
		return enemys;
	} else if (SceneManager::GetInstance()->GetScene() == SceneManager::TUTORIAL) {
		return tutorial_enemy;
	}
}