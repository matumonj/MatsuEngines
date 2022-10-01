#include "ChestControl.h"
#include"SceneManager.h"
#include"Destroy.h"
#include"Feed.h"
#include"PlayerControl.h"
#include"CameraControl.h"
#include"UI.h"
ChestControl* ChestControl::GetInstance()
{
	static ChestControl instance;

	return&instance;
}

void ChestControl::Finalize()
{
	//Destroy(particleMan);
	Num.clear();
	pos.clear();
	Tutorial_chest.clear();
	chests.clear();
	Load_ChestPosition.clear();
}

void ChestControl::Load(DebugCamera* camera)
{
	if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY) {
	//ChestMaxも追加
	file.open("Param_CSV/Chest.csv");

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
	
	Load_ChestPosition.resize(Quantity);

	for (int i = 0; i < Quantity; i++) {

			//chests[i]->SetPosition(pos[i]);
	}
}
if (SceneManager::GetInstance()->GetScene() == SceneManager::TUTORIAL) {
	Tutorial_chest.resize(1);
	Tutorial_chest[0] = std::make_unique<Chest>();
	Tutorial_chest[0]->Initialize(camera);
	Tutorial_chest[0]->SetPosition({ 115,-24,-576 });
	Tutorial_chest[0]->SetRotation({ 0,90,0 });
	//Tutorial_chest[0]->SetScale({ 8.39,10,4 });
}

if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY) {
	for (int i = 0; i < 4; i++) {
		chests[i] = std::make_unique<Chest>();
		chests[i]->Initialize(camera);
		chests[i]->SetRotation({ 0,90,0 });
		//Tutorial_chest[0]->SetScale({ 8.39,10,4 });
	}
	chests[0]->SetPosition({ 57,-20,-293 });
	chests[1]->SetPosition({ 117,-20,-293 });
	chests[2]->SetPosition({ 57,-20,-233 });
	chests[3]->SetPosition({ 117,-20,-233 });

	for (int i = 0; i < 4; i++) {
		PlayPchest[i].particleMan = ParticleManager::Create(4, L"Resources/ParticleTex/normal.png");
		PlayPchest[i].ChestEvent = NON;
	}
}
UpdateRange = 200;
ParticleManager::LoadTexture(8, L"Resources/ParticleTex/normal.png");
TutorialPchest.particleMan = ParticleManager::Create(8, L"Resources/ParticleTex/normal.png");
TutorialPchest.ChestEvent = NON;//particleMan->CreateModel();

}

void ChestControl::Initialize(DebugCamera* camera)
{
	chests.resize(4);
}

void ChestControl::Update(DebugCamera* camera)
{
	if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY) {
		for (int i = 0; i <4; i++) {
			if (chests[i] != nullptr) {
				chests[i]->SetColor({ 1,1,1,1 });
				chests[i]->Update(camera);
				GetChestEvent(chests[i].get(), PlayPchest[i]);
			}
		}
		if (chests[0] != nullptr) {
			chests[0]->SetpColor({ 1.0f,0.1f,0.1f,0.7f });
		}
		if (chests[1] != nullptr) {
			chests[1]->SetpColor({ 0.1f,1.0f,0.1f,0.7f });
		}
		if (chests[2] != nullptr) {
			chests[2]->SetpColor({ 0.1f,0.1f,1.0f,0.7f });
		}
		if (chests[3] != nullptr) {
			chests[3]->SetpColor({ 1.0f,0.2f,0.7f,0.7f });
		}


	}
	if (SceneManager::GetInstance()->GetScene() == SceneManager::TUTORIAL) {
		if (Tutorial_chest[0] != nullptr) {
			Tutorial_chest[0]->SetColor({ 1,1,1,1 });
			Tutorial_chest[0]->SetpColor({ 1,1,1,1 });
			Tutorial_chest[0]->Update(camera);
			GetChestEvent(Tutorial_chest[0].get(), TutorialPchest);

			//	Destroy_unique(Tutorial_chest[0]);
			
		}
	}

}

void ChestControl::Draw()
{
	
	// 3Dオブジェクト描画前処理
	ParticleManager::PreDraw();
	// 3Dオブクジェクトの描画
	TutorialPchest.particleMan->Draw();

	// 3Dオブジェクト描画後処理
	ParticleManager::PostDraw();

	if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY) {
		for (int i = 0; i < 4; i++) {
			PlayPchest[i].particleMan->Draw();
		}
		for (int i = 0; i < 4; i++) {
			if (chests[i] != nullptr) {
				//	if (Collision::GetLength(PlayerControl::GetInstance()->GetPlayer()->GetPosition(), chests[i]->GetPosition()) < UpdateRange) {
				chests[i]->Draw();
			}
		}
	}
	if (SceneManager::GetInstance()->GetScene() == SceneManager::TUTORIAL) {
		if (Tutorial_chest[0] != nullptr) {
			Tutorial_chest[0]->Draw();
		}
	}
}


void ChestControl::GetChestAction()
{
	bool GetMaxChests = GetChestCount == Quantity + 1;
	GetChestCount++;

	ChestDestroy();
	
	
	if (GetMaxChests) {
		//BOSS登場シーンへに切り替え
	}
}

void ChestControl::ChestDestroy()
{
	//破棄処理　終わったらnullptr
}

void ChestControl::GetChestEvent(Chest* chest, ParticleParam& pParam)
{
	XMFLOAT3 Ppos = PlayerControl::GetInstance()->GetPlayer()->GetPosition();
	if (pParam.ChestEvent == NON && Collision::GetLength(Ppos, chest->GetPosition()) < 20.0f) {

		UI::GetInstance()->SetTurnoffUIDraw(true);
		pParam.ChestEvent = FEEDIN;
	}
	if (pParam.ChestEvent == FEEDIN) {
		Feed::GetInstance()->Update_Black(Feed::FEEDIN);
		if (Feed::GetInstance()->GetAlpha() >= 1.0f) {
			pParam.ChestEvent = FEEDOUT;
		}
	} else if (pParam.ChestEvent == FEEDOUT) {
		pParam.pCount++;
		if (pParam.pCount == 100) {
			pParam.ParticleCreateF = true;
		}

		if (pParam.pCount >= 100 + pParam.ParticleLife) {
			chest->SetChestLost(true);
			pParam.pCount = 0;
			pParam.ChestEvent = GETCHEST;
		}
		Feed::GetInstance()->Update_Black(Feed::FEEDOUT);
		PlayerControl::GetInstance()->GetPlayer()->SetPosition({ chest->GetPosition().x,Ppos.y,chest->GetPosition().z - 25.0f });
		PlayerControl::GetInstance()->GetPlayer()->SetStopFlag(true);
		CameraControl::GetInstance()->GetCamera()->SetEye({ Ppos.x + 8.0f,Ppos.y + 10.0f,Ppos.z - 20.0f });
		CameraControl::GetInstance()->GetCamera()->SetTarget(chest->GetPosition());
	} else if (pParam.ChestEvent == GETCHEST) {
		pParam.pCount++;
		if (pParam.pCount == 60) {
			pParam.pCount = 0;
			pParam.ChestEvent = FEEDIN2;
		}
		PlayerControl::GetInstance()->GetPlayer()->SetPosition({ chest->GetPosition().x,Ppos.y,chest->GetPosition().z - 25.0f });
		CameraControl::GetInstance()->GetCamera()->SetEye({ Ppos.x + 8.0f,Ppos.y + 10.0f,Ppos.z - 20.0f });
		CameraControl::GetInstance()->GetCamera()->SetTarget(chest->GetPosition());

	} else if (pParam.ChestEvent == FEEDIN2) {
		Feed::GetInstance()->Update_Black(Feed::FEEDIN);
		if (Feed::GetInstance()->GetAlpha() >= 1.0f) {
			pParam.pCount++;
			if (pParam.pCount > 60) {
				pParam.ChestEvent = FEEDOUT2;
			}
		}
		PlayerControl::GetInstance()->GetPlayer()->SetPosition({ chest->GetPosition().x,Ppos.y,chest->GetPosition().z - 25.0f });
		CameraControl::GetInstance()->GetCamera()->SetEye({ Ppos.x + 8.0f,Ppos.y + 10.0f,Ppos.z - 20.0f });
		CameraControl::GetInstance()->GetCamera()->SetTarget(chest->GetPosition());

	} else if (pParam.ChestEvent == FEEDOUT2) {
		Feed::GetInstance()->Update_Black(Feed::FEEDOUT);
		if (Feed::GetInstance()->GetAlpha() <= 0.0f) {
			pParam.pCount = 0;
			GetChestCount++;
			PlayerControl::GetInstance()->GetPlayer()->SetStopFlag(false);

			UI::GetInstance()->SetTurnoffUIDraw(false);
			pParam.ChestEvent = END;
		}
	}
	GetChestEffect(chest, pParam);
}

void ChestControl::GetChestEffect(Chest* chest, ParticleParam& pParam)
{

	for (int i = 0; i < 30; i++) {
		const float rnd_vel = 0.5f;
		pParam.vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		pParam.vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		pParam.vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		const float rnd_acc = 0.001f;
		pParam.acc.y = -(float)rand() / RAND_MAX * rnd_acc;

		//追加
		if (pParam.ParticleCreateF) {
			pParam.particlePos = { chest->GetPosition() };
			pParam.particleMan->Add(pParam.ParticleLife, pParam.particlePos, pParam.vel, pParam.acc, 3.0f, 0.0f);
			if (i == 30 - 1) {
				pParam.ParticleCreateF = false;
			}
		}
	}

	pParam.particleMan->SetColor({ 1.0f,0.8f,0.2f,0.8f });
	pParam.particleMan->Update(pParam.particleMan->FOLLOW, PlayerControl::GetInstance()->GetPlayer()->GetPosition(), 120.0f);
}