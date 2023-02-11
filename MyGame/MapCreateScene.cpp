#include "MapCreateScene.h"
#include"Input.h"
#include"GrassObj.h"
#include"DirectXCommon.h"
#include"SceneManager.h"
#include"MobEnemy.h"
#include"BossEnemy.h"
#include"MapCreate.h"
#include"PlayScene.h"
#include"TitleScene.h"
#include"Field.h"
#include"CameraControl.h"
#include<fstream>
#include<string>
#include<sstream>
#include<ostream>
#include <iostream>
//シーンのコンストラクタ
MapCreateScene::MapCreateScene(SceneManager* sceneManager)
	: BaseScene(sceneManager)
{
}


#pragma region モデルとエフェクトとライトのインスタンス生成
void MapCreateScene::ModelCreate(DebugCamera* camera)
{
	postEffect = new PostEffect();
	postEffect->Initialize();

	Field::GetIns()->Initialize();

	// ライト生成
	lightGroup = LightGroup::Create();
	// 3Dオブエクトにライトをセット
	Object3d::SetLightGroup(lightGroup);
	GrassObj::SetLightGroup(lightGroup);
	//パラメータの設定
	//lightGroup->LightSetting();

	MapCreate::GetIns()->ObjectInitialize(CameraControl::GetIns()->GetCamera());
}
#pragma endregion


#pragma region オブジェクト+ライトの更新処理
void MapCreateScene::objUpdate(DebugCamera* camera)
{
	Field::GetIns()->Update_Edit();

	MapCreate::GetIns()->ObjectUpdate(CameraControl::GetIns()->GetCamera());
}
#pragma endregion

#pragma region 初期化
void MapCreateScene::Initialize()
{
	CameraControl::GetIns()->ParamSet();

	// 3Dオブジェクトにカメラをセット
	//	Object3d::SetCamera(CameraControl::GetIns()->GetCamera());
	ModelCreate(CameraControl::GetIns()->GetCamera()); //

	//カメラをセット
	f_Object3d::SetCamera(CameraControl::GetIns()->GetCamera());
	//グラフィックパイプライン生成
	f_Object3d::CreateGraphicsPipeline();

	//
	c_postEffect = Default;

	CameraPosition.z = -190;
	CameraPosition.y = 50;

	CameraControl::GetIns()->SetCameraState(CameraControl::MAPCREATE);
}
#pragma endregion

#pragma region 更新処理
void MapCreateScene::Update()
{
	//lightGroup->SpotLightUpdate();

	Input::MouseMove mouseMove = Input::GetIns()->GetMouseMove();
	//マウスの入力状態取得

	//カメラ関係の処理
	CameraControl::GetIns()->GetCamera()->SetEye(CameraPosition);
	CameraControl::GetIns()->GetCamera()->SetTarget({
		CameraPosition.x, CameraPosition.y - 15, CameraPosition.z + 20
	});

	CameraControl::GetIns()->GetCamera()->Update();

	objUpdate(CameraControl::GetIns()->GetCamera()); //オブジェクトの更新処理
	//Player::GetIns()->Update({ 1,1,1,1 }, camera);
	MapCreate::GetIns()->ObjectArgment(CameraControl::GetIns()->GetCamera());
	//シーンチェンジ
}
#pragma endregion

void MapCreateScene::LightUpdate()
{
}

//sプライと以外の描画
void MapCreateScene::MyGameDraw()
{
	Field::GetIns()->Draw();
	MapCreate::GetIns()->ObjectDraw();
}

void MapCreateScene::SpriteDraw()
{
}
#pragma endregion
//↓に入る
#pragma region 描画(imguiとスプライトとモデルまとめたもの)
void MapCreateScene::Draw()
{
	//ポストエフェクトの場合わけ(Bでぼかし Dがデフォルト)
	switch (c_postEffect)
	{
	case Blur: //ぼかし　描画準違うだけ
		postEffect->PreDrawScene();
		MyGameDraw();
		postEffect->PostDrawScene();

		DirectXCommon::GetIns()->BeginDraw();
		postEffect->Draw();
		if (DirectXCommon::GetIns()->GetFullScreen() == false)
		{
			ImGuiDraw();
		}
		DirectXCommon::GetIns()->EndDraw();
		break;

	case Default: //普通のやつ特に何もかかっていない
		postEffect->PreDrawScene();
		postEffect->Draw();
		postEffect->PostDrawScene();

		DirectXCommon::GetIns()->BeginDraw();
		MyGameDraw();
		if (DirectXCommon::GetIns()->GetFullScreen() == false)
		{
			ImGuiDraw();
		}
		DirectXCommon::GetIns()->EndDraw();
		break;
	}
}
#pragma endregion

void MapCreateScene::ImGuiDraw()
{
	MapCreate::GetIns()->ImGuiDraw();

	ImGui::Begin("SelectScene");
	ImGui::SetWindowPos(ImVec2(600, 800));
	ImGui::SetWindowSize(ImVec2(200, 250));

	if (ImGui::RadioButton("Scene_Create", t))
	{
		BaseScene* scene = new PlayScene(sceneManager_); //次のシーンのインスタンス生成
		sceneManager_->SetnextScene(scene); //シーンのセット
	}
	if (ImGui::RadioButton("Scene_Tittle", t))
	{
		BaseScene* scene = new TitleScene(sceneManager_); //次のシーンのインスタンス生成
		sceneManager_->SetnextScene(scene); //シーンのセット
	}
	ImGui::End();


	ImGui::Begin("CameraPosition");
	ImGui::SetWindowPos(ImVec2(0, 800));
	ImGui::SetWindowSize(ImVec2(600, 250));

	ImGui::SliderFloat("CameraPosx", &CameraPosition.x, -400, 400);
	ImGui::SliderFloat("CameraPosy", &CameraPosition.y, -200, 200);
	ImGui::SliderFloat("CameraPosz", &CameraPosition.z, -1600, 500);
	ImGui::End();
}
#pragma region 解放部分
void MapCreateScene::Finalize()
{
	Destroy(postEffect);
	Destroy(lightGroup);
	CameraControl::GetIns()->Finalize();
	MapCreate::GetIns()->Finalize();
	Field::GetIns()->Finalize();
}
#pragma endregion
