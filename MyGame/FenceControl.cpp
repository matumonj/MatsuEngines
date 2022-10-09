#include "FenceControl.h"
#include "EnemyControl.h"
#include"SceneManager.h"
#include"TutorialSprite.h"
#include"PlayerControl.h"
#include"Enemy.h"
FenceControl* FenceControl::GetInstance()
{
	static FenceControl instance;
	return &instance;
}


/*------------------------*/
/*--------解放処理---------*/
/*------------------------*/
void FenceControl::Finalize()
{
	Tutorialfence.clear();
	fences.clear();
	Num.clear();
	pos.clear();
	rot.clear();
	scl.clear();
}


/*------------------------*/
/*--------読み込み処理---------*/
/*-----------csv---------*/
void FenceControl::Load(DebugCamera* camera)
{
	if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY) {
		file.open("Param_CSV/fence.csv");

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
				if (word.find("POP") == 0) {
					std::getline(line_stream, word, ',');
					float x = (float)std::atof(word.c_str());

					std::getline(line_stream, word, ',');
					float y = (float)std::atof(word.c_str());

					std::getline(line_stream, word, ',');
					float z = (float)std::atof(word.c_str());

					pos[i] = { x,y,z };
					//break;
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
		fences.resize(Quantity);

		Load_FencePosition.resize(Quantity);

		for (int i = 0; i < Quantity; i++) {
			fences[i] = std::make_unique<AreaFence>();
			fences[i]->Initialize(camera);
			fences[i]->SetPosition(pos[i]);
			fences[i]->SetRotation(rot[i]);
			fences[i]->SetScale(scl[i]);
		}
	}
	if (SceneManager::GetInstance()->GetScene() == SceneManager::TUTORIAL) {
		Tutorialfence.resize(1);
		Tutorialfence[0]= std::make_unique<AreaFence>();
		Tutorialfence[0]->Initialize(camera);
		Tutorialfence[0]->SetPosition({110.0f,-40.0f,-596.0f});
		Tutorialfence[0]->SetRotation({0.0f,0.0f,0.0f});
		Tutorialfence[0]->SetScale({8.39f,10.0f,4.0f});
	}
}

void FenceControl::Initialize(DebugCamera* camera)
{

}

/*------------------------*/
/*--------更新処理---------*/
/*------------------------*/
void FenceControl::Update_Tutorial(DebugCamera* camera)//チュートリアル時
{
	//チュートリアルエリアの柵が開く条件
	if (EnemyControl::GetInstance()->GetTutorialEnemyindex()[0] != nullptr) {
		TutorialFenceOpen = EnemyControl::GetInstance()->GetTutorialEnemyindex()[0]->GetHP() < 0;
	}
	if (Tutorialfence[0] != nullptr) {
		Tutorialfence[0]->Update(camera);
		Tutorialfence[0]->SetColor({ 1.0f,0.0f,0.0f,1.0f });
	}
	Tutorialfence[0]->FenceOpenCondition(TutorialFenceOpen);
}

void FenceControl::Update_Play(DebugCamera* camera)//プレイシーン時
{
	Player_OldPos = PlayerControl::GetInstance()->GetPlayer()->GetPosition();
	for (int i = 0; i < Quantity; i++) {
		if (fences[i] != nullptr) {
			fences[i]->Update(camera);
		}
	}
}
void FenceControl::Update_Boss(DebugCamera* camera)
{

}
void FenceControl::Update(DebugCamera* camera)
{
	if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY) {
		Update_Play(camera);
	}
	if (SceneManager::GetInstance()->GetScene() == SceneManager::TUTORIAL) {
		Update_Tutorial(camera);
	}
}


/*------------------------*/
/*--------描画処理---------*/
/*------------------------*/
void FenceControl::Draw()
{
	if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY) {

		for (int i = 0; i < Quantity; i++) {
			if (fences[i] != nullptr) {
				fences[i]->Draw();
			}
		}
		}
	if (SceneManager::GetInstance()->GetScene() == SceneManager::TUTORIAL) {
		if (Tutorialfence[0] != nullptr) {
			Tutorialfence[0]->Draw();
		}
	}
}