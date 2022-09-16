#include "PlayerControl.h"
#include"SceneManager.h"
#include"TutorialSprite.h"
#include"PlayerAttackState.h"
#include"AttackCollision.h"
PlayerControl* PlayerControl::GetInstance()
{
	static PlayerControl instance;
	return &instance;
}

void PlayerControl::Finalize()
{
	Destroy_unique(player);
}
void PlayerControl::Load(DebugCamera* camera)
{
	player = std::make_unique<Player>();
	player->Initialize(camera);
	//各シーンの初期座標設定
	if (SceneManager::GetInstance()->GetScene() == SceneManager::TUTORIAL) {
		player->SetPosition({ 92.0f,2.0f,-760.0f });
	} else if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY) {
		player->SetPosition({ 110.0f,5.0f,-379.0f });
	}
	else if (SceneManager::GetInstance()->GetScene() == SceneManager::BOSS) {
		player->SetPosition({ 0.0f,2.0f,-70.0f });
	}
}

void PlayerControl::Initialize(DebugCamera* camera)
{
	AttackCollision::GetInstance()->Init();
}

void PlayerControl::Update(DebugCamera* camera)
{
	player->Update(camera);
	PlayerAttackState::GetInstance()->Update();
}

void PlayerControl::Draw()
{
	if (player!= nullptr) {
		player->Draw();
	}
}

void PlayerControl::ImGuiDraw()
{

}

void PlayerControl::SetColor(XMFLOAT4 color)
{

}
