#include "WoodControl.h"
#include"SceneManager.h"
#include"PlayerControl.h"
#include"mHelper.h"
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
	if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY) {
		file.open("Param_CSV/wood.csv");

		popcom << file.rdbuf();

		file.close();

		while (std::getline(popcom, line)) {
			std::istringstream line_stream(line);
			std::string word;
			std::getline(line_stream, word, ',');

			if (word.find("//") == 0) {
				continue;
			}
			if (word.find("Wood_Quantity") == 0) {
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
				if (word.find("POP") == 0) {
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
		woods.resize(Quantity);

		Load_WoodPosition.resize(Quantity);

		for (int i = 0; i < Quantity; i++) {

			woods[i] = std::make_unique<Wood>();

			woods[i]->Initialize(camera);
			woods[i]->SetPosition(pos[i]);
		}
	}
		if (SceneManager::GetInstance()->GetScene() == SceneManager::TUTORIAL) {
			Tutorialwoods.resize(3);
			Tutorialwoods[0] = std::make_unique<Wood>();
			Tutorialwoods[1] = std::make_unique<Wood>();
			Tutorialwoods[2] = std::make_unique<Wood>();
			for (int i = 0; i < Tutorialwoods.size(); i++) {
				Tutorialwoods[i]->Initialize(camera);
			}
			Tutorialwoods[0]->SetPosition({92.0f,-15.0f,-689.0f});
			Tutorialwoods[1]->SetPosition({ 70.0f,-15.0f,-700.0f });
			Tutorialwoods[2]->SetPosition({ 110.0f,-15.0f,-720.0f });
		}
	UpdateRange = 200;
}

void WoodControl::Initialize(DebugCamera* camera)
{

}

/*------------------------*/
/*--------çXêVèàóù---------*/
/*------------------------*/
void WoodControl::Update_Tutorial(DebugCamera*camera)
{
	for (int i = 0; i < Tutorialwoods.size(); i++) {
		Tutorialwoods[i]->SetColor({ 0.0f,1.0f,0.0f,1.0f });
		Tutorialwoods[i]->Update(camera);

	}
	for (int i = 0; i < Tutorialwoods.size(); i++) {
		if (Tutorialwoods[i]->CollideWood()==true) {
			PlayerControl::GetInstance()->GetPlayer()->isOldPos();
			break;
		//	
		}
	}
}

void WoodControl::Update_Play(DebugCamera* camera)
{
	for (int i = 0; i < Quantity; i++) {
		if (woods[i] != nullptr) {
			//woods[i]->SetColor({ 0.0f,1.0f,0.0f,1.0f });
			woods[i]->Update(camera);
		}
		if (woods[i]->CollideWood()==true) {
			PlayerControl::GetInstance()->GetPlayer()->SetPosition(Player_OldPos);
			break;
		}
	}
}

void WoodControl::Update_Boss(DebugCamera* camera)
{

	
}


/*------------------------*/
/*--------ï`âÊèàóù---------*/
/*------------------------*/
void WoodControl::Draw()
{
	if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY) {
		for (int i = 0; i < Quantity; i++) {
			if (woods[i] != nullptr) {
				woods[i]->Draw();
			}
		}
	}
	if (SceneManager::GetInstance()->GetScene() == SceneManager::TUTORIAL) {
		for (int i = 0; i < Tutorialwoods.size(); i++) {
			if (Tutorialwoods[i] != nullptr) {
				Tutorialwoods[i]->Draw();
			}
		}
	}
}