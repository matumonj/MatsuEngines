#include "StoneControl.h"
#include"SceneManager.h"
#include"PlayerControl.h"
#include"mHelper.h"
StoneControl* StoneControl::GetInstance()
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
/*--------�������---------*/
/*------------------------*/
void StoneControl::Finalize()
{
	Tutorialstones.clear();
	stones.clear();
	Num.clear();
	pos.clear();
}

/*------------------------*/
/*--------�Ǎ�����---------*/
/*---------csv-----------*/
void StoneControl::Load(DebugCamera* camera)
{
	if (SceneManager::GetInstance()->GetScene() == SceneManager::TUTORIAL) {
		file.open("Param_CSV/stone.csv");

		popcom << file.rdbuf();

		file.close();

		while (std::getline(popcom, line)) {
			std::istringstream line_stream(line);
			std::string word;
			std::getline(line_stream, word, ',');

			if (word.find("//") == 0) {
				continue;
			}
			if (word.find("Stone_Quantity") == 0) {
				std::getline(line_stream, word, ',');
				int quantity = (int)std::atof(word.c_str());
				Quantity = quantity;
				break;
			}
		}
		Num.resize(Quantity);
		pos.resize(Quantity);
		rot.resize(Quantity);
		scl.resize(Quantity);
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
				}
				if (word.find("POP") == 0) {
					std::getline(line_stream, word, ',');
					float x = (float)std::atof(word.c_str());

					std::getline(line_stream, word, ',');
					float y = (float)std::atof(word.c_str());

					std::getline(line_stream, word, ',');
					float z = (float)std::atof(word.c_str());

					pos[i] = { x,y,z };
					
				}
				if (word.find("ROTATION") == 0) {
					std::getline(line_stream, word, ',');
					float x = (float)std::atof(word.c_str());

					std::getline(line_stream, word, ',');
					float y = (float)std::atof(word.c_str());

					std::getline(line_stream, word, ',');
					float z = (float)std::atof(word.c_str());

					rot[i] = { x,y,z };
					//break;
				}
				if (word.find("SCALE") == 0) {
					std::getline(line_stream, word, ',');
					float x = (float)std::atof(word.c_str());

					std::getline(line_stream, word, ',');
					float y = (float)std::atof(word.c_str());

					std::getline(line_stream, word, ',');
					float z = (float)std::atof(word.c_str());

					scl[i] = { x,y,z };
					break;
				}

			}
		}
		stones.resize(Quantity);

		Load_StonePosition.resize(Quantity);

		for (int i = 0; i < Quantity; i++) {

			stones[i] = std::make_unique<Stone>();

			stones[i]->Initialize(camera);
			stones[i]->SetPosition(pos[i]);

		}
	}
	if (SceneManager::GetInstance()->GetScene() == SceneManager::TUTORIAL) {
		Tutorialstones.resize(Quantity);
		for (int i = 0; i < Quantity; i++) {

			//����������


			for (int i = 0; i < Tutorialstones.size(); i++) {
			//	if (Num[i] == StoneType::BIG_A) {
					Tutorialstones[i] = std::make_unique<Stone_A>();
				//}
				//if (Num[i] == StoneType::BIG_B) {
				//	Tutorialstones[i] = std::make_unique<Stone_B>();
			//	}
				Tutorialstones[i]->Initialize(camera);
				Tutorialstones[i]->SetPosition(pos[i]);
				Tutorialstones[i]->SetRotation(rot[i]);
				Tutorialstones[i]->SetScale(scl[i]);
			}
		}
		}
	UpdateRange = 200;
}

void StoneControl::Initialize(DebugCamera* camera)
{

}

/*------------------------*/
/*--------�X�V����---------*/
/*------------------------*/
void StoneControl::Update_Tutorial(DebugCamera* camera)
{
	for (int i = 0; i < Tutorialstones.size(); i++) {
		Tutorialstones[i]->SetColor({ 1.0f,1.0f,1.0f,1.0f });
		Tutorialstones[i]->Update(camera);

	}
	for (int i = 0; i < Tutorialstones.size(); i++) {
		if (Tutorialstones[i]->CollideStone() == true) {
			PlayerControl::GetInstance()->GetPlayer()->isOldPos();
			break;
			//	
		}
	}
}

void StoneControl::Update_Play(DebugCamera* camera)
{
	for (int i = 0; i < Quantity; i++) {
		if (stones[i] != nullptr) {
			//stones[i]->SetColor({ 0.0f,1.0f,0.0f,1.0f });
			stones[i]->Update(camera);
		}
		if (stones[i]->CollideStone() == true) {
			PlayerControl::GetInstance()->GetPlayer()->SetPosition(Player_OldPos);
			break;
		}
	}
}

void StoneControl::Update_Boss(DebugCamera* camera)
{


}


/*------------------------*/
/*--------�`�揈��---------*/
/*------------------------*/
void StoneControl::Draw()
{
	if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY) {
		for (int i = 0; i < Quantity; i++) {
			if (stones[i] != nullptr) {
				stones[i]->Draw();
			}
		}
	}
	if (SceneManager::GetInstance()->GetScene() == SceneManager::TUTORIAL) {
		for (int i = 0; i < Tutorialstones.size(); i++) {
			if (Tutorialstones[i] != nullptr) {
				Tutorialstones[i]->Draw();
			}
		}
	}
}