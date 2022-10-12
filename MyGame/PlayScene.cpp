#include "PlayScene.h"
#include"Input.h"
#include"DirectXCommon.h"
#include"TitleScene.h"
#include"SceneManager.h"
#include"MapCreateScene.h"
#include"Field.h"
#include"PlayerAttackState.h"
#include"SistemConfig.h"
#include"EnemyControl.h"
#include"WoodControl.h"
#include"FenceControl.h"
#include"ChestControl.h"
#include"CameraControl.h"
#include"UI.h"
#include"CustomButton.h"
#include"Feed.h"
#include"PlayerControl.h"
#include"BossScene.h"
#include"SelectSword.h"
//シーンのコンストラクタ
PlayScene::PlayScene(SceneManager* sceneManager)
	:BaseScene(sceneManager)
{

}

/*------------------------*/
/*--------初期化処理-------*/
/*-----------------------*/
void PlayScene::Initialize()
{
	if (AllObjectControl.size() == 0) {
		//カメラ一番上に　他のControlがカメラを引数にしてるから
		AllObjectControl.push_back(CameraControl::GetInstance());
		AllObjectControl.push_back(PlayerControl::GetInstance());
		AllObjectControl.push_back(EnemyControl::GetInstance());
		AllObjectControl.push_back(WoodControl::GetInstance());
		AllObjectControl.push_back(FenceControl::GetInstance());
		AllObjectControl.push_back(ChestControl::GetInstance());
	}

	//各オブジェクト初期化
	Field::GetInstance()->Initialize((CameraControl::GetInstance()->GetCamera()));

	for (int i = 0; i < AllObjectControl.size(); i++) {
		AllObjectControl[i]->Initialize((CameraControl::GetInstance()->GetCamera()));
	}
	//カメラをセット
	f_Object3d::SetCamera(CameraControl::GetInstance()->GetCamera());
	//グラフィックパイプライン生成
	f_Object3d::CreateGraphicsPipeline();

	//ポストエフェクト初期化
	postEffect = new PostEffect();
	postEffect->Initialize();

	//カメラ挙動をプレイカットシーン
	CameraControl::GetInstance()->SetCameraState(CameraControl::PLAYCUTSCENE);

}

/*------------------------*/
/*--------更新処理--------*/
/*-----------------------*/
void PlayScene::objUpdate(DebugCamera* camera)
{
	//PlayerControl::GetInstance()->GetPlayer()->SetPosition({ 150.0f,-42.0f,-379.0f });
	Field::GetInstance()->Update((CameraControl::GetInstance()->GetCamera()));
	if (PlayGame) {
		AllObjectControl[1]->Update(CameraControl::GetInstance()->GetCamera());
		AllObjectControl[0]->Update(CameraControl::GetInstance()->GetCamera());

		for (int i = 2; i < AllObjectControl.size(); i++) {
			AllObjectControl[i]->Update((CameraControl::GetInstance()->GetCamera()));
		}
	}
	UI::GetInstance()->HUDUpdate(hudload, (CameraControl::GetInstance()->GetCamera()));

	
}

/*------------------------*/
/*--------更新処理--------*/
/*-----------------------*/
void PlayScene::Update()
{
	SistemConfig::GetInstance()->Update();

	if (!Load && !PlayGame) {
		Load = true;
	}
	
	//オブジェクトの更新処理
	objUpdate((CameraControl::GetInstance()->GetCamera()));

	//csv読み込み
	LoadParam((CameraControl::GetInstance()->GetCamera()));

	//設定画面中ポストエフェクト（今は色反転のみ）
	if (SistemConfig::GetInstance()->GetConfigJudgMent()) {
		c_postEffect = Blur;
	} else {
		c_postEffect = Default;
	}
	if (CameraControl::GetInstance()->GetMoveBosAreaCam() == CameraControl::TARGETPLAYER) {
		if (Feed::GetInstance()->GetAlpha() >= 1.0f) {//画面真っ白なったら
			BaseScene* scene = new BossScene(sceneManager_);//次のシーンのインスタンス生成
			SceneManager::GetInstance()->SetScene(SceneManager::BOSS);
			sceneManager_->SetnextScene(scene);//シーンのセット
		}
	}
}

/*------------------------*/
/*--------描画処理--------*/
/*-----------------------*/
void PlayScene::MyGameDraw()
{

	Field::GetInstance()->Draw();
//	if (EnemyControl::GetInstance()->GetQuentity() > 1) {
		for (int i = 0; i < AllObjectControl.size(); i++) {
			AllObjectControl[i]->Draw();
		}
	//}
}

/*------------------------*/
/*--------描画処理--------*/
/*-----------------------*/
void PlayScene::Draw()
{
	//ポストエフェクトの場合わけ(Bでぼかし Dがデフォルト)
	switch (c_postEffect)
	{
	case Blur://ぼかし　描画準違うだけ
		postEffect->PreDrawScene();
		MyGameDraw();
		postEffect->PostDrawScene();

		DirectXCommon::GetInstance()->BeginDraw();
		postEffect->Draw();
		if (HUD::GetInstance()->GetLayOutMode()) {
			UI::GetInstance()->HUDDraw();
		}
		SistemConfig::GetInstance()->Draw();
		
		DirectXCommon::GetInstance()->EndDraw();
		break;

	case Default://普通のやつ特に何もかかっていない
		postEffect->PreDrawScene();
		postEffect->Draw();
		postEffect->PostDrawScene();

		DirectXCommon::GetInstance()->BeginDraw();
		MyGameDraw();
		SistemConfig::GetInstance()->Draw();
	
		Feed::GetInstance()->Draw();
			if (Feed::GetInstance()->GetAlpha() <= 0.0f) {
				UI::GetInstance()->HUDDraw();
			}
		
		UI::GetInstance()->AreaNameDraw();
		AttackCollision::GetInstance()->Draw();
		DirectXCommon::GetInstance()->EndDraw();
		break;
	}
}

/*------------------------*/
/*--------読込処理--------*/
/*-----------------------*/
void PlayScene::LoadParam(DebugCamera* camera)
{
	if (Load) {
		for (int i = 0; i < AllObjectControl.size(); i++) {
			AllObjectControl[i]->Load((CameraControl::GetInstance()->GetCamera()));
		}
		hudload = true;
		Load = false;
		PlayGame = true;
	}
}

/*------------------------*/
/*--------解放処理--------*/
/*-----------------------*/
void PlayScene::Finalize()
{
	for (int i = 0; i < AllObjectControl.size(); i++) {
		AllObjectControl[i]->Finalize();
	}
	Field::GetInstance()->Finalize();
	AllObjectControl.clear();
}