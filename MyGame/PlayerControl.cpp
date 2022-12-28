#include "PlayerControl.h"
#include"SceneManager.h"
#include"TutorialSprite.h"
#include"PlayerAttackState.h"
#include"AttackCollision.h"
#include "CustomButton.h"
#include"Feed.h"

PlayerControl* PlayerControl::GetInstance()
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

	StartPos = {92.0f, -23.0f, -760.0f};

	player->SetPosition(StartPos);
	AttackCollision::GetInstance()->Init();
}

void PlayerControl::Init_Play()
{
	StartPos = {110.0f, -30.0f, -379.0f};

	player->SetPosition(StartPos);
	AttackCollision::GetInstance()->Init();
}

void PlayerControl::Init_Boss()
{
	StartPos = {-1.0f, 10.0f, -106.0f};
	player->SetHP(player->GetMaxHP());
	player->SetPosition(StartPos);
	AttackCollision::GetInstance()->Init();
}

/*------------------------*/
/*--------解放処理---------*/
/*------------------------*/
void PlayerControl::Finalize()
{
	AttackCollision::GetInstance()->Finalize();
	//Destroy_unique(player);
}

/*------------------------*/
/*--------読込処理---------*/
/*----------scv----------*/
void PlayerControl::Load()
{
	//初期位置
	switch (SceneManager::GetInstance()->GetScene())
	{
	case SceneManager::TUTORIAL:
		StartPos = {92.0f, -20.0f, -760.0f};
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
	AttackCollision::GetInstance()->Init();
}

void PlayerControl::GameOverResetParam()
{
	//死亡時の位置と体力のリセット
	player->SetPosition(StartPos);
	player->SetHP(player->GetMaxHP());
}

/*------------------------*/
/*--------更新処理---------*/
/*------------------------*/
#include"HUD.h"
#include"CameraControl.h"
#include <EnemyControl.h>
//playerの中にある移動処理とかは後でこっち持ってくる
void PlayerControl::Update_Tutorial() //チュートリアル時
{
	if (player == nullptr)
	{
		return;
	}
	player->Update();
	DamageTexUpdate();
}

void PlayerControl::Update_Play() //プレイシーン時
{
	player->Update();

	DamageTexUpdate();
}

void PlayerControl::Update_Boss()
{
	player->Update();
	
	DamageTexUpdate();
}

void PlayerControl::DamageTexUpdate()
{
	if (HUD::GetInstance()->GetRecvDamageFlag())
	{
		dalpha = 1.0f;
	}
	dalpha -= 0.02f;
	DamageTex->setcolor({1, 1, 1, dalpha});
	PlayerAttackState::GetInstance()->Update();
	dalpha = max(dalpha, 0.0f);
}

/*------------------------*/
/*--------描画処理---------*/
/*------------------------*/
void PlayerControl::Draw_Play()
{
	if (player == nullptr||TurnoffDrawF)
	{
		return;
	}
	player->Draw();
}

void PlayerControl::Draw_Tutorial()
{
	if (player == nullptr)
	{
		return;
	}
	player->Draw();
}

void PlayerControl::Draw_Boss()
{
	if (player == nullptr)
	{
		return;
	}
	player->Draw();
}

void PlayerControl::DamageTexDraw()
{
	if (HUD::GetInstance()->GetPlayerHP()->GetSize().x > 0.f && PlayerControl::GetInstance()->GetPlayer()->GetHP() > 0)
	{
		Sprite::PreDraw();
		DamageTex->Draw();
		Sprite::PostDraw();
	}
}

void PlayerControl::BossFieldCol()
{
	if (Collision::GetLength(player->GetPosition(), {0, -10, 0}) > 120)
	{
		player->isOldPos();
	}
}
