#include "FenceControl.h"
#include "EnemyControl.h"


FenceControl* FenceControl::GetInstance()
{
	static FenceControl instance;

	return &instance;
}
void FenceControl::LoadFences(DebugCamera* camera)
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
			Fence_Quantity = quantity;
			break;
		}
	}
	Fence_Num.resize(Fence_Quantity);
	fencepos.resize(Fence_Quantity);
	for (int i = 0; i < Fence_Quantity; i++) {
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

				fencepos[i] = { x,y,z };
				break;
			}
		}
	}
	fences.resize(Fence_Quantity);

	Load_FencePosition.resize(Fence_Quantity);

	for (int i = 0; i < Fence_Quantity; i++) {

		fences[i] = std::make_unique<AreaFence>();

		fences[i]->Initialize(camera);
		fences[i]->SetPosition(fencepos[i]);
	}
}

void FenceControl::Initialize(DebugCamera* camera)
{

}

void FenceControl::Update(DebugCamera* camera)
{
	Player_OldPos = Player::GetInstance()->GetPosition();
	for (int i = 0; i < Fence_Quantity; i++) {
		if (fences[i] != nullptr) {
			fences[i]->Update(camera);
		}
	}
	for (int i = 0; i < Fence_Quantity; i++) {
		/*if (fences[i]->CollideFence() == true) {
			Player::GetInstance()->SetPosition(Player_OldPos);
			Player::GetInstance()->SetGround(true);
			break;
		}*/
	}

		TutorialFenceOpen = EnemyControl::GetInstance()->GetEnemy(0)->GetHP() <= 0;
		fences[0]->FenceOpenCondition(TutorialFenceOpen);
}

void FenceControl::Draw()
{
	for (int i = 0; i < Fence_Quantity; i++) {
		if (fences[i] != nullptr) {
			fences[i]->Draw();
		}
	}
}

void FenceControl::ImGuiDraw()
{

}