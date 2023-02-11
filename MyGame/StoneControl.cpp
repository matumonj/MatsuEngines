#include "StoneControl.h"
#include"SceneManager.h"
#include"PlayerControl.h"
#include"mHelper.h"

StoneControl* StoneControl::GetIns()
{
	static StoneControl instance;

	return &instance;
}

StoneControl::~StoneControl()
{
	Tutorialstones.clear();
	stones.clear();
	Num.clear();
	pos.clear();
}

/*------------------------*/
/*--------âï˙èàóù---------*/
/*------------------------*/
void StoneControl::Finalize()
{
	Tutorialstones.clear();
	stones.clear();
	Num.clear();
	pos.clear();
}

/*------------------------*/
/*--------ì«çûèàóù---------*/
/*---------csv-----------*/

void StoneControl::Init_Tutorial()
{
	file.open("Param_CSV/stone_tutorial.csv");

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
		if (word.find("Stone_Quantity") == 0)
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

				pos[i] = {x, y - 2.f, z};
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
	//stones.resize(Quantity);

	//Load_StonePosition.resize(Quantity);

	//for (int i = 0; i < Quantity; i++) {

	//	stones[i] = std::make_unique<Stone>();

	//	stones[i]->Initialize(camera);
	//	stones[i]->SetPosition(pos[i]);
	//}
	Load_StonePosition.resize(Quantity);
	Tutorialstones.resize(Quantity);
	for (int i = 0; i < Tutorialstones.size(); i++)
	{
		Tutorialstones[i] = std::make_unique<Stone_A>();
		Tutorialstones[i]->Initialize();
		Tutorialstones[i]->SetPosition(pos[i]);
		Tutorialstones[i]->SetRotation(rot[i]);
		Tutorialstones[i]->SetScale(scl[i]);

		Load_StonePosition[i] = pos[i];
	}
}


void StoneControl::Init_Play()
{
	file.open("Param_CSV/stone.csv");

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
		if (word.find("Stone_Quantity") == 0)
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

				pos[i] = {x, y, z};
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

	//Load_StonePosition.resize(Quantity);
	Load_StonePosition.resize(Quantity);
	for (int i = 0; i < Quantity; i++)
	{
		stones[i] = std::make_unique<Stone>();
		stones[i]->Initialize();
		stones[i]->SetPosition(pos[i]);
		stones[i]->SetPosition(pos[i]);
		stones[i]->SetRotation(rot[i]);
		stones[i]->SetScale(scl[i]);
	}
}

void StoneControl::Init_Boss()
{
}

void StoneControl::Load()
{
	UpdateRange = 200;
}

/*------------------------*/
/*--------çXêVèàóù---------*/
/*------------------------*/
void StoneControl::Update_Tutorial()
{
	defpos.x = -52.f;
	defpos.z = 315.f;
	for (int i = 0; i < Tutorialstones.size(); i++)
	{
		Tutorialstones[i]->SetPosition({
			Load_StonePosition[i].x + defpos.x, Load_StonePosition[i].y + defpos.y,
			Load_StonePosition[i].z + defpos.z
		});
		Tutorialstones[i]->SetColors({1.0f, 1.0f, 1.0f, 1.0f});
		Tutorialstones[i]->Update();
	}
	for (int i = 0; i < Tutorialstones.size(); i++)
	{
		if (Tutorialstones[i]->CollideStone() == true)
		{
			PlayerControl::GetIns()->GetPlayer()->isOldPos();
			break;
			//	
		}
	}
}

void StoneControl::Update_Play()
{
	for (int i = 0; i < Quantity; i++)
	{
		if (stones[i] != nullptr)
		{
			//stones[i]->SetColor({ 0.0f,1.0f,0.0f,1.0f });
			stones[i]->Update();
		}
		if (stones[i]->CollideStone() == true)
		{
			PlayerControl::GetIns()->GetPlayer()->SetPosition(Player_OldPos);
			//break;
		}
	}
}

void StoneControl::Update_Boss()
{
}


/*------------------------*/
/*--------ï`âÊèàóù---------*/
/*------------------------*/
void StoneControl::Draw_Tutorial()
{
	for (int i = 0; i < Tutorialstones.size(); i++)
	{
		if (Tutorialstones[i] != nullptr)
		{
			Tutorialstones[i]->Draw();
		}
	}
	ImGui::Begin("Stonepos");
	ImGui::SliderFloat("x", &defpos.x, -100, 100);
	ImGui::SliderFloat("y", &defpos.y, -100, 100);
	ImGui::SliderFloat("z", &defpos.z, -500, 500);
	ImGui::End();
}

void StoneControl::Draw_Play()
{
	for (int i = 0; i < Quantity; i++)
	{
		if (stones[i] != nullptr)
		{
			//stones[i]->Draw();
		}
	}
}

void StoneControl::Draw_Boss()
{
}
