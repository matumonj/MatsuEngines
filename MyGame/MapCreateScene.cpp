#include "MapCreateScene.h"
#include"Input.h"
#include"GrassObj.h"
#include"DirectXCommon.h"
#include"SceneManager.h"
#include"MapCreate.h"
#include"PlayScene.h"
#include"TitleScene.h"
#include"Field.h"
#include"CameraControl.h"
//シーンのコンストラクタ
MapCreateScene::MapCreateScene(SceneManager* sceneManager)
	: BaseScene(sceneManager)
{
}


#pragma region モデルとエフェクトとライトのインスタンス生成
void MapCreateScene::ModelCreate(DebugCamera* camera)
{
	Field::GetIns()->Initialize();

	// ライト生成
	lightGroup = LightGroup::Create();
	// 3Dオブエクトにライトをセット
	Object3d::SetLightGroup(lightGroup);
	GrassObj::SetLightGroup(lightGroup);
	//パラメータの設定
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
	ModelCreate(CameraControl::GetIns()->GetCamera()); //

	//カメラをセット
	f_Object3d::SetCamera(CameraControl::GetIns()->GetCamera());
	//グラフィックパイプライン生成
	f_Object3d::CreateGraphicsPipeline();

	CameraPosition.z = -190;
	CameraPosition.y = 50;

	CameraControl::GetIns()->SetCameraState(CameraControl::MAPCREATE);
}
#pragma endregion

#pragma region 更新処理
void MapCreateScene::Update()
{
	Input::MouseMove mouseMove = Input::GetIns()->GetMouseMove();
	
	//カメラ関係の処理
	CameraControl::GetIns()->GetCamera()->SetEye(CameraPosition);
	CameraControl::GetIns()->GetCamera()->SetTarget({
		CameraPosition.x, CameraPosition.y - 15, CameraPosition.z + 20
	});
	CameraControl::GetIns()->GetCamera()->Update();

	//オブジェクトの更新処理
	objUpdate(CameraControl::GetIns()->GetCamera());
	//配置管理用
	MapCreate::GetIns()->ObjectArgment(CameraControl::GetIns()->GetCamera());
}
#pragma endregion

void MapCreateScene::LightUpdate()
{
}

void MapCreateScene::SpriteDraw()
{
	
}

//スプライと以外の描画
void MapCreateScene::MyGameDraw()
{
	Field::GetIns()->Draw();
	MapCreate::GetIns()->ObjectDraw();
}

//↓に入る
#pragma region 描画(imguiとスプライトとモデルまとめたもの)
void MapCreateScene::Draw()
{
		DirectXCommon::GetIns()->BeginDraw();
		MyGameDraw();
		if (DirectXCommon::GetIns()->GetFullScreen() == false)
		{
			ImGuiDraw();
		}
		DirectXCommon::GetIns()->EndDraw();

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
