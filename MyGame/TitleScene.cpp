#include "TitleScene.h"
#include"Input.h"
#include"PlayScene.h"
#include"MapCreateScene.h"
#include"SceneManager.h"
#include"Tutorial.h"
#include"Feed.h"
#include"BossScene.h"
#include"UI.h"
TitleScene::TitleScene(SceneManager* sceneManager)
	:BaseScene(sceneManager)
{

}

/*------------------------*/
/*-------初期化処理--------*/
/*-----------------------*/
void TitleScene::Initialize()
{
	Sprite::LoadTexture(3, L"Resources/title.png");
	titlesprite = Sprite::Create(3, { 0.0f,0.0f });

	Sprite::LoadTexture(4, L"Resources/title2.png");
	titlesprite2 = Sprite::Create(4, {  WinApp::window_width / 2,0.0f });
	Feed::GetInstance()->initialize();
}

/*------------------------*/
/*-------更新処理--------*/
/*-----------------------*/
void TitleScene::Update()
{

	if (Input::GetInstance()->TriggerButton(Input::Button_A)) {//押されたら
		BaseScene* scene = new MapCreateScene(sceneManager_);//次のシーンのインスタンス生成
		SceneManager::GetInstance()->SetScene(SceneManager::MAPCREATE);
		sceneManager_->SetnextScene(scene);//シーンのセット
	}
	if (Input::GetInstance()->TriggerButton(Input::Button_B)) {//押されたら
		BaseScene* scene = new Tutorial(sceneManager_);//次のシーンのインスタンス生成
		SceneManager::GetInstance()->SetScene(SceneManager::TUTORIAL);
		sceneManager_->SetnextScene(scene);//シーンのセット
	}
	//titlesprite2->SetRotation(180);
	titlesprite->SetSize({ WinApp::window_width/2,WinApp::window_height });
	titlesprite2->SetSize({ WinApp::window_width / 2,WinApp::window_height });
}

/*------------------------*/
/*-------描画処理--------*/
/*-----------------------*/
void TitleScene::SpriteDraw()
{
	Sprite::PreDraw();
	titlesprite2->Draw();
	titlesprite->Draw();
	Sprite::PostDraw();
}

void TitleScene::Draw()
{
	//ポストエフェクトの描画
	DirectXCommon::GetInstance()->BeginDraw();//描画コマンドの上らへんに
	SpriteDraw();
	DirectXCommon::GetInstance()->EndDraw();
}


/*------------------------*/
/*-------解放処理--------*/
/*-----------------------*/
void TitleScene::Finalize()
{
	//delete postEffect;
	delete titlesprite;
}