#include "Tutorial.h"
#include"Input.h"
#include"TitleScene.h"
#include"SceneManager.h"
#include"MapCreateScene.h"
#include"Field.h"
#include"HUD.h"
#include"DebugTxt.h"
#include"PlayerAttackState.h"
#include"SistemConfig.h"
#include"EnemyControl.h"
#include"WoodControl.h"
#include"FenceControl.h"
#include"ChestControl.h"
#include"CameraControl.h"
#include"UI.h"
#include"Effects.h"
#include"PlayScene.h"
#include"CustomButton.h"
#include"Feed.h"
#include"TutorialSprite.h"
#include"PlayerControl.h"
//シーンのコンストラクタ
Tutorial::Tutorial(SceneManager* sceneManager)
	:BaseScene(sceneManager)
{

}

#pragma region オブジェクト+ライトの更新処理
void Tutorial::objUpdate(DebugCamera*camera)
{
	CameraControl::GetInstance()->SetCameraState(CameraControl::PLAYER);

	
	if (Play) {
		AllObjectControl[1]->Update(CameraControl::GetInstance()->GetCamera());
		AllObjectControl[0]->Update(CameraControl::GetInstance()->GetCamera());
	for (int i = 2; i < AllObjectControl.size(); i++) {
		AllObjectControl[i]->Update(CameraControl::GetInstance()->GetCamera());
	}
	//PlayerAttackState::GetInstance()->Update();
	UI::GetInstance()->HUDUpdate(hudload, CameraControl::GetInstance()->GetCamera());
	TutorialSprite::GetInstance()->Update();
}
	Field::GetInstance()->Update(CameraControl::GetInstance()->GetCamera());
	CustomButton::GetInstance()->Update();
}

#pragma endregion

#pragma region 初期化
void Tutorial::Initialize()
{
	input = Input::GetInstance();
	//CameraControl::GetInstance()->SetCameraState(CameraControl::PLAYER);
	if (AllObjectControl.size() == 0) {
		AllObjectControl.push_back(CameraControl::GetInstance());
		AllObjectControl.push_back(PlayerControl::GetInstance());

		AllObjectControl.push_back(EnemyControl::GetInstance());
		AllObjectControl.push_back(FenceControl::GetInstance());
		AllObjectControl.push_back(ChestControl::GetInstance());
		AllObjectControl.push_back(WoodControl::GetInstance());
		
	}
	for (int i = 0; i < AllObjectControl.size(); i++) {
		AllObjectControl[i]->Initialize(CameraControl::GetInstance()->GetCamera());
	}
	TargetMarker::GetInstance()->Initialize();

	// 3Dオブジェクトにカメラをセット
	Object3d::SetCamera(CameraControl::GetInstance()->GetCamera());

	//カメラをセット
	f_Object3d::SetCamera(CameraControl::GetInstance()->GetCamera());
	//グラフィックパイプライン生成
	f_Object3d::CreateGraphicsPipeline();

	//Player::GetInstance()->Initialize(CameraControl::GetInstance()->GetCamera());
	UI::GetInstance()->Initialize();
	SistemConfig::GetInstance()->Initialize();
	CustomButton::GetInstance()->Initialize();
	Field::GetInstance()->Initialize(CameraControl::GetInstance()->GetCamera());
	TutorialSprite::GetInstance()->Initialize();
	postEffect = new PostEffect();
	postEffect->Initialize();
	Feed::GetInstance()->initialize();
}
#pragma endregion

#pragma region 更新処理
void Tutorial::Update()
{
	SistemConfig::GetInstance()->Update();

	objUpdate(CameraControl::GetInstance()->GetCamera());//オブジェクトの更新処理

	LoadParam(CameraControl::GetInstance()->GetCamera());
	
	//if (Player::GetInstance()->GetPosition().z > -470) {
		//scenechange = true;
	//}
	if (scenechange) {
		Feed::GetInstance()->Update_White(Feed::FEEDIN);
	}
	if (SistemConfig::GetInstance()->GetConfigJudgMent()) {
		c_postEffect = Blur;
	} else {
		c_postEffect = Default;
	}
	if (Feed::GetInstance()->GetAlpha()>=1.0f) {//押されたら
		BaseScene* scene = new PlayScene(sceneManager_);//次のシーンのインスタンス生成
		SceneManager::GetInstance()->SetScene(SceneManager::PLAY);
		sceneManager_->SetnextScene(scene);//シーンのセット
	}

}
#pragma endregion 

void Tutorial::MyGameDraw()
{
	Field::GetInstance()->Draw();

	//Player::GetInstance()->Draw();
	if (Play) {
		for (int i = 0; i < AllObjectControl.size(); i++) {
			AllObjectControl[i]->Draw();
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
		MyGameDraw();
		postEffect->PostDrawScene();

		DirectXCommon::GetInstance()->BeginDraw();
		postEffect->Draw();
		if (HUD::GetInstance()->GetLayOutMode()) {
			UI::GetInstance()->HUDDraw();
		}
		SistemConfig::GetInstance()->Draw();
		CustomButton::GetInstance()->Draw();
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
		UI::GetInstance()->HUDDraw();
		SistemConfig::GetInstance()->Draw();
		CustomButton::GetInstance()->Draw();
		Feed::GetInstance()->Draw();
		TutorialSprite::GetInstance()->Draw();
		
		if (DirectXCommon::GetInstance()->GetFullScreen() == false) {
			Player::GetInstance()->ImguiDraw();
			ImGuiDraw();
		}
		DirectXCommon::GetInstance()->EndDraw();
		break;
	}
}
#pragma endregion

void Tutorial::ImGuiDraw()
{
	{
		ImGui::Begin("Obj1");
		ImGui::SetWindowPos(ImVec2(0, 500));
		ImGui::SetWindowSize(ImVec2(500, 300));
		
		if (ImGui::TreeNode("Damage")) {
			//int d = PlayerAttackState::GetInstance()->GetDamage();
			//ImGui::SliderInt("positionX", &d, -100, 100);
			ImGui::TreePop();
		}
		ImGui::End();
	}
	{
		ImGui::Begin("None");
		if (ImGui::Button("Load", ImVec2(70, 50))) {
			LoadEnemy = true;
		}
		ImGui::End();
	}
	//
	
	{//カメラ
		bool defaultPos;
		if (ImGui::RadioButton("DefaultPosition", &defaultPos)) {
			CameraDis = 25;
			CameraHeight = 9;
		}
		ImGui::SliderFloat("rotationX", &cameraAngle, -360, 360);

		ImGui::End();
	}
	ImGui::Begin("Scene");

	if (ImGui::RadioButton("Scene_Create", t)) {
		BaseScene* scene = new MapCreateScene(sceneManager_);//次のシーンのインスタンス生成
		sceneManager_->SetnextScene(scene);//シーンのセット
	}
	if (ImGui::RadioButton("Scene_Tittle", y)) {
		BaseScene* scene = new PlayScene(sceneManager_);//次のシーンのインスタンス生成
		SceneManager::GetInstance()->SetScene(SceneManager::PLAY);
		sceneManager_->SetnextScene(scene);//シーンのセット
	}
	ImGui::End();
	//
	{
		unsigned long current_time = timeGetTime();
		float fps = float(count_frame) / (current_time - prev_time) * 1000;
		ImGui::SliderFloat("FPS", &fps, -10, 50);
		count_frame++;
		ImGui::End();
	}
}

bool Tutorial::LoadParam(DebugCamera* camera)
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

void Tutorial::Finalize()
{
	//SistemConfig::GetInstance()->~SistemConfig();
	delete CameraControl::GetInstance()->GetCamera();
	delete postEffect, lightGroup;
	for (int i = 0; i < AllObjectControl.size(); i++) {
		//delete AllObjectControl[i];
	}
}