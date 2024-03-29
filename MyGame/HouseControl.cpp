﻿#include "HouseControl.h"
#include "CameraControl.h"
#include"SceneManager.h"
#include"Enemy.h"

HouseControl* HouseControl::GetIns()
{
	static HouseControl instance;
	return &instance;
}

void HouseControl::Init_Tutorial()
{
}

void HouseControl::Init_Play()
{
	file.open("Param_CSV/house.csv");

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
		if (word.find("house_Quantity") == 0)
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
	houses.resize(Quantity);

	Load_HousePosition.resize(Quantity);

	for (int i = 0; i < Quantity; i++)
	{
		houses[i] = std::make_unique<House_A>();
		houses[i]->Initialize();
		houses[i]->SetPosition(pos[i]);
		houses[i]->SetRotation(rot[i]);
		//houses[i]->SetScale(scl[i]);
	}
}

void HouseControl::Init_Boss()
{
}

/*------------------------*/
/*--------解放処理---------*/
/*------------------------*/
void HouseControl::Finalize()
{
	houses.clear();
	Num.clear();
	pos.clear();
	rot.clear();
	scl.clear();
}


/*------------------------*/
/*--------読み込み処理---------*/
/*-----------csv---------*/
void HouseControl::Load()
{
}


/*------------------------*/
/*--------更新処理---------*/
/*------------------------*/
void HouseControl::Update_Tutorial() //チュートリアル時
{
}

void HouseControl::Update_Play() //プレイシーン時
{
	for (int i = 0; i < Quantity; i++)
	{
		if (houses[i] != nullptr)
		{
			houses[i]->Update();
		}
	}
	//チュートリアルエリアの柵が開く条件
}

void HouseControl::Update_Boss()
{
}

/*------------------------*/
/*--------描画処理---------*/
/*------------------------*/
void HouseControl::Draw_Play()
{
	for (int i = 0; i < Quantity; i++)
	{
		if (houses[i] != nullptr)
		{
			houses[i]->Draw();
		}
	}
}

void HouseControl::Draw_Tutorial()
{
}

void HouseControl::Draw_Boss()
{
}
