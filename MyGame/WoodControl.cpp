#include "WoodControl.h"
#include"SceneManager.h"
#include"PlayerControl.h"
#include"mHelper.h"
#include "WoodB.h"

WoodControl* WoodControl::GetInstance()
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

void WoodControl::Init_Tutorial(DebugCamera* camera)
{
	Tutorialwoods.resize(3);
	Tutorialwoods[0] = std::make_unique<Wood>();
	Tutorialwoods[1] = std::make_unique<Wood>();
	Tutorialwoods[2] = std::make_unique<Wood>();
	for (int i = 0; i < Tutorialwoods.size(); i++)
	{
		Tutorialwoods[i]->Initialize(camera);
	}
	Tutorialwoods[0]->SetPosition({92.0f, -28.0f, -689.0f});
	Tutorialwoods[1]->SetPosition({70.0f, -28.0f, -700.0f});
	Tutorialwoods[2]->SetPosition({110.0f, -28.0f, -720.0f});
}

void WoodControl::Init_Play(DebugCamera* camera)
{
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

	for (int i = 0; i < Quantity; i++)
	{
		//èâä˙âªèàóù
		if (Num[i] == 1)
		{
			woods[i] = std::make_unique<Wood>();
		}
		if (Num[i] == 2)
		{
			woods[i] = std::make_unique<WoodB>();
		}
		woods[i]->Initialize(camera);
		woods[i]->SetPosition(pos[i]);
	}
}

void WoodControl::Init_Boss(DebugCamera* camera)
{
}

/*------------------------*/
/*--------âï˙èàóù---------*/
/*------------------------*/
void WoodControl::Finalize()
{
	Tutorialwoods.clear();
	woods.clear();
	Num.clear();
	pos.clear();
}

/*------------------------*/
/*--------ì«çûèàóù---------*/
/*---------csv-----------*/
void WoodControl::Load(DebugCamera* camera)
{
	UpdateRange = 200;
}


/*------------------------*/
/*--------çXêVèàóù---------*/
/*------------------------*/
void WoodControl::Update_Tutorial(DebugCamera* camera)
{
	for (int i = 0; i < Tutorialwoods.size(); i++)
	{
		Tutorialwoods[i]->SetColor({0.0f, 1.0f, 0.0f, 1.0f});
		Tutorialwoods[i]->Update(camera);
	}
}

void WoodControl::Update_Play(DebugCamera* camera)
{
	XMFLOAT3 pPos = PlayerControl::GetInstance()->GetPlayer()->GetPosition();

	for (int i = 0; i < Quantity; i++)
	{
		if (woods[i] == nullptr)
		{
			continue;
		}
		//woods[i]->SetColor({ 0.0f,1.0f,0.0f,1.0f });
		woods[i]->Update(camera);
	}
}

void WoodControl::Update_Boss(DebugCamera* camera)
{
}


/*------------------------*/
/*--------ï`âÊèàóù---------*/
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
	XMFLOAT3 pPos = PlayerControl::GetInstance()->GetPlayer()->GetPosition();
	for (int i = 0; i < Quantity; i++)
	{
		if (woods[i] != nullptr)
		{
			woods[i]->Draw();
		}
	}
}

void WoodControl::Draw_Boss()
{
}
