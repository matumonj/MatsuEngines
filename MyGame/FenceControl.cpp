#include "FenceControl.h"

#include "CameraControl.h"
#include "EnemyControl.h"
#include"SceneManager.h"
#include"TutorialSprite.h"
#include"PlayerControl.h"
#include"Enemy.h"

FenceControl* FenceControl::GetIns()
{
	static FenceControl instance;
	return &instance;
}

void FenceControl::Init_Tutorial()
{
	Tutorialfence.resize(1);
	Tutorialfence[0] = std::make_unique<AreaFence>();
	Tutorialfence[0]->Initialize();
	Tutorialfence[0]->SetPosition({58.0f, -40.0f, -300.0f});
	Tutorialfence[0]->SetRotation({0.0f, 0.0f, 0.0f});
	Tutorialfence[0]->SetScale({3.39f, 7.0f, 4.0f});
	BossGate = std::make_unique<AreaFence>();
	BossGate->Initialize();
	BossGate->SetPosition({17.0f, -35.0f, 402.0f});
	BossGate->SetRotation({0.0f, 0.0f, 0.0f});
	BossGate->SetScale({8.39f, 10.0f, 4.0f});
}

void FenceControl::Init_Play()
{
	file.open("Param_CSV/fence.csv");

	popcom << file.rdbuf();

	file.close();

	while (std::getline(popcom, line))
	{
		std::istringstream line_stream(line);
		std::string word;
		std::getline(line_stream, word, ',');

		if (word.find("//") == 0)
		{
			continue;
		}
		if (word.find("Fence_Quantity") == 0)
		{
			std::getline(line_stream, word, ',');
			int quantity = static_cast<int>(std::atof(word.c_str()));
			Quantity = quantity;
			break;
		}
	}
	Num.resize(Quantity);
	pos.resize(Quantity);
	rot.resize(Quantity);
	scl.resize(Quantity);
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
			if (word.find("POP") == 0)
			{
				std::getline(line_stream, word, ',');
				float x = static_cast<float>(std::atof(word.c_str()));

				std::getline(line_stream, word, ',');
				float y = static_cast<float>(std::atof(word.c_str()));

				std::getline(line_stream, word, ',');
				float z = static_cast<float>(std::atof(word.c_str()));

				pos[i] = {x, y, z};
				//break;
			}
			if (word.find("ROTATION") == 0)
			{
				std::getline(line_stream, word, ',');
				float x = static_cast<float>(std::atof(word.c_str()));

				std::getline(line_stream, word, ',');
				float y = static_cast<float>(std::atof(word.c_str()));

				std::getline(line_stream, word, ',');
				float z = static_cast<float>(std::atof(word.c_str()));

				rot[i] = {x, y, z};
				//break;
			}
			if (word.find("SCALE") == 0)
			{
				std::getline(line_stream, word, ',');
				float x = static_cast<float>(std::atof(word.c_str()));

				std::getline(line_stream, word, ',');
				float y = static_cast<float>(std::atof(word.c_str()));

				std::getline(line_stream, word, ',');
				float z = static_cast<float>(std::atof(word.c_str()));

				scl[i] = {x, y, z};
				break;
			}
		}
	}
	fences.resize(Quantity);

	Load_FencePosition.resize(Quantity);

	for (int i = 0; i < Quantity; i++)
	{
		fences[i] = std::make_unique<AreaFence>();
		fences[i]->Initialize();
		fences[i]->SetPosition(pos[i]);
		fences[i]->SetRotation(rot[i]);
		fences[i]->SetScale(scl[i]);
	}
}

void FenceControl::Init_Boss()
{
}

/*------------------------*/
/*--------解放処理---------*/
/*------------------------*/
void FenceControl::Finalize()
{
	Tutorialfence.clear();
	fences.clear();
	Num.clear();
	pos.clear();
	rot.clear();
	scl.clear();
}


/*------------------------*/
/*--------読み込み処理---------*/
/*-----------csv---------*/
void FenceControl::Load()
{
}


/*------------------------*/
/*--------更新処理---------*/
/*------------------------*/
void FenceControl::Update_Tutorial() //チュートリアル時
{
	//チュートリアルエリアの柵が開く条件
	if (EnemyControl::GetIns()->GetEnemy(EnemyControl::TUTORIAL)[0] != nullptr)
	{
		TutorialFenceOpen = TutorialSprite::GetIns()->GetClearSetting();
	}
	if (Tutorialfence[0] != nullptr)
	{
		Tutorialfence[0]->Update();
		Tutorialfence[0]->SetColors({1.0f, 0.0f, 0.0f, 1.0f});
	}
	Tutorialfence[0]->FenceOpenCondition(TutorialFenceOpen);
}

void FenceControl::Update_Play() //プレイシーン時
{
	BossGateOpen = Task::GetIns()->GetAllTaskClear() == TRUE &&
		CameraControl::GetIns()->GetMoveBosAreaCam() == CameraControl::TARGETPLAYER;

	for (int i = 0; i < Quantity; i++)
	{
		if (fences[i] != nullptr)
		{
			fences[i]->Update();
		}
	}
	//チュートリアルエリアの柵が開く条件

	if (BossGate != nullptr)
	{
		BossGate->Update();
		BossGate->SetColors({1.0f, 0.0f, 0.0f, 1.0f});
	}
	BossGate->FenceOpenCondition(BossGateOpen);
}

void FenceControl::Update_Boss()
{
}

/*------------------------*/
/*--------描画処理---------*/
/*------------------------*/
void FenceControl::Draw_Play()
{
	for (int i = 0; i < Quantity; i++)
	{
		if (fences[i] != nullptr)
		{
			fences[i]->Draw();
		}
	}
	if (BossGate != nullptr)
	{
		BossGate->Draw();
	}
}

void FenceControl::Draw_Tutorial()
{
	if (Tutorialfence[0] != nullptr)
	{
		Tutorialfence[0]->Draw();
	}
}

void FenceControl::Draw_Boss()
{
}
