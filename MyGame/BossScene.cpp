#include "BossScene.h"
#include"TitleScene.h"
#include"SceneManager.h"
#include"Field.h"
#include"SistemConfig.h"
#include"EnemyControl.h"
#include"WoodControl.h"
#include"FenceControl.h"
#include"ChestControl.h"
#include"CameraControl.h"
#include"UI.h"
#include"PlayScene.h"
#include"Feed.h"
#include"PlayerControl.h"
#include"DamageManager.h"
BossScene::BossScene(SceneManager* sceneManager)
	:BaseScene(sceneManager)
{

}

/*------------------------*/
/*-------初期化処理-------*/
/*-----------------------*/
void BossScene::Initialize()
{
	DebugTxt::GetInstance()->Initialize(47);
	//各オブジェクトの初期化
	if (AllObjectControl.size() == 0) {//各オブジェクトインスタンスぶちこむ
		AllObjectControl.push_back(CameraControl::GetInstance());
		AllObjectControl.push_back(PlayerControl::GetInstance());
		AllObjectControl.push_back(EnemyControl::GetInstance());
	}
	for (int i = 0; i < AllObjectControl.size(); i++) {
		AllObjectControl[i]->Initialize(CameraControl::GetInstance()->GetCamera());
	}

	Field::GetInstance()->Initialize(CameraControl::GetInstance()->GetCamera());
	
	//ボス攻撃用->できれば移す
	Nail::GetInstance()->ModelSet();
	
	postEffect = new MinimapSprite();
	postEffect->Initialize();
	//カメラをセット
	f_Object3d::SetCamera(CameraControl::GetInstance()->GetCamera());
	//グラフィックパイプライン生成
	f_Object3d::CreateGraphicsPipeline();

	//カメラ挙動をボスカットシーン
	CameraControl::GetInstance()->SetCameraState(CameraControl::BOSSCUTSCENE);
	dc = new DebugCamera(WinApp::window_width, WinApp::window_height);

}

/*------------------------*/
/*--------更新処理--------*/
/*-----------------------*/
void BossScene::Update()
{
	//読み込み
	if (!LoadEnemy&&!Play) {
		LoadEnemy = true;
	}

	SistemConfig::GetInstance()->Update();

	if (Play) {//csvからの読み込み終わってから更新処理
		//1->Player  0->Camera カメラの注視点Playerに合わすのでPlayerが先
		AllObjectControl[1]->Update(CameraControl::GetInstance()->GetCamera());
		AllObjectControl[0]->Update(CameraControl::GetInstance()->GetCamera());
		for (int i = 2; i < AllObjectControl.size(); i++) {
			AllObjectControl[i]->Update(CameraControl::GetInstance()->GetCamera());
		}
		Nail::GetInstance()->Update();
		UI::GetInstance()->HUDUpdate(hudload, CameraControl::GetInstance()->GetCamera());
		}
	dc->Update();

	dc->SetTarget({ CameraControl::GetInstance()->GetCamera()->GetTarget() });
	dc->SetEye({ PlayerControl::GetInstance()->GetPlayer()->GetPosition().x,
		 PlayerControl::GetInstance()->GetPlayer()->GetPosition().y + 300.0f,
		PlayerControl::GetInstance()->GetPlayer()->GetPosition().z - 1 });
	Field::GetInstance()->SetCamera(dc);

	Field::GetInstance()->Update(CameraControl::GetInstance()->GetCamera());
	DamageManager::GetIns()->Upda();

	//各オブジェクトの更新処理
	//csv読み込み部分(Cameraの更新後にするのでobjUpdate()挟んでから)
	LoadParam(CameraControl::GetInstance()->GetCamera());
	if (scenechange && Feed::GetInstance()->GetAlpha() >= 1.0f) {//画面真っ白なったら
		BaseScene* scene = new BossScene(sceneManager_);//次のシーンのインスタンス生成
		Play = false;
		SceneManager::GetInstance()->SetScene(SceneManager::BOSS);
		sceneManager_->SetnextScene(scene);//シーンのセット
	}
	if (scenechange) {
		Feed::GetInstance()->Update_White(Feed::FEEDIN);//白くなります
	}
	if (SistemConfig::GetInstance()->GetConfigJudgMent()) {
		c_postEffect = Blur;
	} else {
		c_postEffect = Default;
	}
}

/*------------------------*/
/*--------描画処理--------*/
/*-----------------------*/
void BossScene::MyGameDraw()
{
	Field::GetInstance()->Draw();
	if (Play) {
		for (int i = 0; i < AllObjectControl.size(); i++) {
			AllObjectControl[i]->Draw();
		}
	}
}

/*------------------------*/
/*--------描画処理--------*/
/*-----------------------*/
void BossScene::Draw()
{
	//ポストエフェクトの場合わけ(Bでぼかし Dがデフォルト)
	switch (c_postEffect)
	{
	case Blur://ぼかし　描画準違うだけ
		postEffect->PreDrawScene();
		//MyGameDraw();
		postEffect->PostDrawScene();

		DirectXCommon::GetInstance()->BeginDraw();
	//	postEffect->Draw();
		SistemConfig::GetInstance()->SwordPedestalDraw();

		//UI
		if (HUD::GetInstance()->GetLayOutMode()) {
			UI::GetInstance()->HUDDraw();
		}
		//設定画面
		SistemConfig::GetInstance()->Draw();
		
		DirectXCommon::GetInstance()->EndDraw();
		break;

	case Default://普通のやつ特に何もかかっていない
		postEffect->PreDrawScene();

		Field::GetInstance()->MiniFieldDraw();

		//Field::GetInstance()->Draw();

		postEffect->PostDrawScene();

		DirectXCommon::GetInstance()->BeginDraw();
		MyGameDraw();
		DamageManager::GetIns()->Draw();

		postEffect->Draw();
		//UI
		if (CameraControl::GetInstance()->GetCameraState() != CameraControl::BOSSCUTSCENE) {
			UI::GetInstance()->HUDDraw();
		}
		Feed::GetInstance()->Draw();
		Field::GetInstance()->WarningDraw();
		SistemConfig::GetInstance()->Draw();

		DirectXCommon::GetInstance()->EndDraw();
		break;
	}
}

/*------------------------*/
/*--------読込処理--------*/
/*-----------------------*/
bool BossScene::LoadParam(DebugCamera* camera)
{
	if (LoadEnemy) {
		for (int i = 0; i < AllObjectControl.size(); i++) {
			AllObjectControl[i]->Load(CameraControl::GetInstance()->GetCamera());
		}
		hudload = true;
		Play = true;
		LoadEnemy = false;
	}
	return true;
}

/*------------------------*/
/*--------解放処理--------*/
/*-----------------------*/
void BossScene::Finalize()
{
	UI::GetInstance()->Finalize();
	SistemConfig::GetInstance()->Finalize();
	AttackCollision::GetInstance()->Finalize();
	AllObjectControl.clear();
	Field::GetInstance()->Finalize();
}