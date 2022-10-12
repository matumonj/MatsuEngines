#include"EnemyControl.h"
#include"MobEnemy.h"
#include"BossEnemy.h"
#include"EnemyAlpha.h"
#include"TutorialSprite.h"
#include"SistemConfig.h"
#include"SceneManager.h"

#include"KnockAttack.h"
#include"CircleAttack.h"
#include"HalfAttack.h"
#include"AltAttack.h"
#include"DebugTxt.h"
#include"FrontCircleAttack.h"
EnemyControl* EnemyControl::GetInstance()
{
	static EnemyControl instance;

	return&instance;
}

/*------------------------*/
/*--------解放処理---------*/
/*------------------------*/
void EnemyControl::Finalize()
{
	Num.clear();
	pos.clear();
	enemys.clear();
	Load_EnemyPosition.clear();
	boss_enemy.clear();
	tutorial_enemy.clear();
}

/*------------------------*/
/*--------読込処理---------*/
/*----------csv-----------*/
void EnemyControl::Initialize(DebugCamera* camera)
{
	boss_enemy.resize(1);
}

void EnemyControl::Load(DebugCamera* camera)
{
	if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY) {

		file.open("Param_CSV/enemy.csv");

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
				enemys[i] = std::make_unique<MobEnemy>();
			}
			if (Num[i] == BETAENEMY) {
				enemys[i] = std::make_unique<EnemyAlpha>();
			}

			enemys[i]->Initialize(camera);
			enemys[i]->SetPosition(pos[i]);
			enemys[i]->SetRespawnPos(pos[i]);
		}
	}
	if (SceneManager::GetInstance()->GetScene() == SceneManager::TUTORIAL) {
		tutorial_enemy.resize(1);
		tutorial_enemy[0] = std::make_unique<MobEnemy>();
		tutorial_enemy[0]->Initialize(camera);
		tutorial_pos = { 89.137f,20.5045f,-707.987f };
		tutorial_enemy[0]->SetPosition(tutorial_pos);
		tutorial_enemy[0]->SetRespawnPos(tutorial_pos);
	}
	if (SceneManager::GetInstance()->GetScene() == SceneManager::BOSS) {
		//boss_enemy.resize(1);
		boss_enemy[0] = std::make_unique<BossEnemy>();
		boss_enemy[0]->Initialize(camera);
		boss_pos = { 0.0f,15.5045f,20.987f };
		boss_enemy[0]->SetPosition(boss_pos);

		gigaboss = std::make_unique<GigaBossEnemy>();

		gigaboss->Initialize(camera);

		KnockAttack::GetInstance()->Initialize();
		CircleAttack::GetInstance()->Initialize();
		HalfAttack::GetInstance()->Initialize();
		AltAttack::GetInstance()->Initialize();
		FrontCircleAttack::GetInstance()->Initialize();
	}
}
/*------------------------*/
/*--------更新処理---------*/
/*------------------------*/
void EnemyControl::Update_Tutorial(DebugCamera* camera)
{
	if (tutorial_enemy[0] == nullptr)return;
		if (TutorialSprite::GetInstance()->GetClearSetting()) {
			tutorial_enemy[0]->SetMoveFlag(true);
			tutorial_enemy[0]->Update(camera);
		}
}
void EnemyControl::Update_Play(DebugCamera* camera)
{
	for (int i = 0; i < Quantity; i++) {
		if (enemys[i] != nullptr) {
			enemys[i]->SetMoveFlag(true);
			enemys[i]->Update(camera);
		}
	}
}
void EnemyControl::Update_Boss(DebugCamera* camera)
{
	if (boss_enemy[0] == nullptr) return;
		boss_enemy[0]->Update(camera);
		gigaboss->Update(camera);
}

/*------------------------*/
/*--------描画処理---------*/
/*------------------------*/
void EnemyControl::Draw()
{
	if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY) {
		for (int i = 0; i < Quantity; i++) {
			if (enemys[i] != nullptr) {
				enemys[i]->Draw();
			}
		}
	}
	if (SceneManager::GetInstance()->GetScene() == SceneManager::TUTORIAL) {
		if (tutorial_enemy[0] != nullptr) {
			if (TutorialSprite::GetInstance()->GetClearSetting()) {
				tutorial_enemy[0]->Draw();
			}
		}
	}
	if (SceneManager::GetInstance()->GetScene() == SceneManager::BOSS) {
		if (boss_enemy[0] != nullptr) {
			boss_enemy[0]->Draw();
			gigaboss->Draw();
		}
		CircleAttack::GetInstance()->Draw();
		HalfAttack::GetInstance()->Draw();
		KnockAttack::GetInstance()->Draw();
		AltAttack::GetInstance()->Draw();
		FrontCircleAttack::GetInstance()->Draw();

	}
}


std::vector<std::unique_ptr<Enemy>>& EnemyControl::GetEnemyindex(int index)
{
		return enemys;
}
std::vector<std::unique_ptr<Enemy>>& EnemyControl::GetTutorialEnemyindex()
{
	return tutorial_enemy;
}
std::vector<std::unique_ptr<Enemy>>& EnemyControl::GetBossEnemyindex()
{
	return boss_enemy;
}
std::unique_ptr<GigaBossEnemy>& EnemyControl::GetGigaBossEnemy()
{
	return gigaboss;
}
