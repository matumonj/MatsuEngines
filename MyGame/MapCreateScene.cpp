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
//�V�[���̃R���X�g���N�^
MapCreateScene::MapCreateScene(SceneManager* sceneManager)
	:BaseScene(sceneManager)
{

}


#pragma region ���f���ƃG�t�F�N�g�ƃ��C�g�̃C���X�^���X����
void MapCreateScene::ModelCreate(DebugCamera* camera)
{
	
	postEffect = new PostEffect();
	postEffect->Initialize();
	
	Field::GetInstance()->Initialize(camera);

	// ���C�g����
	lightGroup = LightGroup::Create();
	// 3D�I�u�G�N�g�Ƀ��C�g���Z�b�g
	Object3d::SetLightGroup(lightGroup);
	//�p�����[�^�̐ݒ�
	lightGroup->LightSetting();

	MapCreate::GetInstance()->ObjectInitialize(camera);

}
#pragma endregion


#pragma region �I�u�W�F�N�g+���C�g�̍X�V����
void MapCreateScene::objUpdate(DebugCamera* camera)
{
	Field::GetInstance()->Update(camera);

	MapCreate::GetInstance()->ObjectUpdate(camera);
}
#pragma endregion

#pragma region ������
void MapCreateScene::Initialize()
{
	
	// �J��������
	camera = new DebugCamera(WinApp::window_width, WinApp::window_height/*input*/);
	// 3D�I�u�W�F�N�g�ɃJ�������Z�b�g
	Object3d::SetCamera(camera);
	camera->SetEye({ 0,0,0 });
	ModelCreate(camera);//

	//�J�������Z�b�g
	f_Object3d::SetCamera(camera);
	//�O���t�B�b�N�p�C�v���C������
	f_Object3d::CreateGraphicsPipeline();

	collisionManager = CollisionManager::GetInstance();
	//
	c_postEffect = Default;

	CameraPosition.z = -190;
	CameraPosition.y = 50;
}
#pragma endregion

#pragma region �X�V����
void MapCreateScene::Update()
{
	lightGroup->SpotLightUpdate();

	Input::MouseMove mouseMove = Input::GetInstance()->GetMouseMove();
	//�}�E�X�̓��͏�Ԏ擾
	
	//�J�����֌W�̏���
	camera->SetEye(CameraPosition);
	camera->SetTarget({ CameraPosition.x,CameraPosition.y-15,CameraPosition.z+20});
	camera->Update();


	objUpdate(camera);//�I�u�W�F�N�g�̍X�V����
	//Player::GetInstance()->Update({ 1,1,1,1 }, camera);
	MapCreate::GetInstance()->ObjectArgment(camera);
	//�V�[���`�F���W

	if (Input::GetInstance()->TriggerKey(DIK_R)) {//�����ꂽ��
		//BaseScene* scene = new PlayScene(sceneManager_);//���̃V�[���̃C���X�^���X����
		//sceneManager_->SetnextScene(scene);//�V�[���̃Z�b�g
	}

}
#pragma endregion 


//s�v���C�ƈȊO�̕`��
void MapCreateScene::MyGameDraw()
{
	Field::GetInstance()->Draw();
	MapCreate::GetInstance()->ObjectDraw();
}
#pragma endregion
//���ɓ���
#pragma region �`��(imgui�ƃX�v���C�g�ƃ��f���܂Ƃ߂�����)
void MapCreateScene::Draw()
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
	MapCreate::GetInstance()->ImGuiDraw();
	if (ImGui::RadioButton("Scene_Create", t)) {
		BaseScene* scene = new PlayScene(sceneManager_);//���̃V�[���̃C���X�^���X����
		sceneManager_->SetnextScene(scene);//�V�[���̃Z�b�g
	}
	if (ImGui::RadioButton("Scene_Tittle", t)) {
		BaseScene* scene = new TitleScene(sceneManager_);//���̃V�[���̃C���X�^���X����
		sceneManager_->SetnextScene(scene);//�V�[���̃Z�b�g
	}
	ImGui::End();


	ImGui::Begin("CameraPosition");
	ImGui::SliderFloat("CameraPosx", &CameraPosition.x, -400, 400);
	ImGui::SliderFloat("CameraPosy", &CameraPosition.y, -200, 200);
	ImGui::SliderFloat("CameraPosz", &CameraPosition.z, -800, 500);
	ImGui::End();
}
#pragma region �������
void MapCreateScene::Finalize()
{
}
#pragma endregion