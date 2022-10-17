#include"EnemyControl.h"
#include"MobEnemy.h"
#include"BossEnemy.h"
#include"EnemyAlpha.h"
#include"TutorialSprite.h"
#include"SistemConfig.h"
#include"SceneManager.h"

#include"ChestControl.h"
#include"PlayerControl.h"
#include"CameraControl.h"
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
	
}

/*------------------------*/
/*--------読込処理---------*/
/*----------csv-----------*/
void EnemyControl::Initialize(DebugCamera* camera)
{
	enemys.resize(EnemyType::BOSS + 1);

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
		enemys[EnemyType::PLAYSCENE].resize(Quantity);

		Load_EnemyPosition.resize(Quantity);

		for (int i = 0; i < Quantity; i++) {

			//初期化処理
			if (Num[i] == ALPHAENEMY) {
				enemys[EnemyType::PLAYSCENE][i] = std::make_unique<MobEnemy>();
			}
			if (Num[i] == BETAENEMY) {
				enemys[EnemyType::PLAYSCENE][i] = std::make_unique<EnemyAlpha>();
			}
			
			enemys[EnemyType::PLAYSCENE][i]->Initialize(camera);
			enemys[EnemyType::PLAYSCENE][i]->SetPosition(pos[i]);
			enemys[EnemyType::PLAYSCENE][i]->SetRespawnPos(pos[i]);
		}
		GuardianSetPos();
	}

	if (SceneManager::GetInstance()->GetScene() == SceneManager::TUTORIAL) {
		enemys[EnemyType::TUTORIAL].resize(1);
		enemys[EnemyType::TUTORIAL][0] = std::make_unique<MobEnemy>();
		enemys[EnemyType::TUTORIAL][0]->Initialize(camera);
		tutorial_pos = { 89.137f,20.5045f,-707.987f };
		enemys[EnemyType::TUTORIAL][0]->SetPosition(tutorial_pos);
		enemys[EnemyType::TUTORIAL][0]->SetRespawnPos(tutorial_pos);
	}
	if (SceneManager::GetInstance()->GetScene() == SceneManager::BOSS) {
		enemys[EnemyType::BOSS].resize(1);
		enemys[EnemyType::BOSS][0] = std::make_unique<BossEnemy>();
		enemys[EnemyType::BOSS][0]->Initialize(camera);
		boss_pos = { 0.0f,15.5045f,20.987f };
		enemys[EnemyType::BOSS][0]->SetPosition(boss_pos);

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
	if (enemys[EnemyType::TUTORIAL][0] == nullptr)return;
		if (TutorialSprite::GetInstance()->GetClearSetting()) {
			enemys[EnemyType::TUTORIAL][0]->SetMoveFlag(true);
			enemys[EnemyType::TUTORIAL][0]->Update(camera);
		}
}
void EnemyControl::Update_Play(DebugCamera* camera)
{
	XMFLOAT3 pPos = PlayerControl::GetInstance()->GetPlayer()->GetPosition();

	for (int i = 0; i < Quantity; i++) {
		if (enemys[EnemyType::PLAYSCENE][i] != nullptr) {
			enemys[EnemyType::PLAYSCENE][i]->SetMoveFlag(true);
			enemys[EnemyType::PLAYSCENE][i]->Update(camera);
		}
	}
		if (CameraControl::GetInstance()->GetEncountFlag() == FALSE) {
			for (int i = GUARDIAN_RED; i < GUARDIAN_GREEN + 1; i++) {
				if (enemys[i][0]->GetHP() <= 0) {
					continue;
				}
				if (Collision::GetLength(pPos, enemys[i][0]->GetPosition()) < 50.0f) {
					CameraControl::GetInstance()->SetEncountGuardianFlag(TRUE);
				}
				enemys[i][0]->Update(camera);
			}
	}
}
void EnemyControl::Update_Boss(DebugCamera* camera)
{
	if (enemys[EnemyType::BOSS][0] == nullptr) return;
	enemys[EnemyType::BOSS][0]->Update(camera);
		gigaboss->Update(camera);
}

/*------------------------*/
/*--------描画処理---------*/
/*------------------------*/
void EnemyControl::Draw()
{
	if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY) {
		for (int i = 0; i < Quantity; i++) {
			if (enemys[EnemyType::PLAYSCENE][i] != nullptr) {
				enemys[EnemyType::PLAYSCENE][i]->Draw();
			}
		}
		for (int i = GUARDIAN_RED; i < GUARDIAN_GREEN + 1; i++) {
			enemys[i][0]->Draw();
		}
	}
	if (SceneManager::GetInstance()->GetScene() == SceneManager::TUTORIAL) {
		if (enemys[EnemyType::TUTORIAL][0] != nullptr) {
			if (TutorialSprite::GetInstance()->GetClearSetting()) {
				enemys[EnemyType::TUTORIAL][0]->Draw();
			}
		}
	}
	if (SceneManager::GetInstance()->GetScene() == SceneManager::BOSS) {
		if (enemys[EnemyType::BOSS][0] != nullptr) {
			enemys[EnemyType::BOSS][0]->Draw();
			gigaboss->Draw();
		}
		CircleAttack::GetInstance()->Draw();
		HalfAttack::GetInstance()->Draw();
		KnockAttack::GetInstance()->Draw();
		AltAttack::GetInstance()->Draw();
		FrontCircleAttack::GetInstance()->Draw();

	}
}

void EnemyControl::GuardianSetPos()
{
	enemys[GUARDIAN_RED][0]->Initialize(CameraControl::GetInstance()->GetCamera());
	enemys[GUARDIAN_RED][0]->SetPosition(ChestControl::GetInstance()->GetChest(ChestControl::RED)->GetPosition());
	
	enemys[GUARDIAN_GREEN][0]->Initialize(CameraControl::GetInstance()->GetCamera());
	enemys[GUARDIAN_GREEN][0]->SetPosition(ChestControl::GetInstance()->GetChest(ChestControl::GREEN)->GetPosition());
	
	enemys[GUARDIAN_BLUE][0]->Initialize(CameraControl::GetInstance()->GetCamera());
	enemys[GUARDIAN_BLUE][0]->SetPosition(ChestControl::GetInstance()->GetChest(ChestControl::BLUE)->GetPosition());
	
	enemys[GUARDIAN_YELLOW][0]->Initialize(CameraControl::GetInstance()->GetCamera());
	enemys[GUARDIAN_YELLOW][0]->SetPosition(ChestControl::GetInstance()->GetChest(ChestControl::YELLOW)->GetPosition());


}
std::vector<std::unique_ptr<Enemy>>&EnemyControl::GetEnemy(EnemyType type)
{
	switch (type)
	{
	case TUTORIAL:
		return enemys[EnemyType::TUTORIAL];
		break;
	case PLAYSCENE:
		return enemys[EnemyType::PLAYSCENE];
		break;
	case GUARDIAN_RED:
		return enemys[EnemyType::GUARDIAN_RED];
		break;
	case GUARDIAN_BLUE:
		return enemys[EnemyType::GUARDIAN_BLUE];
		break;
	case GUARDIAN_YELLOW:
		return enemys[EnemyType::GUARDIAN_YELLOW];
		break;
	case GUARDIAN_GREEN:
		return enemys[EnemyType::GUARDIAN_GREEN];
		break;
	case BOSS:
		return enemys[EnemyType::BOSS];
		break;
	default:
		break;
	}
}
