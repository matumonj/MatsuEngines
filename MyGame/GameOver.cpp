#include "GameOver.h"
#include "GameOver.h"
#include"Feed.h"
#include"SceneManager.h"
#include"BossScene.h"

GameOver::GameOver(SceneManager* sceneManager)
	: BaseScene(sceneManager)
{
}

void GameOver::Initialize()
{
	Sprite::LoadTexture(100, L"Resources/2d/scenetex/gameover.png");
	Sprite* l_tex = Sprite::Create(100, {0, 0});
	GameOverTex.reset(l_tex);
	GameOverTex->SetAnchorPoint({0.5f, 0.5f});
}

void GameOver::Update()
{
	Feed::GetInstance()->Update_Black(Feed::FEEDIN);
	//TexAlpha += 0.02f;
	if (TexAlpha >= 1.0f)
	{
	}
	//��ʐ^�����Ȃ�����
	BaseScene* scene = new BossScene(sceneManager_); //���̃V�[���̃C���X�^���X����
	//Play = false;
	SceneManager::GetInstance()->SetScene(SceneManager::BOSS);
	sceneManager_->SetnextScene(scene); //�V�[���̃Z�b�g
}

void GameOver::Draw()
{
	DirectXCommon::GetInstance()->BeginDraw();
	Feed::GetInstance()->Draw();
	Sprite::PreDraw();
	GameOverTex->Draw();
	Sprite::PostDraw();

	DirectXCommon::GetInstance()->EndDraw();
}

void GameOver::Finalize()
{
}
