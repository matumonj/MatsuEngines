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
//�V�[���̃R���X�g���N�^
PlayScene::PlayScene(SceneManager* sceneManager)
	:BaseScene(sceneManager)
{

}

#pragma region �I�u�W�F�N�g+���C�g�̍X�V����
void PlayScene::objUpdate(DebugCamera*camera)
{

	if (EnemyControl::GetInstance()->GetQuentity() > 1) {
		for (int i = 0; i < AllObjectControl.size(); i++) {
			AllObjectControl[i]->Update((CameraControl::GetInstance()->GetCamera()));
		}
		TargetMarker::GetInstance()->Update((CameraControl::GetInstance()->GetCamera()), PlayerControl::GetInstance()->GetPlayer());
		PlayerAttackState::GetInstance()->Update();
		UI::GetInstance()->HUDUpdate(hudload, (CameraControl::GetInstance()->GetCamera()));
		//Effects::GetInstance()->Update((CameraControl::GetInstance()->GetCamera()));
	}

	PlayerControl::GetInstance()->GetPlayer()->Update( (CameraControl::GetInstance()->GetCamera()));
	Field::GetInstance()->Update((CameraControl::GetInstance()->GetCamera()));
	CustomButton::GetInstance()->Update();
}

#pragma endregion

#pragma region ������
void PlayScene::Initialize()
{
	input = Input::GetInstance();

	if (AllObjectControl.size() == 0) {
		//�J������ԏ�Ɂ@����Control���J�����������ɂ��Ă邩��
		AllObjectControl.push_back(CameraControl::GetInstance());
		AllObjectControl.push_back(WoodControl::GetInstance());
		AllObjectControl.push_back(FenceControl::GetInstance());
		AllObjectControl.push_back(EnemyControl::GetInstance());
		AllObjectControl.push_back(ChestControl::GetInstance());
	
	}
	
	for (int i = 0; i < AllObjectControl.size(); i++) {
		AllObjectControl[i]->Initialize((CameraControl::GetInstance()->GetCamera()));
	}
	Object3d::SetCamera((CameraControl::GetInstance()->GetCamera()));
	
	//�J�������Z�b�g
	f_Object3d::SetCamera((CameraControl::GetInstance()->GetCamera()));
	//�O���t�B�b�N�p�C�v���C������
	f_Object3d::CreateGraphicsPipeline();

	PlayerControl::GetInstance()->GetPlayer()->Initialize((CameraControl::GetInstance()->GetCamera()));
	UI::GetInstance()->Initialize();
	SistemConfig::GetInstance()->Initialize();
	CustomButton::GetInstance()->Initialize();
	Field::GetInstance()->Initialize((CameraControl::GetInstance()->GetCamera()));
	TargetMarker::GetInstance()->Initialize();
	postEffect = new PostEffect();
	postEffect->Initialize();

	//Effects::GetInstance()->Initialize((CameraControl::GetInstance()->GetCamera()));
	
	PlayerControl::GetInstance()->GetPlayer()->SetPosition({ 110,-15,-379 });
}
#pragma endregion

#pragma region �X�V����
void PlayScene::Update()
{


	objUpdate((CameraControl::GetInstance()->GetCamera()));//�I�u�W�F�N�g�̍X�V����

	LoadParam((CameraControl::GetInstance()->GetCamera()));

	if (!cameraMove) {
		if (Feed::GetInstance()->GetAlpha() == 1.0f) {
			LoadEnemy = true;
		}
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
	if(feedout) {
		Feed::GetInstance()->Update_Black(Feed::FEEDOUT);
		if (Feed::GetInstance()->GetAlpha() <= 0.0f) {
			CameraControl::GetInstance()->SetCameraState(CameraControl::PLAYER);
		}
	}
	else {
		if (Collision::GetLength(CameraControl::GetInstance()->GetCamera()->GetEye(), PlayerControl::GetInstance()->GetPlayer()->GetPosition()) < 50) {
			playFeed = true;
		}
		else {
			CameraControl::GetInstance()->SetCameraState(CameraControl::SPLINE);
		}
	}
	
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
	
	PlayerControl::GetInstance()->GetPlayer()->Draw();
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
	{//�J����
		ImGui::Begin("Camera");
		float cz = (CameraControl::GetInstance()->GetCamera())->GetEye().z;
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
			AllObjectControl[i]->Load((CameraControl::GetInstance()->GetCamera()));
		}
		hudload = true;
		LoadEnemy = false;
	}
}

void PlayScene::Finalize()
{
	//SistemConfig::GetInstance()->~SistemConfig();
	delete (CameraControl::GetInstance()->GetCamera());
	//delete postEffect, lightGroup;
	for (int i = 0; i < AllObjectControl.size(); i++) {
		//delete AllObjectControl[i];
	} 
}