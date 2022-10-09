#include "Tutorial.h"
#include"Input.h"
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
#include"BossScene.h"
#include"Feed.h"
#include"PlayerControl.h"
#include"PlayScene.h"
#include"DamageManager.h"
//シーンのコンストラクタ
Tutorial::Tutorial(SceneManager* sceneManager)
	:BaseScene(sceneManager)
{

}

#pragma region オブジェクト+ライトの更新処理
void Tutorial::objUpdate(DebugCamera* camera)
{
	//カメラの注視点をプレイヤーにセット
	CameraControl::GetInstance()->SetCameraState(CameraControl::PLAYER);
	if (!Load && !Play) {
		Load = true;
	}
	if (Play) {//csvからの読み込み終わってから更新処理
		//1->Player  0->Camera カメラの注視点Playerに合わすのでPlayerが先
		AllObjectControl[1]->Update(CameraControl::GetInstance()->GetCamera());
		AllObjectControl[0]->Update(CameraControl::GetInstance()->GetCamera());
		for (int i = 2; i < AllObjectControl.size(); i++) {
			if (AllObjectControl[i] != nullptr) {
				AllObjectControl[i]->Update(CameraControl::GetInstance()->GetCamera());
			}
		}
		UI::GetInstance()->HUDUpdate(hudload, CameraControl::GetInstance()->GetCamera());
		}
	
	dc->Update();
	
	dc->SetTarget({ CameraControl::GetInstance()->GetCamera()->GetTarget()});
	dc->SetEye({ PlayerControl::GetInstance()->GetPlayer()->GetPosition().x,
		 PlayerControl::GetInstance()->GetPlayer()->GetPosition().y+300.0f,
		PlayerControl::GetInstance()->GetPlayer()->GetPosition().z-1});
	Field::GetInstance()->SetCamera(dc);

	Field::GetInstance()->Update(CameraControl::GetInstance()->GetCamera());
}

#pragma endregion

#pragma region 初期化
void Tutorial::Initialize()
{
	Texture::LoadTexture(47, L"Resources/df.png");
	{
		AllObjectControl.push_back(CameraControl::GetInstance());//Camera
		AllObjectControl.push_back(PlayerControl::GetInstance());//Player
		AllObjectControl.push_back(EnemyControl::GetInstance());//Enemy
		AllObjectControl.push_back(FenceControl::GetInstance());//Fence
		AllObjectControl.push_back(ChestControl::GetInstance());//Chest
		AllObjectControl.push_back(WoodControl::GetInstance());//Wood
	//初期化部
	for (int i = 0; i < AllObjectControl.size(); i++) {//初期化
		AllObjectControl[i]->Initialize(CameraControl::GetInstance()->GetCamera());
	}
	}
	
	Field::GetInstance()->Initialize(CameraControl::GetInstance()->GetCamera());

	// 3Dオブジェクトにカメラをセット
	//Object3d::SetCamera(CameraControl::GetInstance()->GetCamera());
	//カメラをセット
	f_Object3d::SetCamera(CameraControl::GetInstance()->GetCamera());
	//グラフィックパイプライン生成
	f_Object3d::CreateGraphicsPipeline();

	//フェードとUIスプライト初期化
	Feed::GetInstance()->initialize();
	UI::GetInstance()->Initialize();

	postEffect = new MinimapSprite();
	postEffect->Initialize();
	//Field::GetInstance()->SetCamera(dc);
	//postEffect->SetPosition({ 100,0 });
	dc = new DebugCamera(WinApp::window_width, WinApp::window_height);
	//Field::GetInstance()->SetCamera(dc);

	SistemConfig::GetInstance()->Initialize();
}
#pragma endregion

#pragma region 更新処理
void Tutorial::Update()
{
	

	
	
	SistemConfig::GetInstance()->Update();
	//各オブジェクトの更新処理
	objUpdate(CameraControl::GetInstance()->GetCamera());//オブジェクトの更新処理
	//csv読み込み部分(Cameraの更新後にするのでobjUpdate()挟んでから)
	LoadParam(CameraControl::GetInstance()->GetCamera());

	//一定数進んだらシーンチェンジ
	bool ArrivalJudg = PlayerControl::GetInstance()->GetPlayer()->GetPosition().z > -470.0f;
	if (ArrivalJudg) {
		scenechange = true;
	}

	if (scenechange) {
		Feed::GetInstance()->Update_White(Feed::FEEDIN);//白くなります
	}
	if (SistemConfig::GetInstance()->GetConfigJudgMent()) {
		c_postEffect = Blur;
	} else {
		c_postEffect = Default;
	}
	//postEffect->SetSize({ 10,10 });
	
if (scenechange&& Feed::GetInstance()->GetAlpha() >= 1.0f) {//画面真っ白なったら
		BaseScene* scene = new BossScene(sceneManager_);//次のシーンのインスタンス生成
		Play = false;
		SceneManager::GetInstance()->SetScene(SceneManager::BOSS);
		sceneManager_->SetnextScene(scene);//シーンのセット
	}
	DamageManager::GetIns()->Upda();

}
#pragma endregion 

void Tutorial::MyGameDraw()
{
	if (Play) {
		for (int i = 0; i < AllObjectControl.size(); i++) {
			if (AllObjectControl[i] != nullptr) {
				AllObjectControl[i]->Draw();
			}
		}
	}
	
}

void Tutorial::Draw()
{
	//ポストエフェクトの場合わけ(Bでぼかし Dがデフォルト)
	switch (c_postEffect)
	{
	case Blur://ぼかし　描画準違うだけ
		postEffect->PreDrawScene();
		SistemConfig::GetInstance()->SwordPedestalDraw();

		postEffect->PostDrawScene();

		DirectXCommon::GetInstance()->BeginDraw();
		//postEffect->Draw();
		SistemConfig::GetInstance()->SwordPedestalDraw();


		//MyGameDraw();
		
		if (HUD::GetInstance()->GetLayOutMode()) {
			UI::GetInstance()->HUDDraw();
		}
		SistemConfig::GetInstance()->Draw();
	
		DirectXCommon::GetInstance()->EndDraw();

		
		break;

	case Default://普通のやつ特に何もかかっていない
		
		postEffect->PreDrawScene();
		
		Field::GetInstance()->MiniFieldDraw();

	//Field::GetInstance()->Draw();

		postEffect->PostDrawScene();

		DirectXCommon::GetInstance()->BeginDraw();
	
		Field::GetInstance()->Draw();

		MyGameDraw();
		postEffect->Draw();
		//Sprite::PreDraw();

	//	Sprite::PostDraw();
		
		//	MyGameDraw();
		UI::GetInstance()->HUDDraw();
		Feed::GetInstance()->Draw();
		SistemConfig::GetInstance()->Draw();
//		DirectXCommon::GetInstance()->ClearDepthBuffer(DirectXCommon::GetInstance()->GetCmdList());

		DirectXCommon::GetInstance()->EndDraw();
		break;
	}
}
#pragma endregion


bool Tutorial::LoadParam(DebugCamera* camera)
{
	if (Load) {
		for (int i = 0; i < AllObjectControl.size(); i++) {
			AllObjectControl[i]->Load(CameraControl::GetInstance()->GetCamera());
		}
		hudload = true;
		Play = true;
		Load = false;
	}
	return true;
}

void Tutorial::Finalize()
{
	for (int i = 0; i < AllObjectControl.size(); i++) {//初期化
		AllObjectControl[i]->Finalize();
	}
	AllObjectControl.clear();
	Field::GetInstance()->Finalize();
	Destroy(postEffect);
}
