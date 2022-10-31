#include "HUD.h"
#include"HUDLayOut.h"
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
	for (int i = 0; i <5; i++) {
		delete ChestCollect[i];
	}
	delete PlayerHP, EnemyHP_Border, EnemyHP_Inner;
	delete FirstAttackSprite, SecondAttackSprite, ThirdAttackSprite;
	delete coolDownSprite[0], coolDownSprite[1], coolDownSprite[2], coolDownSprite[3], BuffSprite;
	for (int i = 0; i < EnemyHP_Border_Multi.size(); i++) {
		delete EnemyHP_Border_Multi[i], EnemyHP_Inner_Multi[i];
	}
}
void HUD::Finalize()
{
	for (int i = 0; i < 5; i++) {
		delete ChestCollect[i];
	}
	delete ChestCollectFrame;
	delete PlayerHP, EnemyHP_Border, EnemyHP_Inner;
	delete FirstAttackSprite, SecondAttackSprite, ThirdAttackSprite;
	delete coolDownSprite[0], coolDownSprite[1], coolDownSprite[2], coolDownSprite[3], BuffSprite;
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

	TaskSprite = Sprite::Create(143, { 0.0f,-200.0f });
	TaskSprite->SetPosition({ WinApp::window_width / 2,WinApp::window_height / 2 });
	TaskSprite->SetAnchorPoint({ 0.5,0.5 });
	PlayerHP = Sprite::Create(144, { 0.0f,-200.0f });
	PlayerHP->SetPosition({ 70,50 });

	PlayerHPFrame = Sprite::Create(145, { 0.0f,-200.0f });

	PlayerHPFrame2 = Sprite::Create(146, { 0.0f,-200.0f });


	for (int i = 0; i < 5; i++) {
		ChestCollect[i] = Sprite::Create(147, { 0.0f,-200.0f });
		ChestCollect[i]->SetPosition({ (float)i * 100.0f+70.0f,100.0f });
		ChestCollect[i]->SetSize({ 100.0f,100.0f });
	}
	ChestCollectFrame = Sprite::Create(148, { 0.0f,-200.0f });
	ChestCollectFrame->SetPosition({ 70.0f,100.0f });
	ChestCollectFrame->SetSize({ 500.0f,100.0f });

	Sprite::LoadTexture(121, L"Resources/bosshp.png");
	Sprite::LoadTexture(20, L"Resources/jump.png");
	Sprite::LoadTexture(21, L"Resources/2d/attackicon/magicattack1.png");
	Sprite::LoadTexture(22, L"Resources/2d/attackicon/magicattack2.png");
	Sprite::LoadTexture(23, L"Resources/attack3.png");

	Sprite::LoadTexture(27, L"Resources/attack1.png");
	Sprite::LoadTexture(25, L"Resources/attack2.png");
	Sprite::LoadTexture(26, L"Resources/attack3.png");

	PlayerHPSize=(float)PlayerControl::GetInstance()->GetPlayer()->GetMaxHP() * 7.0f;

	MiniMapFrame= Sprite::Create(149, { 0.0f,-200.0f });
	MiniMapFrame->SetAnchorPoint({ 0.5F,0.5F });
	MiniFrameSize={ 480,500 };
	MiniframePos={ 1600,200 };
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
	if (skillicon == WAND) {
		FirstAttackSprite = Sprite::Create(20, { CenterPosition.x + 100,CenterPosition.y });

		SecondAttackSprite = Sprite::Create(21, { CenterPosition.x,CenterPosition.y + 100 });

		ThirdAttackSprite = Sprite::Create(22, { CenterPosition.x - 100,CenterPosition.y });

		BuffSprite = Sprite::Create(23, { CenterPosition.x,CenterPosition.y - 100 });
	}
	if (skillicon ==SWORD) {
		FirstAttackSprite = Sprite::Create(20, { CenterPosition.x + 100,CenterPosition.y });

		SecondAttackSprite = Sprite::Create(27, { CenterPosition.x,CenterPosition.y + 100 });

		ThirdAttackSprite = Sprite::Create(25, { CenterPosition.x - 100,CenterPosition.y });

		BuffSprite = Sprite::Create(26, { CenterPosition.x,CenterPosition.y - 100 });
	}

	Sprite::LoadTexture(28, L"Resources/cool.png");
	coolDownSprite[0] = Sprite::Create(28, { CenterPosition.x + 100 + 120,CenterPosition.y + 120 }, { 1,1,1,1 }, { 0.0,1 });
	coolDownSprite[1] = Sprite::Create(28, { CenterPosition.x + 120,CenterPosition.y + 100 + 120 }, { 0,0,0,1 }, { 0.0,1 });
	coolDownSprite[2] = Sprite::Create(28, { CenterPosition.x - 100 + 120,CenterPosition.y + 120 }, { 0,0,0,1 }, { 0.0,1 });
	coolDownSprite[3] = Sprite::Create(28, { CenterPosition.x + 120,CenterPosition.y - 100 + 120 }, { 0,0,0,1 }, { 0.0,1 });
	for (int i = 0; i < 4; i++) {
		coolDownSprite[i]->SetSize({ 100,0 });
	}


}

void HUD::EnemyHPGaugeUpdate(std::vector<std::unique_ptr<Enemy>>& enemy)
{

}
void HUD::SetSkillIcon(SkillIcon icon)
{ 
	skillicon = icon; 
	iconchangeF = true;
	if (iconchangeF) {
		SkillButtonInitialize();
		iconchangeF = false;
	}
}
void HUD::SkillBottonUpdate()
{
	FirstAttackSprite->SetSize({ 120,120 });
	SecondAttackSprite->SetSize({ 120,120 });
	ThirdAttackSprite->SetSize({ 120,120 });
	BuffSprite->SetSize({ 120,120 });

	if (CustomButton::GetInstance()->GetActionButton_JUMP() == CustomButton::BUTTON_B) {
		FirstAttackSprite->SetPosition({ CenterPosition.x + 100,CenterPosition.y });
	} else if (CustomButton::GetInstance()->GetActionButton_JUMP() == CustomButton::BUTTON_A) {
		FirstAttackSprite->SetPosition({ CenterPosition.x,CenterPosition.y + 100 });
	} else if (CustomButton::GetInstance()->GetActionButton_JUMP() == CustomButton::BUTTON_X) {
		FirstAttackSprite->SetPosition({ CenterPosition.x ,CenterPosition.y - 100 });
	} else if (CustomButton::GetInstance()->GetActionButton_JUMP() == CustomButton::BUTTON_Y) {
		FirstAttackSprite->SetPosition({ CenterPosition.x - 100 ,CenterPosition.y });
	}

	if (CustomButton::GetInstance()->GetActionButton_ATTACK() == CustomButton::BUTTON_B) {
		SecondAttackSprite->SetPosition({ CenterPosition.x + 100,CenterPosition.y });
	} else if (CustomButton::GetInstance()->GetActionButton_ATTACK() == CustomButton::BUTTON_A) {
		SecondAttackSprite->SetPosition({ CenterPosition.x,CenterPosition.y + 100 });
	} else if (CustomButton::GetInstance()->GetActionButton_ATTACK() == CustomButton::BUTTON_X) {
		SecondAttackSprite->SetPosition({ CenterPosition.x ,CenterPosition.y - 100 });
	} else if (CustomButton::GetInstance()->GetActionButton_ATTACK() == CustomButton::BUTTON_Y) {
		SecondAttackSprite->SetPosition({ CenterPosition.x - 100,CenterPosition.y });
	}

	if (CustomButton::GetInstance()->GetActionButton_ATTACK2() == CustomButton::BUTTON_B) {
		ThirdAttackSprite->SetPosition({ CenterPosition.x + 100,CenterPosition.y });
	} else if (CustomButton::GetInstance()->GetActionButton_ATTACK2() == CustomButton::BUTTON_A) {
		ThirdAttackSprite->SetPosition({ CenterPosition.x,CenterPosition.y + 100 });
	} else if (CustomButton::GetInstance()->GetActionButton_ATTACK2() == CustomButton::BUTTON_X) {
		ThirdAttackSprite->SetPosition({ CenterPosition.x ,CenterPosition.y - 100 });
	} else if (CustomButton::GetInstance()->GetActionButton_ATTACK2() == CustomButton::BUTTON_Y) {
		ThirdAttackSprite->SetPosition({ CenterPosition.x - 100,CenterPosition.y });
	}

	if (CustomButton::GetInstance()->GetActionButton_ATTACK3() == CustomButton::BUTTON_B) {
		BuffSprite->SetPosition({ CenterPosition.x + 100,CenterPosition.y });
	} else if (CustomButton::GetInstance()->GetActionButton_ATTACK3() == CustomButton::BUTTON_A) {
		BuffSprite->SetPosition({ CenterPosition.x,CenterPosition.y + 100 });
	} else if (CustomButton::GetInstance()->GetActionButton_ATTACK3() == CustomButton::BUTTON_X) {
		BuffSprite->SetPosition({ CenterPosition.x ,CenterPosition.y - 100 });
	} else if (CustomButton::GetInstance()->GetActionButton_ATTACK3() == CustomButton::BUTTON_Y) {
		BuffSprite->SetPosition({ CenterPosition.x - 100,CenterPosition.y });
	}
	//SecondAttackSprite->SetPosition({ CenterPosition.x,CenterPosition.y + 100 });

	coolDownSprite[0]->SetPosition({ CenterPosition.x + 100 + 120,CenterPosition.y + 120 });
	coolDownSprite[1]->SetPosition({ CenterPosition.x + 120,CenterPosition.y + 100 + 120 });
	coolDownSprite[2]->SetPosition({ CenterPosition.x - 100 + 120,CenterPosition.y + 120 });
	coolDownSprite[3]->SetPosition({ CenterPosition.x + 120,CenterPosition.y - 100 + 120 });

	//各剣のクールタイム参照
	TimeSpeed = 1.0f / SelectSword::GetInstance()->GetSword()->GetCoolTime();
	if (CoolTime_Time >= 1.0f) {
		if (CustomButton::GetInstance()->GetAttackAction()|| CustomButton::GetInstance()->Get2AttackAction()|| CustomButton::GetInstance()->Get3AttackAction()) {
			CoolTime_Time = 0.0f;
		}
	} else {
		CoolTime_Time += TimeSpeed;
	}
	for (int i = 0; i < 4; i++) {
		CooltimeSize = { 120, 120 };
		coolDownSprite[i]->SetSize({ CooltimeSize.x,Easing::EaseOut(CoolTime_Time,120,0) });
		coolDownSprite[i]->SetAnchorPoint({ 1,1 });
	}
	if (PlayerAttackState::GetInstance()->GetSkill() != PlayerAttackState::GetInstance()->CoolDown) {
		FirstAttackSprite->setcolor({ 0.5,0.5,0.5,1 });
		SecondAttackSprite->setcolor({ 0.5,0.5,0.5,1 });
		ThirdAttackSprite->setcolor({ 0.5,0.5,0.5,1 });
	}
	if (PlayerAttackState::GetInstance()->GetCoolTime() == 0) {
		FirstAttackSprite->setcolor({ 1,1,1,1 });
		SecondAttackSprite->setcolor({ 1,1,1,1 });
		ThirdAttackSprite->setcolor({ 1,1,1,1 });
	}
	if (RecvDamageflag) {
		easetime += 1.0f / 60.f;
		if (easetime >= 1.0f) {
			RecvDamageflag = false;
		} else {
			PlayerHPSize = Easing::EaseOut(easetime, OldPlayerHPSize, PlayerControl::GetInstance()->GetPlayer()->GetHP()) * 7;
		}
	} else {
		easetime = 0.0f;
		OldPlayerHPSize = PlayerControl::GetInstance()->GetPlayer()->GetHP();
	}
	PlayerHP->SetSize({ PlayerHPSize ,50 });

	PlayerHPFrame->SetSize({ (float)PlayerControl::GetInstance()->GetPlayer()->GetMaxHP() * 7.0f ,50.0f });

	PlayerHPFrame2->SetSize({ (float)PlayerControl::GetInstance()->GetPlayer()->GetMaxHP() * 7.0f ,50.0f });

	PlayerHPFrame->SetPosition(PlayerHP->GetPosition());
	PlayerHPFrame2->SetPosition(PlayerHP->GetPosition());

	MiniMapFrame->SetSize({ MiniFrameSize });
	MiniMapFrame->SetPosition(MiniframePos);
}

void HUD::TaskUpdate(DebugCamera* camera)
{
	XMFLOAT3 cPos = camera->GetEye();
	if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY) {
		if (Collision::GetLength(cPos, CameraControl::GetInstance()->CameraPosIndex(2)) < 10) {
			taskfeed = true;
		}
	}
	if (taskfeed) {
		if (taskSpriteSize.x < 800) {
			taskSpriteSize.x += 20;
		}
		taskAlpha += 0.02f;
		if (taskAlpha >= 2.0f) {
			taskfeed = false;
		}
	} else {
		taskAlpha -= 0.02f;
	}
	TaskSprite->SetSize({ taskSpriteSize.x,1000 });
	TaskSprite->setcolor({ 1,1,1,taskAlpha });
	taskAlpha = min(taskAlpha, 2);
	taskAlpha = max(taskAlpha, 0);
}

void HUD::EnemyHPGauge_MultiUpdate(bool& loadf, DebugCamera* camera, std::vector<std::unique_ptr<Enemy>>& enemy)
{


	if (loadf) {
		for (int i = 0; i < enemy.size(); i++) {
			EnemyHP_Border_Multi.resize(enemy.size());
			EnemyHP_Inner_Multi.resize(enemy.size());
			multi_NowHP.resize(enemy.size());
			multi_OldHP.resize(enemy.size());
			multi_Hpt.resize(enemy.size());
			multi_sizel.resize(enemy.size());

			EnemyHP_Inner_Multi[i] = Texture::Create(122, { 0.0f,-200.0f,1 }, { 1,1,1 }, { 1,1,1,1 });
			EnemyHP_Border_Multi[i] = Texture::Create(123, { 0.0f,-200.0f,1 }, { 1,1,1 }, { 1,1,1,1 });
			//EnemyHP_Border_Multi[i]->SetPosition({ 80,860,1 });
			EnemyHP_Inner_Multi[i]->CreateTexture();
			EnemyHP_Border_Multi[i]->CreateTexture();
			EnemyHP_Border_Multi[i]->SetAnchorPoint({ 0,0 });
			EnemyHP_Inner_Multi[i]->SetAnchorPoint({ 0,0 });

		}
		loadf = false;
	}

	for (int i = 0; i < enemy.size(); i++) {

		//パラメータの設定
		if (enemy[i] != nullptr && EnemyHP_Border_Multi.size() > 0) {
			if (enemy[i]->GetRecvDamage() == true) {
				multi_NowHP[i] = Percent::GetParcent(enemy[i]->GetMaxHP(), enemy[i]->GetHP()) / 20.00f;
				multi_Hpt[i] += 0.001f;
				multi_sizel[i] = { Easing::EaseOut(multi_Hpt[i] ,multi_OldHP[i] ,multi_NowHP[i]),1.5,1 };
				EnemyHP_Inner_Multi[i]->SetScale(multi_sizel[i]);
				if (multi_Hpt[i] >= 1.0f) {
					enemy[i]->SetRecvDamage(false);
				}
			} else {
				multi_OldHP[i] = Percent::GetParcent(enemy[i]->GetMaxHP(), enemy[i]->GetHP()) / 20.00f;
				multi_Hpt[i] = 0;
				EnemyHP_Inner_Multi[i]->SetScale({ Percent::GetParcent(enemy[i]->GetMaxHP(),enemy[i]->GetHP()) / 20.00f,1.5,1 });
			}


			EnemyHP_Inner_Multi[i]->Update(camera);
			EnemyHP_Border_Multi[i]->Update(camera);
			EnemyHP_Border_Multi[i]->SetScale({ 2.5,1,1 });
			EnemyHP_Border_Multi[i]->SetPosition({ enemy[i]->GetPosition().x - 10, enemy[i]->GetPosition().y + 20.0f, enemy[i]->GetPosition().z });

			EnemyHP_Inner_Multi[i]->SetPosition({ EnemyHP_Border_Multi[i]->GetPosition().x,EnemyHP_Border_Multi[i]->GetPosition().y, EnemyHP_Border_Multi[i]->GetPosition().z });

		}
	}
}

void HUD::EnemyHPGauge_MultiDraw()
{
	Texture::PreDraw();
	for (int i = 0; i < EnemyHP_Border_Multi.size(); i++) {
		EnemyHP_Border_Multi[i]->Draw();
		EnemyHP_Inner_Multi[i]->Draw();
	}
	Texture::PostDraw();
}

void HUD::SkillBottonDraw()
{
	ImGui::Begin("Miniframe");
	ImGui::SliderFloat("SizeX", &MiniFrameSize.x, 0, 1000);
	ImGui::SliderFloat("Sizey", &MiniFrameSize.y, 0, 1000);

	ImGui::SliderFloat("posX", &MiniframePos.x, 0, 1900);
	ImGui::SliderFloat("Posy", &MiniframePos.y, -200, 1000);
	ImGui::End();
	Sprite::PreDraw();
	MiniMapFrame->Draw();
	FirstAttackSprite->Draw();
	ThirdAttackSprite->Draw();
	BuffSprite->Draw();
	SecondAttackSprite->Draw();

	PlayerHPFrame2->Draw();
	PlayerHP->Draw();
	PlayerHPFrame->Draw();
	for (int i = 0; i < 4; i++) {
		coolDownSprite[i]->Draw();
	}

	ChestCollectFrame->Draw();
	if (ChestControl::GetInstance()->ChestCount() == 1) {
		ChestCollect[0]->Draw();
	}
	if (ChestControl::GetInstance()->ChestCount() == 2) {
		ChestCollect[0]->Draw();
		ChestCollect[1]->Draw();
	}
	if (ChestControl::GetInstance()->ChestCount() == 3) {
		ChestCollect[0]->Draw();
		ChestCollect[1]->Draw();
		ChestCollect[2]->Draw();
	}
	if (ChestControl::GetInstance()->ChestCount() == 4) {
		ChestCollect[0]->Draw();
		ChestCollect[1]->Draw();
		ChestCollect[2]->Draw();
		ChestCollect[3]->Draw();
	}
	if (ChestControl::GetInstance()->ChestCount() == 5) {
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
	HUDLayOut::GetInstance()->Update();
	PlayerHP->setcolor({ 1.0f,1.0f,1.0f,1.0f });
	for (int i = 0; i < 5; i++) {
		ChestCollect[i]->setcolor({ 1.0f,1.0f,1.0f,1.0f });
	}
}

void HUD::Draw()
{
}

bool HUD::GetLayOutMode()
{
	return HUDLayOut::GetInstance()->GetLayOutMode();
}


void HUD::ChangePosition(XMFLOAT2 MousePosition, XMFLOAT2& c)
{
	float halfsize_x = 50;
	float halfsize_y = 50;

	if (Input::GetInstance()->PushMouseLeft() && Collision::CheckPoint2Rect({ MousePosition.x,MousePosition.y }, { {c.x,c.y},{100,100} }) == true)
	{
		f = true;
	}
	if (f) {
		c = { MousePosition.x - halfsize_x,MousePosition.y - halfsize_y };
		if (Input::GetInstance()->PushMouseLeft() != true) {
			f = false;
		}
	}
}

void HUD::AreaName()
{
	Sprite::PreDraw();
	TaskSprite->Draw();
	Sprite::PostDraw();
}