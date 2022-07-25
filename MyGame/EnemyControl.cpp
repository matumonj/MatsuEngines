#include"EnemyControl.h"
#include"MobEnemy.h"
#include"BossEnemy.h"
#include "WalkJudgement.h"
#include"WalkAction.h"
#include "FollowJudgement.h"
#include"FollowAction.h"
#include"StayJudgment.h"
#include"StayAction.h"
#include"EnemyAttackAction.h"
#include"SistemConfig.h"
#include"EnemyAttackJudgement.h"

EnemyControl* EnemyControl::GetInstance()
{
	static EnemyControl instance;

	return&instance;
}
void EnemyControl::Load(DebugCamera*camera)
{
	file.open("EnemyParam_CSV/open.csv");
	//file2.open("EnemyParam_CSV/wood.csv");

	popcom << file.rdbuf();

	//popcom2 << file2.rdbuf();

	file.close();
	//file2.close();
	//return oi;
	//fopen_s(&fp, "posxx.json", "r");
	/*流れとしては
	敵の数読み込み->
	読み込んだ敵の数分エネミーのパラメータ配列の要素数増やす->
	敵の数分作ったら配列の中身をロードしたものに->
	敵の番号が1だったらα,2だったらβでインスタンス生成、初期化
	*/
	//fread(&Enemy_Quantity, sizeof(int), 1, fp);
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
		enemys[i]->SetEnemyPosition(pos[i]);
		enemys[i]->SearchInit();
	}
}

void EnemyControl::Initialize(DebugCamera* camera)
{
	behavior.AddNode("", "Root", 0, BehaviorTree::SELECT_RULE::PRIORITY, NULL, NULL);
	behavior.AddNode("Root", "Attack", 1, BehaviorTree::SELECT_RULE::NON, EnemyAttackJudgement::GetInstance(), EnemyAttackAction::GetInstance());
	behavior.AddNode("Root", "Walk", 2, BehaviorTree::SELECT_RULE::NON, WalkJudgment::GetInstance(), WalkAction::GetInstance());
	behavior.AddNode("Root", "Follow", 3, BehaviorTree::SELECT_RULE::NON, FollowJudgement::GetInstance(), FollowAction::GetInstance());
}

void EnemyControl::Update(DebugCamera* camera)
{
	for (int i = 0; i < Quantity; i++) {
		if (enemys[i] != nullptr) {
			enemys[i]->SetMoveFlag(true);
			enemys[i]->Update({ 1,1,1,1 }, camera);
			enemys[i]->SearchAction(camera);
		}
	}
}

void EnemyControl::Draw()
{
	for (int i = 0; i < Quantity; i++) {
		if (enemys[i] != nullptr && enemys[i]->State_Dead() == false) {
				enemys[i]->Draw();
				enemys[i]->SearchDraw();
		}
	}
}

void EnemyControl::ImGuiDraw()
{

}

std::vector<std::unique_ptr<Enemy>> &EnemyControl::GetEnemyindex(int index)
{
	return enemys;
}
