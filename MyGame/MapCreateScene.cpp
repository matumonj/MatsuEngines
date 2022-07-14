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

	MapCreate::SetBoxModel(camera);

}
#pragma endregion


#pragma region �I�u�W�F�N�g+���C�g�̍X�V����
void MapCreateScene::objUpdate(DebugCamera* camera)
{
	Field::GetInstance()->Update(camera);

	MapCreate::UpdateBoxModel(camera);
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

}
#pragma endregion

#pragma region �X�V����
void MapCreateScene::Update()
{
	lightGroup->SpotLightUpdate();

	Input::MouseMove mouseMove = Input::GetInstance()->GetMouseMove();
	//�}�E�X�̓��͏�Ԏ擾
	
	//�J�����֌W�̏���
	camera->SetDistance(0);
	camera->SetEye({0,20,-100});
	camera->SetTarget({0,0,0});
	camera->Update();


	objUpdate(camera);//�I�u�W�F�N�g�̍X�V����
	//Player::GetInstance()->Update({ 1,1,1,1 }, camera);
	MapCreate::EnemyArgment(camera);
	//�V�[���`�F���W

	if (Input::GetInstance()->TriggerKey(DIK_R)) {//�����ꂽ��
		//BaseScene* scene = new PlayScene(sceneManager_);//���̃V�[���̃C���X�^���X����
		//sceneManager_->SetnextScene(scene);//�V�[���̃Z�b�g
	}

}
#pragma endregion 

//�X�v���C�g�̕`��
#pragma region ���f���̕`��
void MapCreateScene::SpriteDraw()
{

	Field::GetInstance()->Draw();

	MapCreate::EnemyDraw();

	MapCreate::DrawBoxModel();
	
}
//s�v���C�ƈȊO�̕`��
void MapCreateScene::MyGameDraw()
{
	SpriteDraw();

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
	MapCreate::ImguiDraw_Enemy();

	if (ImGui::RadioButton("Scene_Create", t)) {
		BaseScene* scene = new PlayScene(sceneManager_);//���̃V�[���̃C���X�^���X����
		sceneManager_->SetnextScene(scene);//�V�[���̃Z�b�g
	}
	if (ImGui::RadioButton("Scene_Tittle", t)) {
		BaseScene* scene = new TitleScene(sceneManager_);//���̃V�[���̃C���X�^���X����
		sceneManager_->SetnextScene(scene);//�V�[���̃Z�b�g
	}
	ImGui::End();
}
#pragma region �������
void MapCreateScene::Finalize()
{
}
#pragma endregion