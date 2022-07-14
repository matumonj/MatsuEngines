#include "MapCreateScene.h"
#include"Input.h"
#include"DirectXCommon.h"
#include"SceneManager.h"
#include"MobEnemy.h"
#include"BossEnemy.h"
#include"MeshCollider.h"
#include"TouchableObject.h"
#include"CollisionManager.h"
#include"SphereCollider.h"
#include"MapCreate.h"
#include"PlayScene.h"
#include"TitleScene.h"
#include"Field.h"
#include"PlayerAttackState.h"
#include<fstream>
#include<string>
#include<sstream>
#include<ostream>
#include <iostream>
//シーンのコンストラクタ
MapCreateScene::MapCreateScene(SceneManager* sceneManager)
	:BaseScene(sceneManager)
{

}


#pragma region モデルとエフェクトとライトのインスタンス生成
void MapCreateScene::ModelCreate(DebugCamera* camera)
{
	
	postEffect = new PostEffect();
	postEffect->Initialize();
	
	Field::GetInstance()->Initialize(camera);

	// ライト生成
	lightGroup = LightGroup::Create();
	// 3Dオブエクトにライトをセット
	Object3d::SetLightGroup(lightGroup);
	//パラメータの設定
	lightGroup->LightSetting();

	MapCreate::SetBoxModel(camera);

}
#pragma endregion


#pragma region オブジェクト+ライトの更新処理
void MapCreateScene::objUpdate(DebugCamera* camera)
{
	Field::GetInstance()->Update(camera);

	MapCreate::UpdateBoxModel(camera);
}
#pragma endregion

#pragma region 初期化
void MapCreateScene::Initialize()
{
	
	// カメラ生成
	camera = new DebugCamera(WinApp::window_width, WinApp::window_height/*input*/);
	// 3Dオブジェクトにカメラをセット
	Object3d::SetCamera(camera);
	camera->SetEye({ 0,0,0 });
	ModelCreate(camera);//

	//カメラをセット
	f_Object3d::SetCamera(camera);
	//グラフィックパイプライン生成
	f_Object3d::CreateGraphicsPipeline();

	collisionManager = CollisionManager::GetInstance();
	//
	c_postEffect = Default;

}
#pragma endregion

#pragma region 更新処理
void MapCreateScene::Update()
{
	lightGroup->SpotLightUpdate();

	Input::MouseMove mouseMove = Input::GetInstance()->GetMouseMove();
	//マウスの入力状態取得
	
	//カメラ関係の処理
	camera->SetDistance(0);
	camera->SetEye({0,20,-100});
	camera->SetTarget({0,0,0});
	camera->Update();


	objUpdate(camera);//オブジェクトの更新処理
	//Player::GetInstance()->Update({ 1,1,1,1 }, camera);
	MapCreate::EnemyArgment(camera);
	//シーンチェンジ

	if (Input::GetInstance()->TriggerKey(DIK_R)) {//押されたら
		//BaseScene* scene = new PlayScene(sceneManager_);//次のシーンのインスタンス生成
		//sceneManager_->SetnextScene(scene);//シーンのセット
	}

}
#pragma endregion 

//スプライトの描画
#pragma region モデルの描画
void MapCreateScene::SpriteDraw()
{

	Field::GetInstance()->Draw();

	MapCreate::EnemyDraw();

	MapCreate::DrawBoxModel();
	
}
//sプライと以外の描画
void MapCreateScene::MyGameDraw()
{
	SpriteDraw();

}
#pragma endregion
//↓に入る
#pragma region 描画(imguiとスプライトとモデルまとめたもの)
void MapCreateScene::Draw()
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
		if (DirectXCommon::GetInstance()->GetFullScreen() == false) {
			ImGuiDraw();
		}
		DirectXCommon::GetInstance()->EndDraw();
		break;

	case Default://普通のやつ特に何もかかっていない
		postEffect->PreDrawScene();
		postEffect->Draw();
		postEffect->PostDrawScene();

		DirectXCommon::GetInstance()->BeginDraw();
		MyGameDraw();
		if (DirectXCommon::GetInstance()->GetFullScreen() == false) {
			ImGuiDraw();
		}
		DirectXCommon::GetInstance()->EndDraw();
		break;
	}
}
#pragma endregion

void MapCreateScene::ImGuiDraw()
{
	MapCreate::ImguiDraw_Enemy();

	if (ImGui::RadioButton("Scene_Create", t)) {
		BaseScene* scene = new PlayScene(sceneManager_);//次のシーンのインスタンス生成
		sceneManager_->SetnextScene(scene);//シーンのセット
	}
	if (ImGui::RadioButton("Scene_Tittle", t)) {
		BaseScene* scene = new TitleScene(sceneManager_);//次のシーンのインスタンス生成
		sceneManager_->SetnextScene(scene);//シーンのセット
	}
	ImGui::End();
}
#pragma region 解放部分
void MapCreateScene::Finalize()
{
}
#pragma endregion