#include "PlayerControl.h"
#include"SceneManager.h"
#include"TutorialSprite.h"
#include"PlayerAttackState.h"
PlayerControl* PlayerControl::GetInstance()
{
	static PlayerControl instance;
	return &instance;
}
void PlayerControl::Load(DebugCamera* camera)
{
	player = std::make_unique<Player>();
	player->Initialize(camera);
	if (SceneManager::GetInstance()->GetScene() == SceneManager::TUTORIAL) {
		player->SetPosition({ 92,-27,-760 });
	} else if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY) {
		player->SetPosition({ 92,-27,-760 });
	}
}

void PlayerControl::Initialize(DebugCamera* camera)
{
	player = std::make_unique<Player>();
	player->Initialize(camera);
	if (SceneManager::GetInstance()->GetScene() == SceneManager::TUTORIAL) {
		player->SetPosition({92,-27,-760});
	}
	else if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY) {
		player->SetPosition({ 92,-27,-760 });
	}
}

void PlayerControl::Update(DebugCamera* camera)
{
	player->SetPosition({ 92,-27,-760 });
	PlayerAttackState::GetInstance()->Update();
	//Player::GetInstance()->Update(camera);
	if (player != nullptr) {
		player->Update(camera);
	}

}

void PlayerControl::Draw()
{
	//if (Players[0].get() == nullptr)return;
	//if (Players[0].get() != nullptr) {
		if (player!= nullptr) {
				player->Draw();
			}
	//}
}

void PlayerControl::ImGuiDraw()
{

}

void PlayerControl::SetColor(XMFLOAT4 color)
{

}