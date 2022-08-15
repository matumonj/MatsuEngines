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
//�V�[���̃R���X�g���N�^
PlayScene::PlayScene(SceneManager* sceneManager)
	:BaseScene(sceneManager)
{

}

#pragma region �I�u�W�F�N�g+���C�g�̍X�V����
void PlayScene::objUpdate(DebugCamera* camera)
{

	Player::GetInstance()->Update({ 1,1,1,p_alpha }, camera);
	
	if (EnemyControl::GetInstance()->GetQuentity() > 1) {
		for (int i = 0; i < AllObjectControl.size(); i++) {
			AllObjectControl[i]->Update(camera);
		}
		//TargetMarker::GetInstance()->Update(camera, Player::GetInstance());
		PlayerAttackState::GetInstance()->Update();
		//UI::GetInstance()->HUDUpdate(hudload, camera);
		//Effects::GetInstance()->Update(camera);

	}
	//HUD::GetInstance()->TaskUpdate(camera);
	//TargetMarker::GetInstance()->Update(enemys, camera, Player::GetInstance());
	Field::GetInstance()->Update(camera);
	CustomButton::GetInstance()->Update();
}

#pragma endregion

#pragma region ������
void PlayScene::Initialize()
{
	input = Input::GetInstance();

	if (AllObjectControl.size() == 0) {
		AllObjectControl.push_back(WoodControl::GetInstance());
		AllObjectControl.push_back(FenceControl::GetInstance());
		AllObjectControl.push_back(EnemyControl::GetInstance());
		AllObjectControl.push_back(ChestControl::GetInstance());
		//AllObjectControl.push_back(CameraControl::GetInstance());
	}
	//TargetMarker::GetInstance()->Initialize();

	// �J��������
	camera = new DebugCamera(WinApp::window_width, WinApp::window_height/*input*/);
	// 3D�I�u�W�F�N�g�ɃJ�������Z�b�g
	Object3d::SetCamera(camera);
	// ���C�g����
	lightGroup = LightGroup::Create();
	// 3D�I�u�G�N�g�Ƀ��C�g���Z�b�g
	Object3d::SetLightGroup(lightGroup);
	//�p�����[�^�̐ݒ�
	lightGroup->LightSetting();

	lightGroup->SetSpotLightPos(SpotLightPos);
	
	lightGroup->SetSpotLightColor(SpotLightColor);

	//�J�������Z�b�g
	f_Object3d::SetCamera(camera);
	//�O���t�B�b�N�p�C�v���C������
	f_Object3d::CreateGraphicsPipeline();

	for (int i = 0; i < AllObjectControl.size(); i++) {
		AllObjectControl[i]->Initialize(camera);
	}
	Player::GetInstance()->Initialize(camera);
	//UI::GetInstance()->Initialize();
	SistemConfig::GetInstance()->Initialize();
	CustomButton::GetInstance()->Initialize();
	Field::GetInstance()->Initialize(camera);

	postEffect = new PostEffect();
	postEffect->Initialize();

	//Effects::GetInstance()->Initialize(camera);
	
	Player::GetInstance()->SetPosition({ 110,-15,-379 });
}
#pragma endregion

#pragma region �X�V����
void PlayScene::Update()
{
	if (!cameraMove) {
		Feed::GetInstance()->Update_White(Feed::FEEDOUT);
	}
	if (Feed::GetInstance()->GetAlpha() <= 0.0f) {
		cameraMove = true;
	}
	lightGroup->SpotLightUpdate();
	
	SistemConfig::GetInstance()->Update();
	if (input->Pushkey(DIK_RIGHT)) {
		charaAngle += 0.5f;
		cameraAngle -= 0.5f;
		Player::GetInstance()->SetCharaRotation(charaAngle);
	} else if (input->Pushkey(DIK_LEFT) || input->RightTiltStick(input->Left)) {
		cameraAngle += 0.5f;
		charaAngle -= 0.5f;
		Player::GetInstance()->SetCharaRotation(charaAngle);
	}
	if (cameraAngle >= 360 + 90 || cameraAngle <= -360) {
		cameraAngle = 0;
	}
	
	
	if (playFeed) {
		Feed::GetInstance()->Update_Black(Feed::FEEDIN);
		if (Feed::GetInstance()->GetAlpha() >= 1.0f) {
			feedout = true;
			playFeed = false;
		}
	}
	if(feedout) {
		Feed::GetInstance()->Update_Black(Feed::FEEDOUT);
		if (Feed::GetInstance()->GetAlpha() <= 0.0f) {
			camera->SetEye(CameraPosition);
		}
	}
	else {
		if (Collision::GetLength(camera->GetEye(), Player::GetInstance()->GetPosition()) < 50) {
			playFeed = true;
		}
	}
	CameraPosition.x = Player::GetInstance()->GetPosition().x + cosf((float)(cameraAngle) * 3.14f / 180.0f) * CameraDis;
	CameraPosition.z = Player::GetInstance()->GetPosition().z + sinf((float)(cameraAngle) * 3.14f / 180.0f) * CameraDis;
	CameraPosition.y = Player::GetInstance()->GetPosition().y + CameraHeight;

	//�J�����֌W�̏���
	camera->SetDistance(distance);//
	//camera->SetEye(CameraPosition);
	camera->SetTarget({ Player::GetInstance()->GetPosition() });
	camera->Update();

	LoadParam(camera);
	
	
	
	objUpdate(camera);//�I�u�W�F�N�g�̍X�V����
	
	if (SistemConfig::GetInstance()->GetConfigJudgMent()) {
		c_postEffect = Blur;
	} else {
		c_postEffect = Default;
	}
	if (input->TriggerKey(DIK_R)) {//�����ꂽ��
		BaseScene* scene = new MapCreateScene(sceneManager_);//���̃V�[���̃C���X�^���X����
		sceneManager_->SetnextScene(scene);//�V�[���̃Z�b�g
	}

}
#pragma endregion 

void PlayScene::MyGameDraw()
{
	Field::GetInstance()->Draw();
	
	Player::GetInstance()->Draw();
	if (EnemyControl::GetInstance()->GetQuentity() > 1) {
		for (int i = 0; i < AllObjectControl.size(); i++) {
		AllObjectControl[i]->Draw();
		}
	}
	//Effects::GetInstance()->Draw();
	Texture::PreDraw();
	//TargetMarker::GetInstance()->Draw();
	Texture::PostDraw();
}

void PlayScene::Draw()
{
	//�|�X�g�G�t�F�N�g�̏ꍇ�킯(B�łڂ��� D���f�t�H���g)
	switch (c_postEffect)
	{
	case Blur://�ڂ����@�`�揀�Ⴄ����
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

	case Default://���ʂ̂���ɉ����������Ă��Ȃ�
		postEffect->PreDrawScene();
		postEffect->Draw();
		postEffect->PostDrawScene();

		DirectXCommon::GetInstance()->BeginDraw();
		MyGameDraw();
		SistemConfig::GetInstance()->Draw();
		CustomButton::GetInstance()->Draw();
	
		Feed::GetInstance()->Draw();
		//UI::GetInstance()->HUDDraw();

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
	{
		ImGui::Begin("None");
		if (ImGui::Button("Load", ImVec2(70, 50))) {
			LoadEnemy = true;
		}
		ImGui::End();
	}
	//
	Player::GetInstance()->ImguiDraw();
	{//�J����
		ImGui::Begin("Camera");
		float cz = camera->GetEye().z;
		ImGui::SliderFloat("positionXZ", &cz, 0, 500);
		ImGui::SliderFloat("positionY", &CameraHeight, 0, 30);
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
		BaseScene* scene = new MapCreateScene(sceneManager_);//���̃V�[���̃C���X�^���X����
		sceneManager_->SetnextScene(scene);//�V�[���̃Z�b�g
	}
	if (ImGui::RadioButton("Scene_Tittle", y)) {
		BaseScene* scene = new TitleScene(sceneManager_);//���̃V�[���̃C���X�^���X����
		sceneManager_->SetnextScene(scene);//�V�[���̃Z�b�g
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

void PlayScene::LoadParam(DebugCamera*camera)
{
	if(LoadEnemy){
		for (int i = 0; i < AllObjectControl.size();i++) {
			AllObjectControl[i]->Load(camera);
		}
		hudload = true;
		LoadEnemy = false;
	}
}

void PlayScene::Finalize()
{
	//SistemConfig::GetInstance()->~SistemConfig();
	delete camera;
	delete postEffect, lightGroup;
	for (int i = 0; i < AllObjectControl.size(); i++) {
		//delete AllObjectControl[i];
	} 
}