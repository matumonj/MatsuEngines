#include "TitleScene.h"
#include"Input.h"
#include"PlayScene.h"
#include"MapCreateScene.h"
#include"SceneManager.h"
#include"Tutorial.h"
#include"Feed.h"
#include"BossScene.h"
TitleScene::TitleScene(SceneManager* sceneManager)
	:BaseScene(sceneManager)
{

}
/// <summary>
/// 初期化
/// </summary>
void TitleScene::Initialize()
{
	Sprite::LoadTexture(3, L"Resources/background.png");
	titlesprite = Sprite::Create(3, { 0.0f,0.0f });
	Feed::GetInstance()->initialize();
}

/// <summary>
/// 更新処理
/// </summary>
void TitleScene::Update()
{
	//ENTERで次のシーンへ
	if (Input::GetInstance()->TriggerButton(Input::Button_B)) {//押されたら
		BaseScene* scene = new BossScene(sceneManager_);//次のシーンのインスタンス生成
		SceneManager::GetInstance()->SetScene(SceneManager::BOSS);
		sceneManager_->SetnextScene(scene);//シーンのセット
	}
	titlesprite->SetSize({ WinApp::window_width,WinApp::window_height });
}

/// <summary>
/// スプライト描画
/// </summary>
/// <param name="cmdList"></param>
void TitleScene::SpriteDraw()
{
	Sprite::PreDraw();
	titlesprite->Draw();
	Sprite::PostDraw();
}

/// <summary>
/// 描画
/// </summary>
/// <param name="cmdList"></param>
void TitleScene::Draw()
{
	//ポストエフェクトの描画
	DirectXCommon::GetInstance()->BeginDraw();//描画コマンドの上らへんに
	SpriteDraw();
	ImGui::Begin("Scene");

	if (ImGui::RadioButton("Scene_Play", t)) {

		BaseScene* scene = new BossScene(sceneManager_);//次のシーンのインスタンス生成
		SceneManager::GetInstance()->SetScene(SceneManager::BOSS);
		sceneManager_->SetnextScene(scene);//シーンのセット
	}
	if (ImGui::RadioButton("Scene_MapCreate", y)) {
		BaseScene* scene = new MapCreateScene(sceneManager_);//次のシーンのインスタンス生成
		sceneManager_->SetnextScene(scene);//シーンのセット
	}
	ImGui::End();
	DirectXCommon::GetInstance()->EndDraw();

}
void TitleScene::Finalize()
{
	//delete postEffect;
	delete titlesprite;
}