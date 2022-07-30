#include "ChestControl.h"

ChestControl* ChestControl::GetInstance()
{
	static ChestControl instance;

	return& instance;
}
void ChestControl::Load(DebugCamera* camera)
{
	file.open("EnemyParam_CSV/Chest.csv");

	popcom << file.rdbuf();

	file.close();

	while (std::getline(popcom, line)) {
		std::istringstream line_stream(line);
		std::string word;
		std::getline(line_stream, word, ',');

		if (word.find("//") == 0) {
			continue;
		}
		if (word.find("Chest_Quantity") == 0) {
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
	chests.resize(Quantity);

	Load_ChestPosition.resize(Quantity);

	for (int i = 0; i < Quantity; i++) {

		chests[i] = std::make_unique<Chest>();

		chests[i]->Initialize(camera);
		chests[i]->SetPosition(pos[i]);
	}
	UpdateRange = 200;
}

void ChestControl::Initialize(DebugCamera* camera)
{

}

void ChestControl::Update(DebugCamera* camera)
{
	for (int i = 0; i < Quantity; i++) {
		if (chests[i] != nullptr) {
			if (Collision::GetLength(Player::GetInstance()->GetPosition(), chests[i]->GetPosition()) < UpdateRange) {
				chests[i]->Update(camera);
			}
		}
	}
	for (int i = 0; i < Quantity; i++) {
		if (chests[i]->CollideChest() == true) {
			//Player::GetInstance()->SetPosition(Player_OldPos);
			//Player::GetInstance()->SetGround(true);
			break;
		}
	}
}

void ChestControl::Draw()
{
	for (int i = 0; i < Quantity; i++) {
		if (chests[i] != nullptr) {
			if (Collision::GetLength(Player::GetInstance()->GetPosition(), chests[i]->GetPosition()) < UpdateRange) {
				chests[i]->Draw();
			}
		}
	}
}
