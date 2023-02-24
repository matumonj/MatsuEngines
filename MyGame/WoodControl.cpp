#include "WoodControl.h"
#include"PlayerControl.h"
#include"mHelper.h"
#include "WoodB.h"

WoodControl* WoodControl::GetIns()
{
	static WoodControl instance;

	return &instance;
}

WoodControl::~WoodControl()
{
	Tutorialwoods.clear();
	woods.clear();
	Num.clear();
	pos.clear();
}

void WoodControl::Init_Tutorial()
{
	//チュートリアルの木は5本
	Tutorialwoods.resize(WoodSize);

	//初期化
	for (int i = 0; i < Tutorialwoods.size(); i++)
	{
		Tutorialwoods[i] = std::make_unique<Wood>();
		Tutorialwoods[i]->Initialize();
	}

	Tutorialwoods[0]->SetPosition({42.0f, -28.0f, -389.0f});
	Tutorialwoods[1]->SetPosition({20.0f, -28.0f, -350.0f});
	Tutorialwoods[2]->SetPosition({70.0f, -28.0f, -340.0f});
	Tutorialwoods[3]->SetPosition({30.0f, -28.0f, -410.0f});
	Tutorialwoods[4]->SetPosition({70.0f, -28.0f, -390.0f});
}

void WoodControl::Init_Play()
{
	//CSV読み込み
	file.open("Param_CSV/wood.csv");

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
		if (word.find("Wood_Quantity") == 0)
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
			if (word.find("POP") == 0)
			{
				std::getline(line_stream, word, ',');
				float x = static_cast<float>(std::atof(word.c_str()));

				std::getline(line_stream, word, ',');
				float y = static_cast<float>(std::atof(word.c_str()));

				std::getline(line_stream, word, ',');
				float z = static_cast<float>(std::atof(word.c_str()));

				pos[i] = {x, y - 8, z};
				break;
			}
		}
	}
	woods.resize(Quantity);

	Load_WoodPosition.resize(Quantity);

	//木を2種類の中から番号ごとに割り振り
	for (int i = 0; i < Quantity; i++)
	{
		//初期化処理
		if (Num[i] == WOOD_A)
		{
			woods[i] = std::make_unique<Wood>();
		}
		if (Num[i] == WOOD_B)
		{
			woods[i] = std::make_unique<WoodB>();
		}
		woods[i]->Initialize();
		woods[i]->SetPosition(pos[i]);
	}
}

void WoodControl::Init_Boss()
{
}

/*------------------------*/
/*--------解放処理---------*/
/*------------------------*/
void WoodControl::Finalize()
{
	Tutorialwoods.clear();
	woods.clear();
	Num.clear();
	pos.clear();
}

/*------------------------*/
/*--------読込処理---------*/
/*---------csv-----------*/
void WoodControl::Load()
{
	UpdateRange = 200;
}


/*------------------------*/
/*--------更新処理---------*/
/*------------------------*/
void WoodControl::Update_Tutorial()
{
	for (int i = 0; i < Tutorialwoods.size(); i++)
	{
		Tutorialwoods[i]->SetColors({0.0f, 1.0f, 0.0f, 1.0f});
		Tutorialwoods[i]->Update();
	}
}

void WoodControl::Update_Play()
{
	XMFLOAT3 pPos = PlayerControl::GetIns()->GetPlayer()->GetPosition();

	for (int i = 0; i < Quantity; i++)
	{
		if (woods[i] == nullptr)
		{
			continue;
		}
		woods[i]->Update();
	}
}

void WoodControl::Update_Boss()
{
}


/*------------------------*/
/*--------描画処理---------*/
/*------------------------*/
void WoodControl::Draw_Tutorial()
{
	for (int i = 0; i < Tutorialwoods.size(); i++)
	{
		if (Tutorialwoods[i] != nullptr)
		{
			Tutorialwoods[i]->Draw();
		}
	}
}

void WoodControl::Draw_Play()
{
	XMFLOAT3 pPos = PlayerControl::GetIns()->GetPlayer()->GetPosition();
	for (int i = 0; i < Quantity; i++)
	{
		if (woods[i] != nullptr)
		{
			//一定以上はなれたら描画切る
			if (Collision::GetLength(pPos, woods[i]->GetPosition()) > 300.f)
			{
				continue;
			}
			woods[i]->Draw();
		}
	}
}

void WoodControl::Draw_Boss()
{
}
