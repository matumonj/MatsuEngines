#include "HUD.h"
#include"TargetMarker.h"
#include"mHelper.h"
#include"PlayerAttackState.h"
#include"Collision.h"
#include"Player.h"
#include"CameraControl.h"
#include"CustomButton.h"
#include"SceneManager.h"
#include"Input.h"
#include"CustomButton.h"
#include"SelectSword.h"
#include"PlayerControl.h"
#include"ChestControl.h"

HUD::~HUD()
{
	for (int i = 0; i < 5; i++)
	{
		delete ChestCollect[i];
	}
	delete PlayerHP, EnemyHP_Border, EnemyHP_Inner;
	for (int i = 0; i < EnemyHP_Border_Multi.size(); i++)
	{
		delete EnemyHP_Border_Multi[i], EnemyHP_Inner_Multi[i];
	}
}

void HUD::Finalize()
{
	for (int i = 0; i < 5; i++)
	{
		delete ChestCollect[i];
	}
	delete ChestCollectFrame;
	delete PlayerHP, EnemyHP_Border, EnemyHP_Inner;
	EnemyHP_Border_Multi.clear(), EnemyHP_Inner_Multi.clear();
}

HUD* HUD::GetInstance()
{
	static HUD instance;
	return &instance;
}

void HUD::Initialize()
{
	RecvDamageflag = false;
	Sprite::LoadTexture(143, L"Resources/quest.png");
	Sprite::LoadTexture(144, L"Resources/HPTex/HPgauge.png");
	Sprite::LoadTexture(145, L"Resources/HPTex/frame.png");
	Sprite::LoadTexture(146, L"Resources/HPTex/frame2.png");
	Sprite::LoadTexture(147, L"Resources/chestCollect1.png");
	Sprite::LoadTexture(148, L"Resources/chestColFrame.png");

	Sprite::LoadTexture(149, L"Resources/2d/minimapframe.png");

	TaskSprite = Sprite::Create(143, {0.0f, -200.0f});
	TaskSprite->SetPosition({WinApp::window_width / 2, WinApp::window_height / 2});
	TaskSprite->SetAnchorPoint({0.5, 0.5});
	PlayerHP = Sprite::Create(144, {0.0f, -200.0f});
	PlayerHP->SetPosition({70, 50});

	PlayerHPFrame = Sprite::Create(145, {0.0f, -200.0f});

	PlayerHPFrame2 = Sprite::Create(146, {0.0f, -200.0f});


	for (int i = 0; i < 5; i++)
	{
		ChestCollect[i] = Sprite::Create(147, {0.0f, -200.0f});
		ChestCollect[i]->SetPosition({static_cast<float>(i) * 100.0f + 70.0f, 100.0f});
		ChestCollect[i]->SetSize({100.0f, 100.0f});
	}
	ChestCollectFrame = Sprite::Create(148, {0.0f, -200.0f});
	ChestCollectFrame->SetPosition({70.0f, 100.0f});
	ChestCollectFrame->SetSize({500.0f, 100.0f});


	PlayerHPSize = static_cast<float>(PlayerControl::GetInstance()->GetPlayer()->GetMaxHP()) * 7.0f;

	MiniMapFrame = Sprite::Create(149, {0.0f, -200.0f});
	MiniMapFrame->SetAnchorPoint({0.5F, 0.5F});
	MiniFrameSize = {480, 500};
	MiniframePos = {1600, 200};
	MiniMapFrame->SetSize({MiniFrameSize});
	MiniMapFrame->SetPosition(MiniframePos);
}

void HUD::EnemyHPGaugeInitialize()
{
	Texture::LoadTexture(122, L"Resources/bosshp.png");
	Texture::LoadTexture(123, L"Resources/backright.png");
	Sprite::LoadTexture(12, L"Resources/bosshp.png");
	Sprite::LoadTexture(13, L"Resources/bosshp.png");
}

void HUD::SkillButtonInitialize()
{
	
	
}

void HUD::EnemyHPGaugeUpdate(std::vector<std::unique_ptr<Enemy>>& enemy)
{
}

void HUD::SetSkillIcon(SkillIcon icon)
{
	skillicon = icon;
	iconchangeF = true;
	if (iconchangeF)
	{
		SkillButtonInitialize();
		iconchangeF = false;
	}
}


void HUD::SkillBottonUpdate()
{

	//各剣のクールタイム参照
	TimeSpeed = 1.0f / SelectSword::GetInstance()->GetSword()->GetCoolTime();
	if (CoolTime_Time >= 1.0f)
	{
		if (CustomButton::GetInstance()->GetAttackAction() || CustomButton::GetInstance()->Get2AttackAction() ||
			CustomButton::GetInstance()->Get3AttackAction())
		{
			CoolTime_Time = 0.0f;
		}
	}
	else
	{
		CoolTime_Time += TimeSpeed;
	}
	
	
	if (RecvDamageflag)
	{
		easetime += 1.0f / 60.f;
		if (easetime >= 1.0f)
		{
			RecvDamageflag = false;
		}
		else
		{
			PlayerHPSize = Easing::EaseOut(easetime, OldPlayerHPSize,
			                               static_cast<float>(PlayerControl::GetInstance()->GetPlayer()->GetHP())) *
				7.0f;
		}
	}
	else
	{
		easetime = 0.0f;
		OldPlayerHPSize = static_cast<float>(PlayerControl::GetInstance()->GetPlayer()->GetHP());
	}
	PlayerHP->SetSize({PlayerHPSize, 50.0f});

	PlayerHPFrame->SetSize({static_cast<float>(PlayerControl::GetInstance()->GetPlayer()->GetMaxHP()) * 7.0f, 50.0f});

	PlayerHPFrame2->SetSize({static_cast<float>(PlayerControl::GetInstance()->GetPlayer()->GetMaxHP()) * 7.0f, 50.0f});

	PlayerHPFrame->SetPosition(PlayerHP->GetPosition());
	PlayerHPFrame2->SetPosition(PlayerHP->GetPosition());
}

void HUD::TaskUpdate(DebugCamera* camera)
{
	XMFLOAT3 cPos = camera->GetEye();
	if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY)
	{
		if (Collision::GetLength(cPos, {CameraControl::GetInstance()->CameraPointIndex(2)}) < 10.0f)
		{
			taskfeed = true;
		}
	}
	if (taskfeed)
	{
		if (taskSpriteSize.x < 800.0f)
		{
			taskSpriteSize.x += 20.0f;
		}
		taskAlpha += 0.02f;
		if (taskAlpha >= 2.0f)
		{
			taskfeed = false;
		}
	}
	else
	{
		taskAlpha -= 0.02f;
	}
	TaskSprite->SetSize({taskSpriteSize.x, 1000.0f});
	TaskSprite->setcolor({1.0f, 1.0f, 1.0f, taskAlpha});
	taskAlpha = min(taskAlpha, 2.0f);
	taskAlpha = max(taskAlpha, 0.0f);
}

void HUD::EnemyHPGauge_MultiUpdate(bool& loadf, DebugCamera* camera, std::vector<std::unique_ptr<Enemy>>& enemy)
{
	if (loadf)
	{
		for (int i = 0; i < enemy.size(); i++)
		{
			EnemyHP_Border_Multi.resize(enemy.size());
			EnemyHP_Inner_Multi.resize(enemy.size());
			multi_NowHP.resize(enemy.size());
			multi_OldHP.resize(enemy.size());
			multi_Hpt.resize(enemy.size());
			multi_sizel.resize(enemy.size());

			EnemyHP_Inner_Multi[i] = Texture::Create(122, {0.0f, -200.0f, 1}, {1.0f, 1.0f, 1.0f},
			                                         {1.0f, 1.0f, 1.0f, 1.0f});
			EnemyHP_Border_Multi[i] = Texture::Create(123, {0.0f, -200.0f, 1}, {1.0f, 1.0f, 1.0f},
			                                          {1.0f, 1.0f, 1.0f, 1.0f});
			//EnemyHP_Border_Multi[i]->SetPosition({ 80,860,1 });
			EnemyHP_Inner_Multi[i]->CreateTexture();
			EnemyHP_Border_Multi[i]->CreateTexture();
			EnemyHP_Border_Multi[i]->SetAnchorPoint({0.0f, 0.0f});
			EnemyHP_Inner_Multi[i]->SetAnchorPoint({0.0f, 0.0f});
		}
		loadf = false;
	}

	for (int i = 0; i < enemy.size(); i++)
	{
		//パラメータの設定
		if (enemy[i] != nullptr && EnemyHP_Border_Multi.size() > 0)
		{
		


			EnemyHP_Inner_Multi[i]->Update(camera);
			EnemyHP_Border_Multi[i]->Update(camera);
			EnemyHP_Border_Multi[i]->SetScale({2.5f, 1.0f, 1.0f});
			EnemyHP_Border_Multi[i]->SetPosition({
				enemy[i]->GetPosition().x - 10.0f, enemy[i]->GetPosition().y + 20.0f, enemy[i]->GetPosition().z
			});

			EnemyHP_Inner_Multi[i]->SetPosition({
				EnemyHP_Border_Multi[i]->GetPosition().x, EnemyHP_Border_Multi[i]->GetPosition().y,
				EnemyHP_Border_Multi[i]->GetPosition().z
			});
		}
	}
}

void HUD::EnemyHPGauge_MultiDraw()
{
	Texture::PreDraw();
	for (int i = 0; i < EnemyHP_Border_Multi.size(); i++)
	{
		EnemyHP_Border_Multi[i]->Draw();
		EnemyHP_Inner_Multi[i]->Draw();
	}
	Texture::PostDraw();
}

void HUD::SkillBottonDraw()
{
	Sprite::PreDraw();
	MiniMapFrame->Draw();
	PlayerHPFrame2->Draw();
	PlayerHP->Draw();
	PlayerHPFrame->Draw();


	ChestCollectFrame->Draw();
	if (ChestControl::GetInstance()->ChestCount() == 1)
	{
		ChestCollect[0]->Draw();
	}
	if (ChestControl::GetInstance()->ChestCount() == 2)
	{
		ChestCollect[0]->Draw();
		ChestCollect[1]->Draw();
	}
	if (ChestControl::GetInstance()->ChestCount() == 3)
	{
		ChestCollect[0]->Draw();
		ChestCollect[1]->Draw();
		ChestCollect[2]->Draw();
	}
	if (ChestControl::GetInstance()->ChestCount() == 4)
	{
		ChestCollect[0]->Draw();
		ChestCollect[1]->Draw();
		ChestCollect[2]->Draw();
		ChestCollect[3]->Draw();
	}
	if (ChestControl::GetInstance()->ChestCount() == 5)
	{
		ChestCollect[0]->Draw();
		ChestCollect[1]->Draw();
		ChestCollect[2]->Draw();
		ChestCollect[3]->Draw();
		ChestCollect[4]->Draw();
	}
	Sprite::PostDraw();
}

void HUD::Update()
{
	PlayerHP->setcolor({1.0f, 1.0f, 1.0f, 1.0f});
	for (int i = 0; i < 5; i++)
	{
		ChestCollect[i]->setcolor({1.0f, 1.0f, 1.0f, 1.0f});
	}
}

void HUD::Draw()
{
}

void HUD::AreaName()
{
	Sprite::PreDraw();
	TaskSprite->Draw();
	Sprite::PostDraw();
}
