#include "LoadScene.h"
#include"Input.h"
#include"PlayScene.h"
#include"SceneManager.h"
LoadScene::LoadScene(SceneManager* sceneManager)
	:BaseScene(sceneManager)
{

}
/// <summary>
/// 初期化
/// </summary>
void LoadScene::Initialize()
{
	Sprite::LoadTexture(39, L"Resources/bosshp.png");
	loadsprite = Sprite::Create(39, { 0.0f,0.0f });
}

/// <summary>
/// 更新処理
/// </summary>
void LoadScene::Update()
{
	
	loadsprite->SetSize({ WinApp::window_width,WinApp::window_height });
}

/// <summary>
/// スプライト描画
/// </summary>
/// <param name="cmdList"></param>
void LoadScene::SpriteDraw()
{
	
}

/// <summary>
/// 描画
/// </summary>
/// <param name="cmdList"></param>
void LoadScene::Draw()
{


}
void LoadScene::Finalize()
{
	//delete postEffect;
	delete loadsprite;
}