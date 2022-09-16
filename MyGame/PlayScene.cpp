#include "PlayScene.h"
#include"Input.h"
#include"DirectXCommon.h"
#include"TitleScene.h"
#include"SceneManager.h"
#include"MapCreateScene.h"
#include"Field.h"
#include"HUD.h"
#include"PlayerAttackState.h"
#include"SistemConfig.h"
#include"EnemyControl.h"
#include"WoodControl.h"
#include"FenceControl.h"
#include"ChestControl.h"
#include"CameraControl.h"
#include"UI.h"
#include"Effects.h"
#include"CustomButton.h"
#include"Feed.h"
#include"PlayerControl.h"
//シーンのコンストラクタ
PlayScene::PlayScene(SceneManager* sceneManager)
	:BaseScene(sceneManager)
{

}

#pragma region オブジェクト+ライトの更新処理
void PlayScene::objUpdate(DebugCamera*camera)
{
	for (int i = 0; i < AllObjectControl.size(); i++) {
			AllObjectControl[i]->Update((CameraControl::GetInstance()->GetCamera()));
		}
		PlayerAttackState::GetInstance()->Update();
		UI::GetInstance()->HUDUpdate(hudload, (CameraControl::GetInstance()->GetCamera()));

	Field::GetInstance()->Update((CameraControl::GetInstance()->GetCamera()));
	CustomButton::GetInstance()->Update();
}

#pragma endregion

#pragma region 初期化
void PlayScene::Initialize()
{
	input = Input::GetInstance();

	if (AllObjectControl.size() == 0) {
		//カメラ一番上に　他のControlがカメラを引数にしてるから
		AllObjectControl.push_back(CameraControl::GetInstance());
		AllObjectControl.push_back(WoodControl::GetInstance());
		AllObjectControl.push_back(FenceControl::GetInstance());
		AllObjectControl.push_back(EnemyControl::GetInstance());
		AllObjectControl.push_back(ChestControl::GetInstance());
		AllObjectControl.push_back(PlayerControl::GetInstance());
	}
	
	for (int i = 0; i < AllObjectControl.size(); i++) {
		AllObjectControl[i]->Initialize((CameraControl::GetInstance()->GetCamera()));
	}
	Object3d::SetCamera((CameraControl::GetInstance()->GetCamera()));
	
	//カメラをセット
	f_Object3d::SetCamera((CameraControl::GetInstance()->GetCamera()));
	//グラフィックパイプライン生成
	f_Object3d::CreateGraphicsPipeline();

	Field::GetInstance()->Initialize((CameraControl::GetInstance()->GetCamera()));
	postEffect = new PostEffect();
	postEffect->Initialize();

}
#pragma endregion

#pragma region 更新処理
void PlayScene::Update()
{
	if (!LoadEnemy &&!PlayGame) {
		LoadEnemy = true;
	}
	
		objUpdate((CameraControl::GetInstance()->GetCamera()));//オブジェクトの更新処理

		LoadParam((CameraControl::GetInstance()->GetCamera()));
		if (PlayGame) {
		if (!cameraMove) {

			Feed::GetInstance()->Update_White(Feed::FEEDOUT);
		}

		if (Feed::GetInstance()->GetAlpha() <= 0.0f) {
			cameraMove = true;
		}

		SistemConfig::GetInstance()->Update();

		if (playFeed) {
			Feed::GetInstance()->Update_Black(Feed::FEEDIN);
			if (Feed::GetInstance()->GetAlpha() >= 1.0f) {
				feedout = true;
				playFeed = false;
			}
		}
		CameraControl::GetInstance()->SetCameraState(CameraControl::SPLINE);
		if (feedout) {
			Feed::GetInstance()->Update_Black(Feed::FEEDOUT);
			if (Feed::GetInstance()->GetAlpha() <= 0.0f) {
				//CameraControl::GetInstance()->SetCameraState(CameraControl::PLAYER);
			}
		} else {
			if (Collision::GetLength(CameraControl::GetInstance()->GetCamera()->GetEye(), PlayerControl::GetInstance()->GetPlayer()->GetPosition()) < 50) {
				playFeed = true;
			} else {

			}
		}

		if (SistemConfig::GetInstance()->GetConfigJudgMent()) {
			c_postEffect = Blur;
		} else {
			c_postEffect = Default;
		}
		if (input->TriggerKey(DIK_R)) {//押されたら
			BaseScene* scene = new MapCreateScene(sceneManager_);//次のシーンのインスタンス生成
			sceneManager_->SetnextScene(scene);//シーンのセット
		}
	}
}
#pragma endregion 

void PlayScene::MyGameDraw()
{
	Field::GetInstance()->Draw();
	
	if (EnemyControl::GetInstance()->GetQuentity() > 1) {
		for (int i = 0; i < AllObjectControl.size(); i++) {
		AllObjectControl[i]->Draw();
		}
	}
}

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
			//UI::GetInstance()->HUDDraw();
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
		SistemConfig::GetInstance()->Draw();
		CustomButton::GetInstance()->Draw();
	
		//Feed::GetInstance()->Draw();
		if (feedout) {
			if (Feed::GetInstance()->GetAlpha() <= 0.0f) {
				UI::GetInstance()->HUDDraw();
			}
		}
		UI::GetInstance()->AreaNameDraw();

		if (DirectXCommon::GetInstance()->GetFullScreen() == false) {
			ImGuiDraw();
		}
		DirectXCommon::GetInstance()->EndDraw();
		break;
	}
}
#pragma endregion

void PlayScene::ImGuiDraw()
{
	{
		ImGui::Begin("Obj1");
		ImGui::SetWindowPos(ImVec2(0, 500));
		ImGui::SetWindowSize(ImVec2(500, 300));
		
		if (ImGui::TreeNode("Damage")) {
			int d = PlayerAttackState::GetInstance()->GetDamage();
			ImGui::SliderInt("positionX", &d, -100, 100);
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
	PlayerControl::GetInstance()->GetPlayer()->ImguiDraw();
	{//カメラ
		ImGui::Begin("Camera");
		float cz = (CameraControl::GetInstance()->GetCamera())->GetEye().z;
		ImGui::SliderFloat("positionXZ", &cz, 0, 500);
		ImGui::SliderFloat("positionY", &CameraHeight, 0, 30);
		bool defaultPos;
		if (ImGui::RadioButton("DefaultPosition", &defaultPos)) {
			CameraDis = 25;
			CameraHeight = 9;
		}
		ImGui::End();
	}
	ImGui::Begin("Scene");

	if (ImGui::RadioButton("Scene_Create", t)) {
		BaseScene* scene = new MapCreateScene(sceneManager_);//次のシーンのインスタンス生成
		sceneManager_->SetnextScene(scene);//シーンのセット
	}
	if (ImGui::RadioButton("Scene_Tittle", y)) {
		BaseScene* scene = new TitleScene(sceneManager_);//次のシーンのインスタンス生成
		sceneManager_->SetnextScene(scene);//シーンのセット
	}
	ImGui::End();

}

void PlayScene::LoadParam(DebugCamera*camera)
{
	if(LoadEnemy){
		for (int i = 0; i < AllObjectControl.size();i++) {
			AllObjectControl[i]->Load((CameraControl::GetInstance()->GetCamera()));
		}
		hudload = true;
		LoadEnemy = false;
		PlayGame = true;
	}
}

void PlayScene::Finalize()
{
	//SistemConfig::GetInstance()->~SistemConfig();
	for (int i = 0; i < AllObjectControl.size(); i++) {
		AllObjectControl[i]->Finalize();
	}
	Field::GetInstance()->Finalize();
	AllObjectControl.clear();

}