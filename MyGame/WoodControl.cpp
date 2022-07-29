#include "WoodControl.h"
WoodControl* WoodControl::GetInstance()
{
	static WoodControl instance;

	return &instance;
}

WoodControl::~WoodControl()
{
	
}

void WoodControl::Load(DebugCamera* camera)
{
	file.open("EnemyParam_CSV/wood.csv");

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

void WoodControl::Initialize(DebugCamera* camera)
{

}

void WoodControl::Update(DebugCamera* camera)
{
	Player_OldPos= Player::GetInstance()->GetPosition();
	for (int i = 0; i < Quantity; i++) {
		if (woods[i] != nullptr) {
			woods[i]->Update(camera);
		}
	}
	for (int i = 0; i < Quantity; i++) {
		if (woods[i]->CollideWood() == true) {
			Player::GetInstance()->SetPosition(Player_OldPos);
			Player::GetInstance()->SetGround(true);
			break;
		}
	}
}

void WoodControl::Draw()
{
	for (int i = 0; i < Quantity; i++) {
		if (woods[i] != nullptr) {
			woods[i]->Draw();
		}
	}
}

void WoodControl::ImGuiDraw()
{

}