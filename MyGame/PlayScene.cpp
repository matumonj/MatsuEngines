#include "PlayScene.h"
#include"Input.h"
#include"DirectXCommon.h"
#include"TitleScene.h"
#include"SceneManager.h"
#include"MobEnemy.h"
#include"BossEnemy.h"
#include"MeshCollider.h"
#include"TouchableObject.h"
#include"CollisionManager.h"
#include"SphereCollider.h"
#include"MapCreateScene.h"
#include"Field.h"
#include"HUD.h"
#include"PlayerAttackState.h"
#include "PhysicsAction.h"
#include "WalkJudgement.h"
#include"WalkAction.h"
#include "FollowJudgement.h"
#include"FollowAction.h"
#include"StayJudgment.h"
#include"StayAction.h"
#include"EnemyAttackAction.h"
#include"SistemConfig.h"
#include"EnemyAttackJudgement.h"
#include"EnemyControl.h"
#include"WoodControl.h"
#include"FenceControl.h"
//シーンのコンストラクタ
PlayScene::PlayScene(SceneManager* sceneManager)
	:BaseScene(sceneManager)
{

}

#pragma region スプライトの生成
//スプライト生成
void PlayScene::SpriteCreate()
{
	HUD::GetInstance()->Initialize();
	HUD::GetInstance()->EnemyHPGaugeInitialize();
	HUD::GetInstance()->SkillButtonInitialize();
	gameui = new GameUI();
	gameui->Initialize();
	// デバッグテキスト用テクスチャ読み込み
	Sprite::LoadTexture(debugTextTexNumber, L"Resources/debugfont2.png");
	Sprite::LoadTexture(1, L"Resources/0057b6fa9ec85ae.jpg");
	Sprite::LoadTexture(2, L"Resources/tyuta_C.png");

	//普通のテクスチャ(スプライトじゃないよ)
	Texture::LoadTexture(6, L"Resources/DQVDS_-_Mechanowyrm.png");
	Texture::LoadTexture(1, L"Resources/ball.png");
	//モデル名を指定してファイル読み込み
	fbxmodel = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");

	SistemConfig::GetInstance()->Initialize();
}
#pragma endregion

#pragma region モデルとエフェクトとライトのインスタンス生成
void PlayScene::ModelCreate(DebugCamera* camera)
{

	// ライト生成
	lightGroup = LightGroup::Create();
	// 3Dオブエクトにライトをセット
	Object3d::SetLightGroup(lightGroup);
	//パラメータの設定
	lightGroup->LightSetting();

	Field::GetInstance()->Initialize(camera);

	collisionManager = CollisionManager::GetInstance();


}
#pragma endregion

#pragma region オブジェクト+ライトの更新処理
void PlayScene::objUpdate(DebugCamera* camera)
{
	XMFLOAT3 oldp = Player::GetInstance()->GetPosition();
	Player::GetInstance()->Update({ 1,1,1,p_alpha }, camera);

	if (EnemyControl::GetInstance()->GetQuentity() > 1) {
		EnemyControl::GetInstance()->Update(camera);

		FenceControl::GetInstance()->Update(camera);

		WoodControl::GetInstance()->Update(camera);
	}
}
#pragma endregion

#pragma region 初期化
void PlayScene::Initialize()
{
	input = Input::GetInstance();
	TargetMarker::GetInstance()->Initialize();
	// カメラ生成
	camera = new DebugCamera(WinApp::window_width, WinApp::window_height/*input*/);
	// 3Dオブジェクトにカメラをセット
	Object3d::SetCamera(camera);
	camera->SetEye({ Player_Pos.x,Player_Pos.y + 5,Player_Pos.z - 15 });

	SpriteCreate();//
	ModelCreate(camera);//

	lightGroup->SetSpotLightPos(SpotLightPos);
	lightGroup->SetSpotLightColor(SpotLightColor);

	//f_Object3d::SetDevice();
	//カメラをセット
	f_Object3d::SetCamera(camera);
	//グラフィックパイプライン生成
	f_Object3d::CreateGraphicsPipeline();

	EnemyControl::GetInstance()->Initialize(camera);
	Player::GetInstance()->Initialize(camera);

	postEffect = new PostEffect();
	postEffect->Initialize();

}
#pragma endregion

#pragma region 更新処理
void PlayScene::Update()
{
	lightGroup->SpotLightUpdate();

	SistemConfig::GetInstance()->Update();
	LoadParam();
	LoadParam_Wood();
	LoadParam_Fence();
	if (input->Pushkey(DIK_RIGHT)) {
		charaAngle += 0.5f;
		cameraAngle -= 0.5f;
		Player::GetInstance()->SetCharaRotation(charaAngle);
	} else if (input->Pushkey(DIK_LEFT)||input->RightTiltStick(input->Left)) {
		cameraAngle += 0.5f;
		charaAngle -= 0.5f;
		Player::GetInstance()->SetCharaRotation(charaAngle);
	}
	if (cameraAngle >= 360 + 90 || cameraAngle <= -360) {
		cameraAngle = 0;
	}
	//FBXモデルの更新
	//object1->Updata(TRUE);
	CameraPosition.x = Player::GetInstance()->GetPosition().x + cosf((float)(cameraAngle) * 3.14f / 180.0f) * CameraDis;
	CameraPosition.z = Player::GetInstance()->GetPosition().z + sinf((float)(cameraAngle) * 3.14f / 180.0f) * CameraDis;
	CameraPosition.y = Player::GetInstance()->GetPosition().y + CameraHeight;

	//カメラ関係の処理
	camera->SetDistance(distance);//
	camera->SetEye(CameraPosition);
	camera->SetTarget({ Player::GetInstance()->GetPosition() });
	camera->Update();

	Field::GetInstance()->Update(camera);

	if (enemys.size() > 1) {
		TargetMarker::GetInstance()->Update(enemys, camera, Player::GetInstance());
		HUD::GetInstance()->EnemyHPGaugeUpdate(enemys);
		HUD::GetInstance()->EnemyHPGauge_MultiUpdate(hudload, camera, enemys);

		PlayerAttackState::GetInstance()->Update(enemys);
		
	}

	objUpdate(camera);//オブジェクトの更新処理


	//gameui->Update(enemys, targetm);
	//                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         BV
	//シーンチェンジ
	
	HUD::GetInstance()->SkillBottonUpdate();
	HUD::GetInstance()->Update();
	if (input->TriggerKey(DIK_R)) {//押されたら
		BaseScene* scene = new MapCreateScene(sceneManager_);//次のシーンのインスタンス生成
		sceneManager_->SetnextScene(scene);//シーンのセット
	}

}
#pragma endregion 

//スプライトの描画
#pragma region モデルの描画
void PlayScene::SpriteDraw()
{
	if (SistemConfig::GetInstance()->GetConfigJudgMent()) {
		c_postEffect = Blur;
	} else {
		c_postEffect = Default;
	}
	Player::GetInstance()->PreDraw();
	if (!turnoff_player) {//デバッグ用
		Player::GetInstance()->Draw();
	}
	Player::GetInstance()->PostDraw();

	if (EnemyControl::GetInstance()->GetQuentity()>1) {
		EnemyControl::GetInstance()->Draw();

		WoodControl::GetInstance()->Draw();

		FenceControl::GetInstance()->Draw();
	}
}
//sプライと以外の描画
void PlayScene::MyGameDraw()
{

	Field::GetInstance()->Draw();

	SpriteDraw();

	Texture::PreDraw();
	TargetMarker::GetInstance()->Draw();
	Texture::PostDraw();


	//gameui->Draw(enemys,targetm,Player::GetInstance());
	//普通のテクスチャの描画
	//if (input->Pushkey(DIK_P)) {
	//Player::GetInstance()->normalAttack(targetm,enemys,camera);

}
#pragma endregion
//↓に入る
#pragma region 描画(imguiとスプライトとモデルまとめたもの)
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
			HUD::GetInstance()->EnemyHPGauge_MultiDraw();
			HUD::GetInstance()->Draw();
			HUD::GetInstance()->SkillBottonDraw();
		}
			SistemConfig::GetInstance()->Draw();
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
		HUD::GetInstance()->EnemyHPGauge_MultiDraw();
		HUD::GetInstance()->Draw();
		HUD::GetInstance()->SkillBottonDraw();

		SistemConfig::GetInstance()->Draw();
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
		if (ImGui::TreeNode("light_position")) {
			ImGui::SliderFloat("positionX", &SpotLightPos[0], -100, 100);
			ImGui::SliderFloat("positionY", &SpotLightPos[1], -100, 100);
			ImGui::SliderFloat("positionZ", &SpotLightPos[2], -100, 100);

			lightGroup->SetSpotLightPos(SpotLightPos);
			if (ImGui::Button("spotlight ON")) {
				lightGroup->SetSpotLightActive(0, true);
			}
			if (ImGui::Button("spotlight OFF")) {
				lightGroup->SetSpotLightActive(0, false);
			}
			ImGui::ColorPicker3("light_color", SpotLightColor);
			lightGroup->SetSpotLightColor(SpotLightColor);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Damage")) {
			int d = PlayerAttackState::GetInstance()->GetDamage();
			ImGui::SliderInt("positionX", &d, -100, 100);
			ImGui::TreePop();
		}
		ImGui::End();
	}
	//
	
	{
		ImGui::Begin("None");
		if (ImGui::Button("Load", ImVec2(70, 50))) {
			LoadEnemy = true;
		}
		//ImGui::Image(L"Resources/0057b6fa9ec85ae.jpg", ImVec2(100, 100));
		ImGui::End();
	}
	//
	Player::GetInstance()->ImguiDraw();
	{//カメラ
		ImGui::Begin("Camera");
		ImGui::SliderFloat("positionXZ", &CameraDis, 0, 100);
		ImGui::SliderFloat("positionY", &CameraHeight, 0, 30);
		bool defaultPos;
		if (ImGui::RadioButton("DefaultPosition", &defaultPos)) {
			CameraDis = 25;
			CameraHeight = 9;
		}
		ImGui::SliderFloat("rotationX", &cameraAngle, -360, 360);

		ImGui::End();
	}
	//

	{
		ImGui::Begin("Drawing");
		ImGui::Checkbox("TurnOff_Player", &turnoff_player);
		ImGui::Checkbox("TurnOff_Enemy", &turnoff_enemy);

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
	//
	{
		unsigned long current_time = timeGetTime();
		float fps = float(count_frame) / (current_time - prev_time) * 1000;
		ImGui::SliderFloat("FPS", &fps, -10, 50);
		count_frame++;
		ImGui::End();
	}
}
#pragma region 解放部分
void PlayScene::Finalize()
{
	delete model, model5, model2;
	//delete Player::GetInstance();


}

void PlayScene::LoadParam()
{
	if(LoadEnemy){
		EnemyControl::GetInstance()->LoadEnemy(camera);
		hudload = true;
	}
}

void PlayScene::LoadParam_Wood()
{
	if (LoadEnemy) {
		WoodControl::GetInstance()->LoadWood(camera);
		hudload = true;
	}
}

void PlayScene::LoadParam_Fence()
{
	if (LoadEnemy) {
		FenceControl::GetInstance()->LoadFences(camera);
		LoadEnemy = false;
	}
}
#pragma endregion
