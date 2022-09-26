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
#include"SelectSword.h"
#include"KnockAttack.h"
#include<iomanip>
//�V�[���̃R���X�g���N�^
Tutorial::Tutorial(SceneManager* sceneManager)
	:BaseScene(sceneManager)
{

}

#pragma region �I�u�W�F�N�g+���C�g�̍X�V����
void Tutorial::objUpdate(DebugCamera* camera)
{
	//�J�����̒����_���v���C���[�ɃZ�b�g
	CameraControl::GetInstance()->SetCameraState(CameraControl::PLAYER);
	if (!Load && !Play) {
		Load = true;
	}
	if (Play) {//csv����̓ǂݍ��ݏI����Ă���X�V����
		//1->Player  0->Camera �J�����̒����_Player�ɍ��킷�̂�Player����
		AllObjectControl[1]->Update(CameraControl::GetInstance()->GetCamera());
		AllObjectControl[0]->Update(CameraControl::GetInstance()->GetCamera());
		for (int i = 2; i < AllObjectControl.size(); i++) {
			if (AllObjectControl[i] != nullptr) {
				AllObjectControl[i]->Update(CameraControl::GetInstance()->GetCamera());
			}
		}
		UI::GetInstance()->HUDUpdate(hudload, CameraControl::GetInstance()->GetCamera());
		}

	Field::GetInstance()->Update(CameraControl::GetInstance()->GetCamera());
}

#pragma endregion

#pragma region ������
void Tutorial::Initialize()
{
	Texture::LoadTexture(47, L"Resources/debugfont2.png");
	
	input = Input::GetInstance();
	//if (AllObjectControl.size() == 0) {//�e�I�u�W�F�N�g�C���X�^���X�Ԃ�����
		AllObjectControl.push_back(CameraControl::GetInstance());//Camera
		AllObjectControl.push_back(PlayerControl::GetInstance());//Player
		AllObjectControl.push_back(EnemyControl::GetInstance());//Enemy
		AllObjectControl.push_back(FenceControl::GetInstance());//Fence
		AllObjectControl.push_back(ChestControl::GetInstance());//Chest
		AllObjectControl.push_back(WoodControl::GetInstance());//Wood
	//}
	for (int i = 0; i < AllObjectControl.size(); i++) {//������
		AllObjectControl[i]->Initialize(CameraControl::GetInstance()->GetCamera());
	}
	Field::GetInstance()->Initialize(CameraControl::GetInstance()->GetCamera());

	// 3D�I�u�W�F�N�g�ɃJ�������Z�b�g
	Object3d::SetCamera(CameraControl::GetInstance()->GetCamera());
	//�J�������Z�b�g
	f_Object3d::SetCamera(CameraControl::GetInstance()->GetCamera());
	//�O���t�B�b�N�p�C�v���C������
	f_Object3d::CreateGraphicsPipeline();

	//�t�F�[�h��UI�X�v���C�g������
	Feed::GetInstance()->initialize();
	UI::GetInstance()->Initialize();

	//�ݒ��ʏ���
	SistemConfig::GetInstance()->Initialize();

	postEffect = new PostEffect();
	postEffect->Initialize();
	
}
#pragma endregion

#pragma region �X�V����
void Tutorial::Update()
{
	SistemConfig::GetInstance()->Update();
	//�e�I�u�W�F�N�g�̍X�V����
	objUpdate(CameraControl::GetInstance()->GetCamera());//�I�u�W�F�N�g�̍X�V����
	//csv�ǂݍ��ݕ���(Camera�̍X�V��ɂ���̂�objUpdate()����ł���)
	LoadParam(CameraControl::GetInstance()->GetCamera());

	//��萔�i�񂾂�V�[���`�F���W
	bool ArrivalJudg = PlayerControl::GetInstance()->GetPlayer()->GetPosition().z > -470.0f;
	if (ArrivalJudg) {
		scenechange = true;
	}

	if (scenechange) {
		Feed::GetInstance()->Update_White(Feed::FEEDIN);//�����Ȃ�܂�
	}
	if (SistemConfig::GetInstance()->GetConfigJudgMent()) {
		c_postEffect = Blur;
	} else {
		c_postEffect = Default;
	}

	if (scenechange&& Feed::GetInstance()->GetAlpha() >= 1.0f) {//��ʐ^�����Ȃ�����
		BaseScene* scene = new BossScene(sceneManager_);//���̃V�[���̃C���X�^���X����
		Play = false;
		SceneManager::GetInstance()->SetScene(SceneManager::BOSS);
		sceneManager_->SetnextScene(scene);//�V�[���̃Z�b�g
	}

	if (input->TriggerKey(DIK_G)) {//��ʐ^�����Ȃ�����
		BaseScene* scene = new TitleScene(sceneManager_);//���̃V�[���̃C���X�^���X����
		Play = false;
		SceneManager::GetInstance()->SetScene(SceneManager::TITLE);
		sceneManager_->SetnextScene(scene);//�V�[���̃Z�b�g
	}

}
#pragma endregion 

void Tutorial::MyGameDraw()
{
	Field::GetInstance()->Draw();
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
			UI::GetInstance()->HUDDraw();
		}
		SelectSword::GetInstance()->Draw();
		SistemConfig::GetInstance()->Draw();
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
		
		UI::GetInstance()->HUDDraw();
		Feed::GetInstance()->Draw();
		SistemConfig::GetInstance()->Draw();
		
		if (DirectXCommon::GetInstance()->GetFullScreen() == false) {
			PlayerControl::GetInstance()->GetPlayer()->ImguiDraw();
			ImGuiDraw();
		}
		DirectXCommon::GetInstance()->EndDraw();
		break;
	}
}
#pragma endregion

void Tutorial::ImGuiDraw()
{
	//{
	//	ImGui::Begin("Obj1");
	//	ImGui::SetWindowPos(ImVec2(0, 500));
	//	ImGui::SetWindowSize(ImVec2(500, 300));

	//	if (ImGui::TreeNode("Damage")) {
	//		int d = PlayerAttackState::GetInstance()->GetDamage();
	//		ImGui::SliderInt("damage", &d, -100, 100);
	//		ImGui::TreePop();
	//	}
	//	ImGui::End();
	//}
	//{
	//	ImGui::Begin("None");
	//	if (ImGui::Button("Load", ImVec2(70, 50))) {
	//		LoadEnemy = true;
	//	}
	//	ImGui::End();
	//}
	//{//�J����
	//	bool defaultPos;
	//	if (ImGui::RadioButton("DefaultPosition", &defaultPos)) {
	//		CameraDis = 25;
	//		CameraHeight = 9;
	//	}
	//	ImGui::SliderFloat("rotationX", &cameraAngle, -360, 360);

	//	ImGui::End();
	//}
	//ImGui::Begin("Scene");

	//if (ImGui::RadioButton("Scene_Create", t)) {
	//	BaseScene* scene = new MapCreateScene(sceneManager_);//���̃V�[���̃C���X�^���X����
	//	sceneManager_->SetnextScene(scene);//�V�[���̃Z�b�g
	//}
	//if (ImGui::RadioButton("Scene_Tittle", y)) {
	//	BaseScene* scene = new TitleScene(sceneManager_);//���̃V�[���̃C���X�^���X����
	//	SceneManager::GetInstance()->SetScene(SceneManager::PLAY);
	//	sceneManager_->SetnextScene(scene);//�V�[���̃Z�b�g
	//}
	//ImGui::End();
	////
	//{
	//	unsigned long current_time = timeGetTime();
	//	float fps = float(count_frame) / (current_time - prev_time) * 1000;
	//	ImGui::SliderFloat("FPS", &fps, -10, 50);
	//	count_frame++;
	//	ImGui::End();
	//}
}

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
	for (int i = 0; i < AllObjectControl.size(); i++) {//������
		AllObjectControl[i]->Finalize();
	}
	AllObjectControl.clear();

	//AttackCollision::GetInstance()->Finalize();

	Field::GetInstance()->Finalize();
	
	delete acol;

	Destroy(postEffect);
}
