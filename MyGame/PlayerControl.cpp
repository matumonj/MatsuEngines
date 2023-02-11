#include "PlayerControl.h"
#include"SceneManager.h"
#include"TutorialSprite.h"
#include"PlayerAttackState.h"
#include"AttackCollision.h"

PlayerControl* PlayerControl::GetIns()
{
	static PlayerControl instance;
	return &instance;
}

void PlayerControl::Init_Tutorial()
{
	Sprite::LoadTexture(190, L"Resources/2d/damage/playerdamage.png");

	DamageTex = Sprite::Create(190, {0, 0});
	DamageTex->SetSize({1900, 1000});
	player = std::make_unique<Player>();
	player->Initialize();

	StartPos = {62.0f, -20.0f, -386.0f};

	player->SetPosition(StartPos);
	AttackCollision::GetIns()->Init();
}

void PlayerControl::Init_Play()
{
	StartPos = {53.0f, -20.0f, -189.0f};

	player->SetHP(player->GetMaxHP());

	player->SetPosition(StartPos);
	AttackCollision::GetIns()->Init();
}

void PlayerControl::Init_Boss()
{
	StartPos = {-1.0f, 10.0f, -80.0f};
	player->SetHP(player->GetMaxHP());
	player->SetPosition(StartPos);
	AttackCollision::GetIns()->Init();
}

/*------------------------*/
/*--------解放処理---------*/
/*------------------------*/
void PlayerControl::Finalize()
{
	AttackCollision::GetIns()->Finalize();
	//Destroy_unique(player);
}

/*------------------------*/
/*--------読込処理---------*/
/*----------scv----------*/
void PlayerControl::Load()
{
	//初期位置
	switch (SceneManager::GetIns()->GetScene())
	{
	case SceneManager::TUTORIAL:
		StartPos = {92.0f, -50.0f, -760.0f};
		break;
	case SceneManager::PLAY:
		StartPos = {110.0f, -12.0f, -379.0f};
		break;
	case SceneManager::BOSS:
		StartPos = {0.0f, 12.0f, -50.0f};
		break;
	default:
		break;
	}

	player->SetPosition(StartPos);
	AttackCollision::GetIns()->Init();
}

void PlayerControl::GameOverResetParam()
{
	//死亡時の位置と体力のリセット
	player->SetPosition(StartPos);
	player->ReStartSetParam();
	player->SetHP(player->GetMaxHP());
}

/*------------------------*/
/*--------更新処理---------*/
/*------------------------*/
#include"HUD.h"
#include"CameraControl.h"
//playerの中にある移動処理とかは後でこっち持ってくる
void PlayerControl::Update_Tutorial() //チュートリアル時
{
	if (player == nullptr)
	{
		return;
	}

	PlayerAttackState::GetIns()->Update();
	player->Update();
	DamageTexUpdate();
}

void PlayerControl::Update_Play() //プレイシーン時
{
	PlayerAttackState::GetIns()->Update();
	player->Update();

	TargetMarker::GetIns()->Update_PlayScene(CameraControl::GetIns()->GetCamera());

	DamageTexUpdate();
}

void PlayerControl::Update_Boss()
{
	PlayerAttackState::GetIns()->Update();
	player->Update();

	DamageTexUpdate();
}

void PlayerControl::DamageTexUpdate()
{
	if (HUD::GetIns()->GetRecvDamageFlag())
	{
		dalpha = 1.0f;
	}
	dalpha -= 0.02f;
	DamageTex->setcolor({1, 1, 1, dalpha});
	dalpha = max(dalpha, 0.0f);
	/*if (HUD::GetIns()->GetRecvDamageFlag())
	{
		vignette = 0.5f;
	}
	if(player->GetHP()<0)
	{
		vignette = 0.5f;
	}
	vignette -= 0.02f;
	vignette = max(vignette, 0.0f);*/
}

void PlayerControl::DamageTexDraw()
{
	if (HUD::GetIns()->GetPlayerHP()->GetSize().x > 0.f && GetIns()->GetPlayer()->GetHP() > 0)
	{
		player->DamageTexDraw();
		Sprite::PreDraw();
		DamageTex->Draw();
		Sprite::PostDraw();
	}
}

/*------------------------*/
/*--------描画処理---------*/
/*------------------------*/
/*同じ処理が並んでしまっている...*/
void PlayerControl::Draw_Play()
{
	if (player == nullptr || TurnoffDrawF)
	{
		return;
	}
	player->Draw();
	ImGui::Begin("pos,,");

	ImGui::SetWindowPos(ImVec2(0, 500));
	ImGui::SetWindowSize(ImVec2(300, 300));
	ImGui::Text("%f", player->GetPosition().x);
	ImGui::Text("%f", player->GetPosition().z);
	ImGui::End();
}

void PlayerControl::Draw_Tutorial()
{
	if (player == nullptr)
	{
		return;
	}
	player->Draw();
	ImGui::Begin("pos,,");

	ImGui::SetWindowPos(ImVec2(0, 500));
	ImGui::SetWindowSize(ImVec2(300, 300));
	ImGui::Text("%f", player->GetPosition().x);
	ImGui::Text("%f", player->GetPosition().z);
	ImGui::End();
}

void PlayerControl::Draw_Boss()
{
	if (player == nullptr)
	{
		return;
	}
	player->Draw();
}

void PlayerControl::BossFieldCol()
{
	if (Collision::GetLength(player->GetPosition(), {0, -10, 0}) > 120)
	{
		player->isOldPos();
	}
}
