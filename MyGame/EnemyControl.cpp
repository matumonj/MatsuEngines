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

	return &instance;
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

void EnemyControl::Init_Tutorial(DebugCamera* camera)
{
	enemys.resize(BOSS + 1);

	enemys[TUTORIAL].resize(1);
	enemys[TUTORIAL][0] = std::make_unique<MobEnemy>();
	enemys[TUTORIAL][0]->Initialize(camera);
	tutorial_pos = {100.137f, 20.5045f, -650.987f};
	enemys[TUTORIAL][0]->SetPosition(tutorial_pos);
	enemys[TUTORIAL][0]->SetRespawnPos(tutorial_pos);
}

void EnemyControl::Init_Play(DebugCamera* camera)
{
	enemys.resize(BOSS + 1);
	file.open("Param_CSV/enemy.csv");

	popcom << file.rdbuf();

	file.close();
	/*流れとしては
	敵の数読み込み->
	読み込んだ敵の数分エネミーのパラメータ配列の要素数増やす->
	敵の数分作ったら配列の中身をロードしたものに->
	敵の番号が1だったらα,2だったらβでインスタンス生成、初期化
	*/
	while (std::getline(popcom, line))
	{
		std::istringstream line_stream(line);
		std::string word;
		std::getline(line_stream, word, ',');

		if (word.find("//") == 0)
		{
			continue;
		}
		if (word.find("Enemy_Quantity") == 0)
		{
			std::getline(line_stream, word, ',');
			int quantity = static_cast<int>(std::atof(word.c_str()));
			Quantity = quantity;
			break;
		}
	}
	Num.resize(Quantity);
	pos.resize(Quantity);
	for (int i = 0; i < Quantity; i++)
	{
		while (std::getline(popcom, line))
		{
			std::istringstream line_stream(line);
			std::string word;
			std::getline(line_stream, word, ',');

			if (word.find("//") == 0)
			{
				continue;
			}
			if (word.find("Number") == 0)
			{
				std::getline(line_stream, word, ',');
				int number = static_cast<int>(std::atof(word.c_str()));
				Num[i] = number;
			}
			else if (word.find("POP") == 0)
			{
				std::getline(line_stream, word, ',');
				float x = static_cast<float>(std::atof(word.c_str()));

				std::getline(line_stream, word, ',');
				float y = static_cast<float>(std::atof(word.c_str()));

				std::getline(line_stream, word, ',');
				float z = static_cast<float>(std::atof(word.c_str()));

				pos[i] = {x, y, z};
				break;
			}
		}
	}
	enemys[PLAYSCENE].resize(Quantity);

	Load_EnemyPosition.resize(Quantity);

	for (int i = 0; i < Quantity; i++)
	{
		//初期化処理
		if (Num[i] == ALPHAENEMY)
		{
			enemys[PLAYSCENE][i] = std::make_unique<MobEnemy>();
		}
		if (Num[i] == BETAENEMY)
		{
			enemys[PLAYSCENE][i] = std::make_unique<EnemyAlpha>();
		}

		enemys[PLAYSCENE][i]->Initialize(camera);
		enemys[PLAYSCENE][i]->SetPosition(pos[i]);
		enemys[PLAYSCENE][i]->SetRespawnPos(pos[i]);
	}
}


void EnemyControl::Init_Boss(DebugCamera* camera)
{
	enemys[BOSS].push_back(std::make_unique<BossEnemy>());
	enemys[BOSS][0] = std::make_unique<BossEnemy>();
	enemys[BOSS][0]->Initialize(camera);
	boss_pos = {-1.0f, 10.0f, 20.987f};
	enemys[BOSS][0]->SetPosition(boss_pos);

	HalfAttack::GetInstance()->Initialize();
	KnockAttack::GetInstance()->Initialize();
	CircleAttack::GetInstance()->Initialize();
	AltAttack::GetInstance()->Initialize();
	FrontCircleAttack::GetInstance()->Initialize();
}

void EnemyControl::Load(DebugCamera* camera)
{
}

/*------------------------*/
/*--------更新処理---------*/
/*------------------------*/
void EnemyControl::Update_Tutorial(DebugCamera* camera)
{
	if (enemys[TUTORIAL][0] == nullptr)return;
	if (TutorialSprite::GetInstance()->GetClearMove())
	{
		//enemys[EnemyType::TUTORIAL][0]->SetMoveFlag(true);
		enemys[TUTORIAL][0]->Update(camera);
	}
	if (enemys[TUTORIAL][0]->GetObjAlpha() <= 0.0f)
	{
		Destroy_unique(enemys[TUTORIAL][0]);
	}
}

void EnemyControl::Update_Play(DebugCamera* camera)
{
	XMFLOAT3 pPos = PlayerControl::GetInstance()->GetPlayer()->GetPosition();

	for (int i = 0; i < Quantity; i++)
	{
		if (enemys[PLAYSCENE][i] == nullptr) continue;
		if (Collision::GetLength(pPos, enemys[PLAYSCENE][i]->GetPosition()) < 200)
		{
			enemys[PLAYSCENE][i]->SetMoveFlag(true);
			enemys[PLAYSCENE][i]->Update(camera);
		}
		if (enemys[PLAYSCENE][i]->GetObjAlpha() <= 0.0f)
		{
			Destroy_unique(enemys[PLAYSCENE][i]);
		}
	}
}

void EnemyControl::Update_Boss(DebugCamera* camera)
{
	if (enemys[BOSS][0] == nullptr) return;
	enemys[BOSS][0]->Update(camera);
	if (enemys[BOSS][0]->GetObjAlpha() <= 0)
	{
		Destroy_unique(enemys[BOSS][0]);
	}
}

/*------------------------*/
/*--------描画処理---------*/
/*------------------------*/
#include"imgui.h"

void EnemyControl::Draw_Tutorial()
{
	if (SceneManager::GetInstance()->GetScene() == SceneManager::TUTORIAL)
	{
		if (enemys[TUTORIAL][0] != nullptr)
		{
			if (TutorialSprite::GetInstance()->GetClearMove())
			{
				enemys[TUTORIAL][0]->Draw();
			}
		}
	}
}

void EnemyControl::Draw_Play()
{
	XMFLOAT3 pPos = PlayerControl::GetInstance()->GetPlayer()->GetPosition();
	for (int i = 0; i < Quantity; i++)
	{
		if (enemys[PLAYSCENE][i] != nullptr)
		{
			if (Collision::GetLength(pPos, enemys[PLAYSCENE][i]->GetPosition()) < 200)
			{
				enemys[PLAYSCENE][i]->Draw();
			}
		}
	}
}

void EnemyControl::Draw_Boss()
{
	if (enemys[BOSS][0] == nullptr)return;
	enemys[BOSS][0]->Draw();
	CircleAttack::GetInstance()->Draw();
	HalfAttack::GetInstance()->Draw();
	KnockAttack::GetInstance()->Draw();
	AltAttack::GetInstance()->Draw();
	FrontCircleAttack::GetInstance()->Draw();
}


std::vector<std::unique_ptr<Enemy>>& EnemyControl::GetEnemy(EnemyType type)
{
	if (type == TUTORIAL)
	{
		return enemys[TUTORIAL];
	}
	if (type == PLAYSCENE)
	{
		return enemys[PLAYSCENE];
	}
	if (type == BOSS)
	{
		return enemys[BOSS];
	}

	return enemys[NON];
}
