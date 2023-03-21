#include "MyGame.h"

#include "GameOver.h"
#include"SceneManager.h"
#include"imgui.h"
#include"ParticleManager.h"
#include "ModelManager.h"
#include"ImageManager.h"

void MyGame::Initialize()
{
	Framework::Initialize();
	Sprite::StaticInitialize(WinApp::window_width, WinApp::window_height);
	LightGroup::StaticInitialize();
	FbxLoader::GetIns()->Initialize();
	Texture::StaticInitialize(WinApp::window_width, WinApp::window_height);
	ParticleManager::StaticInitialize(WinApp::window_width, WinApp::window_height);
	//初期シーン
	BaseScene* scene_ = new TitleScene(sceneManger);
	sceneManger->SetnextScene(scene_);
	//フィールドモデルだけ最初から
	ModelManager::GetIns()->InitField();
	//タイトルテクスチャ
	ImageManager::GetIns()->TitleSpriteInit();

	GameOver::GetIns()->Initialize();

	Sprite::LoadTexture(0, L"Resources/2d/LevelUp/debugfont.png");

	DebugTextSprite::GetIns()->Initialize(0);
}

void MyGame::Update()
{
	Framework::Update();
}


void MyGame::Draw()
{
	Framework::Draw();
}

void MyGame::Finalize()
{
	Framework::Finalize();
}
