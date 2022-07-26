#include "FenceControl.h"
#include "EnemyControl.h"

FenceControl* FenceControl::GetInstance()
{
	static FenceControl instance;
	return &instance;
}
void FenceControl::Load(DebugCamera* camera)
{
	file.open("EnemyParam_CSV/fence.csv");

	//popcom << file.rdbuf();

	popcom << file.rdbuf();

	file.close();
	
	while (std::getline(popcom, line)) {
		std::istringstream line_stream(line);
		std::string word;
		std::getline(line_stream, word, ',');

		if (word.find("//") == 0) {
			continue;
		}
		if (word.find("Fence_Quantity") == 0) {
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
	fences.resize(Quantity);

	Load_FencePosition.resize(Quantity);

	for (int i = 0; i < Quantity; i++) {

		fences[i] = std::make_unique<AreaFence>();

		fences[i]->Initialize(camera);
		fences[i]->SetPosition(pos[i]);
	}
}

void FenceControl::Initialize(DebugCamera* camera)
{

}

void FenceControl::Update(DebugCamera* camera)
{
	Player_OldPos = Player::GetInstance()->GetPosition();
	for (int i = 0; i < Quantity; i++) {
		if (fences[i] != nullptr) {
			fences[i]->Update(camera);
		}
	}
	for (int i = 0; i < Quantity; i++) {
		/*if (fences[i]->CollideFence() == true) {
			Player::GetInstance()->SetPosition(Player_OldPos);
			Player::GetInstance()->SetGround(true);
			break;
		}*/
	}

		TutorialFenceOpen = EnemyControl::GetInstance()->GetEnemyindex(0)[TYUTORIAL]->GetHP() <= 1000;
		fences[TYUTORIAL]->FenceOpenCondition(TutorialFenceOpen);
}

void FenceControl::Draw()
{
	for (int i = 0; i <Quantity; i++) {
		if (fences[i] != nullptr) {
			fences[i]->Draw();
		}
	}
}

void FenceControl::ImGuiDraw()
{

}