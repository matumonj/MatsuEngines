#include "ChestControl.h"
#include"SceneManager.h"
#include"Destroy.h"
#include"Feed.h"
#include"PlayerControl.h"
#include"CameraControl.h"
ChestControl* ChestControl::GetInstance()
{
	static ChestControl instance;

	return&instance;
}

void ChestControl::Finalize()
{
	Destroy(particleMan);
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
	chests.resize(Quantity);

	Load_ChestPosition.resize(Quantity);

	for (int i = 0; i < Quantity; i++) {

		chests[i] = std::make_unique<Chest>();

		chests[i]->Initialize(camera);
		chests[i]->SetPosition(pos[i]);
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
UpdateRange = 200;
ParticleManager::LoadTexture(4, L"Resources/ParticleTex/normal.png");
particleMan= ParticleManager::Create(4,L"Resources/ParticleTex/normal.png");
ChestEvent = NON;
//particleMan->CreateModel();
}

void ChestControl::Initialize(DebugCamera* camera)
{

}

void ChestControl::Update(DebugCamera* camera)
{
	if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY) {
		for (int i = 0; i < Quantity; i++) {
			if (chests[i] != nullptr) {
				chests[i]->SetColor({ 1,1,1,1 });
				chests[i]->Update(camera);
				GetChestEvent(chests[i].get());
			}
		}
	}
	if (SceneManager::GetInstance()->GetScene() == SceneManager::TUTORIAL) {
		if (Tutorial_chest[0] != nullptr) {
			Tutorial_chest[0]->SetColor({ 1,1,1,1 });
			Tutorial_chest[0]->Update(camera);
			//	Destroy_unique(Tutorial_chest[0]);
			GetChestEvent(Tutorial_chest[0].get());
		}
	}

}

void ChestControl::Draw()
{
	// 3Dオブジェクト描画前処理
	ParticleManager::PreDraw();
	// 3Dオブクジェクトの描画
	particleMan->Draw();
	// 3Dオブジェクト描画後処理
	ParticleManager::PostDraw();

	if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY) {
		for (int i = 0; i < Quantity; i++) {
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

void ChestControl::GetChestEvent(Chest* chest)
{
	XMFLOAT3 Ppos = PlayerControl::GetInstance()->GetPlayer()->GetPosition();
	if (ChestEvent == NON&& Collision::GetLength(Ppos, chest->GetPosition()) < 5.0f) {
		ChestEvent = FEEDIN;
	}
	if (ChestEvent==FEEDIN) {
		Feed::GetInstance()->Update_Black(Feed::FEEDIN);
		if (Feed::GetInstance()->GetAlpha() >= 1.0f) {
			ChestEvent = FEEDOUT;
		}
	}
	else if (ChestEvent == FEEDOUT) {
			pCount++;
		if (pCount == 100) {
			ParticleCreateF = true;
		}

		if (pCount >= 100 + ParticleLife) {
			Tutorial_chest[0]->SetChestLost(true);
			pCount = 0;
			ChestEvent = GETCHEST;
		}
		Feed::GetInstance()->Update_Black(Feed::FEEDOUT);
		PlayerControl::GetInstance()->GetPlayer()->SetPosition({ chest->GetPosition().x,Ppos.y,chest->GetPosition().z - 25.0f });
		CameraControl::GetInstance()->GetCamera()->SetEye({Ppos.x+8.0f,Ppos.y+10.0f,Ppos.z-20.0f});
		CameraControl::GetInstance()->GetCamera()->SetTarget(chest->GetPosition());
	}
	else if (ChestEvent ==GETCHEST) {
		pCount++;
		if (pCount == 60) {
			pCount = 0;
			ChestEvent = FEEDIN2;
		}
		PlayerControl::GetInstance()->GetPlayer()->SetPosition({ chest->GetPosition().x,Ppos.y,chest->GetPosition().z - 25.0f });
		CameraControl::GetInstance()->GetCamera()->SetEye({ Ppos.x + 8.0f,Ppos.y + 10.0f,Ppos.z - 20.0f });
		CameraControl::GetInstance()->GetCamera()->SetTarget(chest->GetPosition());

	}
	else if (ChestEvent == FEEDIN2) {
		Feed::GetInstance()->Update_Black(Feed::FEEDIN);
		if (Feed::GetInstance()->GetAlpha() >= 1.0f) {
			pCount++;
			if (pCount > 60) {
				ChestEvent = FEEDOUT2;
			}
		}
		PlayerControl::GetInstance()->GetPlayer()->SetPosition({chest->GetPosition().x,Ppos.y,chest->GetPosition().z - 25.0f });
		CameraControl::GetInstance()->GetCamera()->SetEye({ Ppos.x + 8.0f,Ppos.y + 10.0f,Ppos.z - 20.0f });
		CameraControl::GetInstance()->GetCamera()->SetTarget(chest->GetPosition());

	}
	else if (ChestEvent == FEEDOUT2) {
		Feed::GetInstance()->Update_Black(Feed::FEEDOUT);
		if (Feed::GetInstance()->GetAlpha() <= 0.0f) {
			pCount = 0;
			GetChestCount++;
			ChestEvent = END;
		}
	}
	GetChestEffect(chest);
}

void ChestControl::GetChestEffect(Chest* chest)
{
	Tutorial_chest[0]->SetpColor({ 1.0f,0.9f,0.0f,0.7f });
	const int ParticleSize = 30;
	for (int i = 0; i < ParticleSize; i++) {
		const float rnd_vel = 0.5f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		XMFLOAT3 acc{};
		const float rnd_acc = 0.001f;
		acc.y = -(float)rand() / RAND_MAX * rnd_acc;

		//追加
		if (ParticleCreateF) {
			particlePos = { Tutorial_chest[0]->GetPosition()};
			particleMan->Add(ParticleLife, particlePos, vel, acc, 3.0f, 0.0f);
			if (i == ParticleSize - 1) {
				ParticleCreateF = false;
			}
		}
		}

	particleMan->SetColor({ 1.0f,0.8f,1.0f,0.8f });
	particleMan->Update(particleMan->FOLLOW,PlayerControl::GetInstance()->GetPlayer()->GetPosition(),120.0f);
}