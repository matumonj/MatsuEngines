#include "PlayerControl.h"
#include"SceneManager.h"
#include"TutorialSprite.h"
#include"PlayerAttackState.h"
#include"AttackCollision.h"
#include"Feed.h"
PlayerControl* PlayerControl::GetInstance()
{
	static PlayerControl instance;
	return &instance;
}

/*------------------------*/
/*--------解放処理---------*/
/*------------------------*/
void PlayerControl::Finalize()
{
	AttackCollision::GetInstance()->Finalize();
		Destroy_unique(player);
}

/*------------------------*/
/*--------読込処理---------*/
/*----------scv----------*/
void PlayerControl::Load(DebugCamera* camera)
{
	switch (SceneManager::GetInstance()->GetScene())
	{
	case SceneManager::TUTORIAL:
		StartPos = { 92.0f,-12.0f,-760.0f };
		break;
	case SceneManager::PLAY:
		StartPos = { 110.0f,-12.0f,-379.0f };
		break;
	case SceneManager::BOSS:
		StartPos = { 0.0f,2.0f,-50.0f };
		break;
	default:
		break;
	}

	player->SetPosition(StartPos);
	AttackCollision::GetInstance()->Init();
}

void PlayerControl::Initialize(DebugCamera* camera)
{
	Texture::LoadTexture(93, L"Resources/ParticleTex/slash.png");

	player = std::make_unique<Player>();
	player->Initialize(camera);
	//各シーンの初期座標設定

}

/*------------------------*/
/*--------更新処理---------*/
/*------------------------*/
//playerの中にある移動処理とかは後でこっち持ってくる
void PlayerControl::Update_Tutorial(DebugCamera* camera)//チュートリアル時
{
	player->Update(camera);
	PlayerAttackState::GetInstance()->Update();
}

void PlayerControl::Update_Play(DebugCamera* camera)//プレイシーン時
{
	player->Update(camera);
	PlayerAttackState::GetInstance()->Update();
}
void PlayerControl::Update_Boss(DebugCamera* camera)
{
	player->Update(camera);
	PlayerAttackState::GetInstance()->Update();

}
/*------------------------*/
/*--------描画処理---------*/
/*------------------------*/
void PlayerControl::Draw()
{
	if (player == nullptr)return;
		player->Draw();	
}
