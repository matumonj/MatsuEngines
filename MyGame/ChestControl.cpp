#include "ChestControl.h"
#include"SceneManager.h"
#include"Destroy.h"
#include"Feed.h"
#include"PlayerControl.h"
#include"CameraControl.h"
#include"FootSwitch.h"
#include"UI.h"

ChestControl* ChestControl::GetIns()
{
	static ChestControl instance;

	return &instance;
}

/*------------------------*/
/*--------解放処理---------*/
/*------------------------*/
void ChestControl::Finalize()
{
	//Destroy(particleMan);
	Num.clear();
	pos.clear();
	Tutorial_chest.clear();
	chests.clear();
	Load_ChestPosition.clear();
}

/*------------------------*/
/*--------読込処理---------*/
/*----------csv-----------*/
void ChestControl::Init_Tutorial()
{
	Tutorial_chest.resize(1);
	Tutorial_chest[0] = std::make_unique<Chest>();
	Tutorial_chest[0]->Initialize();
	Tutorial_chest[0]->SetPosition({50.0f, -24.0f, -246.0f});
	Tutorial_chest[0]->SetRotation({0.0f, 90.0f, 0.0f});

	ParticleManager::LoadTexture(8, L"Resources/ParticleTex/normal.png");
	TutorialPchest.particleMan = ParticleManager::Create(8, L"Resources/ParticleTex/normal.png");
	TutorialPchest.ChestEvent = NON; //particleMan->CreateModel();
}

void ChestControl::Init_Play()
{
	//ChestMaxも追加
	file.open("Param_CSV/Chest.csv");

	popcom << file.rdbuf();

	file.close();

	while (std::getline(popcom, line))
	{
		std::istringstream line_stream(line);
		std::string word;
		std::getline(line_stream, word, ',');

		if (word.find("//") == 0)
		{
			continue;
		}
		if (word.find("Chest_Quantity") == 0)
		{
			std::getline(line_stream, word, ',');
			int quantity = static_cast<int>(std::atof(word.c_str()));
			Quantity = quantity;
			break;
		}
	}
	Num.resize(Quantity);
	pos.resize(Quantity);
	for (int i = 0; i < Quantity; i++)
	{
		while (std::getline(popcom, line))
		{
			std::istringstream line_stream(line);
			std::string word;
			std::getline(line_stream, word, ',');

			if (word.find("//") == 0)
			{
				continue;
			}
			if (word.find("POP") == 0)
			{
				std::getline(line_stream, word, ',');
				float x = static_cast<float>(std::atof(word.c_str()));

				std::getline(line_stream, word, ',');
				float y = static_cast<float>(std::atof(word.c_str()));

				std::getline(line_stream, word, ',');
				float z = static_cast<float>(std::atof(word.c_str()));

				pos[i] = {x, y, z};
				break;
			}
		}
	}
	Load_ChestPosition.resize(Quantity);

	chests.resize(4);
	for (int i = 0; i < 4; i++)
	{
		chests[i] = std::make_unique<Chest>();
		chests[i]->Initialize();
		chests[i]->SetRotation({0.0f, 90.0f, 0.0f});
		chests[i]->SetPosition({990, 999, 0});
	}

	DebugCamera* l_camera = CameraControl::GetIns()->GetCamera();
	ChestCage = std::make_unique<Object3d>();
	ChestCage->Initialize(l_camera);
	ChestCage->SetModel(ModelManager::GetIns()->GetModel(ModelManager::CAGE));
	

	ParticleManager::LoadTexture(8, L"Resources/ParticleTex/normal.png");
	for (int i = 0; i < 4; i++)
	{
		PlayPchest[i].particleMan = ParticleManager::Create(8, L"Resources/ParticleTex/normal.png");
		PlayPchest[i].ChestEvent = NON;
	}
}

void ChestControl::Init_Boss()
{
}

void ChestControl::Load()
{
}


/*------------------------*/
/*--------更新処理---------*/
/*------------------------*/
void ChestControl::Update_Tutorial()
{
	if (Tutorial_chest[0] != nullptr)
	{
		Tutorial_chest[0]->SetColors({1.0f, 1.0f, 1.0f, 1.0f});
		Tutorial_chest[0]->SetpColor({1.0f, 1.0f, 1.0f, 1.0f});
		Tutorial_chest[0]->Update();
		GetChestEvent(Tutorial_chest[0].get(), TutorialPchest);

		if (TutorialPchest.ChestEvent == END)
		{
			PlayerControl::GetIns()->GetPlayer()->SetIdle(false);
			GetTutorialChestJudg = true;
		}
	}
}

void ChestControl::Update_Play()
{
	chests.resize(4);

	const XMFLOAT4 color_red = {1.0f, 0.1f, 0.1f, 0.7f};
	const XMFLOAT4 color_green = {0.1f, 1.0f, 0.1f, 0.7f};
	const XMFLOAT4 color_blue = {0.1f, 0.1f, 1.0f, 0.7f};
	const XMFLOAT4 color_yellow = {1.0f, 0.2f, 0.7f, 0.7f};

	for (int i = 0; i < 4; i++)
	{
		if (chests[i] != nullptr)
		{
			chests[i]->SetColors({1, 1, 1, 1});
			chests[i]->Update();
			GetChestEvent(chests[i].get(), PlayPchest[i]);
		}
	}

	if (chests[RED] != nullptr)
	{
		chests[RED]->SetpColor(color_red);
	}
	if (chests[GREEN] != nullptr)
	{
		chests[GREEN]->SetpColor(color_green);
	}
	if (chests[BLUE] != nullptr)
	{
		chests[BLUE]->SetPosition(FootSwitch::GetIns()->Switchs_CenterPos());
		chests[BLUE]->SetpColor(color_blue);

		ChestCage->SetPosition({ CagePos.x,CagePos.y-5.f,CagePos.z });
		ChestCage->SetColor({ 1.f,1.f,1.f,0.5f });
		ChestCage->SetUVf(true);
		ChestCage->SetScale({ 6.f,3.f,6.f });
		ChestCage->Update(CameraControl::GetIns()->GetCamera());
	}
	if (chests[YELLOW] != nullptr)
	{
		chests[YELLOW]->SetpColor(color_yellow);
	}

	if(FootSwitch::GetIns()->GetClearSwicthsQuantity()>=4)
	{
		CagePos.y -= 1.f;
	}
	else
	{
		CagePos = chests[BLUE]->GetPosition();
	}

	CageColPlayer();
}

void ChestControl::CageOBBSet()
{
	
}
void ChestControl::CageColPlayer()
{
	OBB playerOBB, CageOBB;
	playerOBB.SetOBBParam_Pos(PlayerControl::GetIns()->GetPlayer()->GetPosition());
	playerOBB.SetOBBParam_Rot(PlayerControl::GetIns()->GetPlayer()->GetMatrot());
	playerOBB.SetOBBParam_Scl({ 1.0f, 1.0f, 1.0f });

	//柵のOBB 回転ベクトル
	CageOBB.SetOBBParam_Pos(CagePos);
	CageOBB.SetOBBParam_Rot(ChestCage->GetMatrot());
	CageOBB.SetOBBParam_Scl({
		ChestCage->GetMatScl().r[0].m128_f32[0] * 5,
		ChestCage->GetMatScl().r[1].m128_f32[1] * 5,
		ChestCage->GetMatScl().r[2].m128_f32[2] *5
		});

	if (Collision::CheckOBBCollision(playerOBB, CageOBB) == true)
	{
		PlayerControl::GetIns()->GetPlayer()->isOldPos();
	}
}


void ChestControl::SetChestAppearance(Color color, XMFLOAT3 position)
{
	chests[color]->SetPosition(position);
}

void ChestControl::Update_Boss()
{
}

/*------------------------*/
/*--------描画処理---------*/
/*------------------------*/
void ChestControl::Draw_Tutorial()
{
	// 3Dオブジェクト描画前処理
	ParticleManager::PreDraw();
	// 3Dオブクジェクトの描画
	TutorialPchest.particleMan->Draw();
	// 3Dオブジェクト描画後処理
	ParticleManager::PostDraw();

	if (Tutorial_chest[0] != nullptr)
	{
		Tutorial_chest[0]->Draw();
	}
}

void ChestControl::Draw_Play()
{
	// 3Dオブジェクト描画前処理
	ParticleManager::PreDraw();
	for (int i = 0; i < 4; i++)
	{
		if (chests[i] != nullptr)
		{
			PlayPchest[i].particleMan->Draw();
		}
	}
	// 3Dオブジェクト描画後処理
	ParticleManager::PostDraw();

	for (int i = 0; i < 4; i++)
	{
		if (chests[i] != nullptr)
		{
			chests[i]->Draw();
		}
	}
	Object3d::PreDraw();
	ChestCage->Draw();
	Object3d::PostDraw();
}

void ChestControl::Draw_Boss()
{
}


void ChestControl::GetChestAction()
{
	bool GetMaxChests = GetChestCount == Quantity + 1;
	GetChestCount++;

	ChestDestroy();


	if (GetMaxChests)
	{
		//BOSS登場シーンへに切り替え
	}
}

void ChestControl::ChestDestroy()
{
	//破棄処理　終わったらnullptr
}

void ChestControl::GetChestEvent(Chest* chest, ParticleParam& pParam)
{
	XMFLOAT3 Ppos = PlayerControl::GetIns()->GetPlayer()->GetPosition();
	if (pParam.ChestEvent == NON && Collision::GetLength(Ppos, chest->GetPosition()) < 20.0f)
	{
		UI::GetIns()->SetTurnoffUIDraw(true);
		pParam.ChestEvent = FEEDIN;
	}
	if (pParam.ChestEvent == FEEDIN)
	{
		Feed::GetIns()->Update_Black(Feed::FEEDIN);
		if (Feed::GetIns()->GetAlpha() >= 1.0f)
		{
			pParam.ChestEvent = FEEDOUT;
		}
	}
	else if (pParam.ChestEvent == FEEDOUT)
	{
		if (Input::GetIns()->TriggerButton(Input::Y))
		{
			chest->SetChestLost(true);
			pParam.pCount = 0;
			pParam.ChestEvent = FEEDIN2;
		}
		pParam.pCount++;
		if (pParam.pCount == 100)
		{
			pParam.ParticleCreateF = true;
		}

		if (pParam.pCount >= 100 + pParam.ParticleLife)
		{
			chest->SetChestLost(true);
			pParam.pCount = 0;
			pParam.ChestEvent = GETCHEST;
		}
		Feed::GetIns()->Update_Black(Feed::FEEDOUT);
		PlayerControl::GetIns()->GetPlayer()->SetPosition({
			chest->GetPosition().x, Ppos.y, chest->GetPosition().z - 25.0f
		});
		PlayerControl::GetIns()->GetPlayer()->SetRotation({-90.0f, 0.0f, 0.0f});
		PlayerControl::GetIns()->GetPlayer()->SetStopFlag(true);
		CameraControl::GetIns()->GetCamera()->SetEye({Ppos.x + 8.0f, Ppos.y + 10.0f, Ppos.z - 20.0f});
		CameraControl::GetIns()->GetCamera()->SetTarget(chest->GetPosition());
	}
	else if (pParam.ChestEvent == GETCHEST)
	{
		if (Input::GetIns()->TriggerButton(Input::Y))
		{
			pParam.pCount = 0;
			pParam.ChestEvent = FEEDIN2;
		}
		pParam.pCount++;
		if (pParam.pCount == 60)
		{
			pParam.pCount = 0;
			pParam.ChestEvent = FEEDIN2;
		}
		PlayerControl::GetIns()->GetPlayer()->SetPosition({
			chest->GetPosition().x, Ppos.y, chest->GetPosition().z - 25.0f
		});
		CameraControl::GetIns()->GetCamera()->SetEye({Ppos.x + 8.0f, Ppos.y + 10.0f, Ppos.z - 20.0f});
		CameraControl::GetIns()->GetCamera()->SetTarget(chest->GetPosition());
	}
	else if (pParam.ChestEvent == FEEDIN2)
	{
		Feed::GetIns()->Update_Black(Feed::FEEDIN);
		if (Feed::GetIns()->GetAlpha() >= 1.0f)
		{
			pParam.pCount++;
			if (pParam.pCount > 60)
			{
				pParam.ChestEvent = FEEDOUT2;
			}
		}
		PlayerControl::GetIns()->GetPlayer()->SetPosition({
			chest->GetPosition().x, Ppos.y, chest->GetPosition().z - 25.0f
		});
		PlayerControl::GetIns()->GetPlayer()->SetIdle(true);
		CameraControl::GetIns()->GetCamera()->SetEye({Ppos.x + 8.0f, Ppos.y + 10.0f, Ppos.z - 20.0f});
		CameraControl::GetIns()->GetCamera()->SetTarget(chest->GetPosition());
	}
	else if (pParam.ChestEvent == FEEDOUT2)
	{
		Feed::GetIns()->Update_Black(Feed::FEEDOUT);
		if (Feed::GetIns()->GetAlpha() <= 0.0f)
		{
			pParam.pCount = 0;
			GetChestCount++;
			PlayerControl::GetIns()->GetPlayer()->SetStopFlag(false);

			UI::GetIns()->SetTurnoffUIDraw(false);
			pParam.ChestEvent = END;
		}
	}
	GetChestEffect(chest, pParam);
}

void ChestControl::GetChestEffect(Chest* chest, ParticleParam& pParam)
{
	for (int i = 0; i < 30; i++)
	{
		const float rnd_vel = 0.5f;
		pParam.vel.x = static_cast<float>(rand()) / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		pParam.vel.y = static_cast<float>(rand()) / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		pParam.vel.z = static_cast<float>(rand()) / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		const float rnd_acc = 0.001f;
		pParam.acc.y = -static_cast<float>(rand()) / RAND_MAX * rnd_acc;

		//追加
		if (pParam.ParticleCreateF)
		{
			pParam.particlePos = {chest->GetPosition()};
			pParam.particleMan->Add(pParam.ParticleLife, pParam.particlePos, pParam.vel, pParam.acc, 3.0f, 0.0f);
			if (i == 30 - 1)
			{
				pParam.ParticleCreateF = false;
			}
		}
	}

	pParam.particleMan->SetColor({1.0f, 0.8f, 0.2f, 0.8f});
	pParam.particleMan->Update(pParam.particleMan->FOLLOW, PlayerControl::GetIns()->GetPlayer()->GetPosition(),
	                           120);
}
