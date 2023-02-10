#include "GameOver.h"
#include"SceneManager.h"
#include "EnemyControl.h"
#include "HUD.h"
#include "PlayerControl.h"
#include"mHelper.h"
#include "PlayerAttackState.h"
#include "UI.h"

GameOver* GameOver::GetIns()
{
	static GameOver ins;

	return &ins;
}

GameOver::GameOver()
{
}

void GameOver::Initialize()
{
	ReStartF = false;
	Sprite::LoadTexture(110, L"Resources/2d/SceneTex/gameoverframe.png");
	Sprite* l_tex = Sprite::Create(110, {0, 0});
	GameOverTexFrame.reset(l_tex);
	GameOverTexFrame->SetAnchorPoint({0.5f, 0.5f});

	Sprite::LoadTexture(111, L"Resources/2d/SceneTex/gameovertxt.png");
	Sprite* l_tex2 = Sprite::Create(111, {0, 0});
	GameOverTex.reset(l_tex2);
	GameOverTex->SetAnchorPoint({0.5f, 0.5f});
	FrameSize.y = 500.f;

	Texture::LoadTexture(72, L"Resources/2d/mainEffect/pipo-hiteffect041.png");

	RestartPar = std::make_unique<Particle>();
	RestartPar->Init(72);
	RestartPar->SetParScl({0.3f, 0.3f});
	RestartPar->SetParColor({1.f, 1.f, 1.f, 1.f});
}

void GameOver::Update()
{
	if (ReStartF)
	{
		PlayerAttackState::GetIns()->SetHitStopJudg(false);
		AlphaEaseT -= 0.01f;
		FrameScalingT -= 0.01f;
		if (AlphaEaseT <= 0.0f)
		{
			EnemyControl::GetIns()->GameOverResetParam();
			if (SceneManager::GetIns()->GetScene() == SceneManager::PLAY) {
				EnemyControl::GetIns()->GuardianCreate();
			}
			PlayerControl::GetIns()->GameOverResetParam();
			PlayerControl::GetIns()->TurnoffDraw(false);
			UI::GetIns()->SetTurnoffUIDraw(false);
			PlayerDestF = true;
			ReStartF = false;
		}
	}
	else
	{
		//UI::GetIns()->SetTurnoffUIDraw(false);
		if (HUD::GetIns()->GetPlayerHP()->GetSize().x <= 50.f && PlayerControl::GetIns()->GetPlayer()->GetHP()
			<= 0)
		{
			UI::GetIns()->SetTurnoffUIDraw(true);
			PlayerAttackState::GetIns()->SetHitStopJudg(true);
			FrameScalingT += 0.04f;
			if (FrameScalingT >= 1.0f)
			{
				AlphaEaseT += 0.02f;
			}
			if (AlphaEaseT >= 1.0f && Input::GetIns()->TriggerButton(Input::B))
			{
				PlayerControl::GetIns()->TurnoffDraw(true);
				if (SceneManager::GetIns()->GetScene() == SceneManager::PLAY) {
					EnemyControl::GetIns()->GuardianReset();
				}
				PlayerDestF = true;
				ReStartF = true;
			}
		}
	}
	FrameSize.x = Easing::EaseOut(FrameScalingT, 0.0f, 1900.0f);
	TexAlpha = Easing::EaseOut(AlphaEaseT, 0.0f, 0.7f);

	GameOverTexFrame->SetPosition({960.f, 480.f});
	GameOverTexFrame->SetSize(FrameSize);
	GameOverTex->SetSize({1900.f, 400.f});
	GameOverTex->SetPosition({960.f, 480.f});
	GameOverTex->setcolor({1.f, 1.f, 1.f, TexAlpha});


	XMFLOAT3 ppos = PlayerControl::GetIns()->GetPlayer()->GetPosition();
	if (PlayerDestF)
	{
		RestartPar->SetParF(1);
		RestartPar->CreateParticle(PlayerDestF, {ppos});
		PlayerDestF = false;
	}


	RestartPar->Upda_B();
	TexAlpha = max(TexAlpha, 0.0f);
	TexAlpha = min(TexAlpha, 1.0f);
	AlphaEaseT = max(AlphaEaseT, 0.0f);
	AlphaEaseT = min(AlphaEaseT, 1.0f);

	FrameScalingT = max(FrameScalingT, 0.0f);
	FrameScalingT = min(FrameScalingT, 1.0f);
}

void GameOver::Draw()
{

	Draw_DestParticle();

	Sprite::PreDraw();
	GameOverTexFrame->Draw();
	GameOverTex->Draw();


	Sprite::PostDraw();

	ImGui::Begin("pos");
	ImGui::Text("hp %d", PlayerControl::GetIns()->GetPlayer()->GetHP());
	ImGui::SliderFloat("posy", &FramePos.y, 0.f, 1000.f);
	ImGui::End();
}

void GameOver::Draw_DestParticle()
{
	RestartPar->Draw();
}

void GameOver::Finalize()
{
}
