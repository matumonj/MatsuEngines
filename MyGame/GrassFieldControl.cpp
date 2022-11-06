#include "GrassFieldControl.h"
#include"SceneManager.h"
#include"PlayerControl.h"
#include"mHelper.h"
GrassFieldControl* GrassFieldControl::GetInstance()
{
	static GrassFieldControl instance;

	return &instance;
}

GrassFieldControl::~GrassFieldControl()
{
	Tutorialgrassfields.clear();
	grassfields.clear();
	Num.clear();
	pos.clear();
}
void GrassFieldControl::Init_Tutorial(DebugCamera* camera)
{
	file.open("Param_CSV/grass.csv");

	popcom << file.rdbuf();

	file.close();

	while (std::getline(popcom, line)) {
		std::istringstream line_stream(line);
		std::string word;
		std::getline(line_stream, word, ',');

		if (word.find("//") == 0) {
			continue;
		}
		if (word.find("Grass_Quantity") == 0) {
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
	grassfields.resize(Quantity);

	/*for (int i = 0; i < Quantity; i++) {
		grassfields[i] = std::make_unique<GrassField>();
		grassfields[i]->Initialize(camera);
		grassfields[i]->SetPosition(pos[i]);
	}*/
	Tutorialgrassfields.resize(Quantity);

	for (int i = 0; i < Tutorialgrassfields.size(); i++) {
		Tutorialgrassfields[i] = std::make_unique<GrassField>();
		Tutorialgrassfields[i]->Initialize(camera);
		Tutorialgrassfields[i]->SetPosition(pos[i]);
		Tutorialgrassfields[i]->SetRotation(rot[i]);
		Tutorialgrassfields[i]->SetScale(scl[i]);
	}
}

void GrassFieldControl::Init_Play(DebugCamera* camera)
{

}
void GrassFieldControl::Init_Boss(DebugCamera* camera)
{

}
/*------------------------*/
/*--------âï˙èàóù---------*/
/*------------------------*/
void GrassFieldControl::Finalize()
{
	Tutorialgrassfields.clear();
	grassfields.clear();
	Num.clear();
	pos.clear();
}

/*------------------------*/
/*--------ì«çûèàóù---------*/
/*---------csv-----------*/
void GrassFieldControl::Load(DebugCamera* camera)
{
	
	UpdateRange = 200;
}

/*------------------------*/
/*--------çXêVèàóù---------*/
/*------------------------*/
void GrassFieldControl::Update_Tutorial(DebugCamera* camera)
{
	for (int i = 0; i < Tutorialgrassfields.size(); i++) {
		//Tutorialgrassfields[i]->SetColor({ 1.0f,1.0f,1.0f,1.0f });
		Tutorialgrassfields[i]->Update(camera);

	}

}

void GrassFieldControl::Update_Play(DebugCamera* camera)
{
	for (int i = 0; i < Quantity; i++) {
		if (grassfields[i] != nullptr) {
			//grassfields[i]->SetColor({ 0.0f,1.0f,0.0f,1.0f });
			grassfields[i]->Update(camera);
		}
		
	}
}

void GrassFieldControl::Update_Boss(DebugCamera* camera)
{


}


/*------------------------*/
/*--------ï`âÊèàóù---------*/
/*------------------------*/
void GrassFieldControl::Draw_Tutorial()
{
	for (int i = 0; i < Tutorialgrassfields.size(); i++) {
		if (Tutorialgrassfields[i] != nullptr) {
			Tutorialgrassfields[i]->Draw();
		}
	}
}
void GrassFieldControl::Draw_Play()
{
	for (int i = 0; i < Quantity; i++) {
		if (grassfields[i] != nullptr) {
			grassfields[i]->Draw();
		}
	}
}
void GrassFieldControl::Draw_Boss()
{

}