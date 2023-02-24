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
//�V�[���̃R���X�g���N�^
MapCreateScene::MapCreateScene(SceneManager* sceneManager)
	: BaseScene(sceneManager)
{
}


#pragma region ���f���ƃG�t�F�N�g�ƃ��C�g�̃C���X�^���X����
void MapCreateScene::ModelCreate(DebugCamera* camera)
{
	Field::GetIns()->Initialize();

	// ���C�g����
	lightGroup = LightGroup::Create();
	// 3D�I�u�G�N�g�Ƀ��C�g���Z�b�g
	Object3d::SetLightGroup(lightGroup);
	GrassObj::SetLightGroup(lightGroup);
	//�p�����[�^�̐ݒ�
	MapCreate::GetIns()->ObjectInitialize(CameraControl::GetIns()->GetCamera());
}
#pragma endregion


#pragma region �I�u�W�F�N�g+���C�g�̍X�V����
void MapCreateScene::objUpdate(DebugCamera* camera)
{
	Field::GetIns()->Update_Edit();

	MapCreate::GetIns()->ObjectUpdate(CameraControl::GetIns()->GetCamera());
}
#pragma endregion

#pragma region ������
void MapCreateScene::Initialize()
{
	CameraControl::GetIns()->ParamSet();

	// 3D�I�u�W�F�N�g�ɃJ�������Z�b�g
	ModelCreate(CameraControl::GetIns()->GetCamera()); //

	//�J�������Z�b�g
	f_Object3d::SetCamera(CameraControl::GetIns()->GetCamera());
	//�O���t�B�b�N�p�C�v���C������
	f_Object3d::CreateGraphicsPipeline();

	CameraPosition.z = -190;
	CameraPosition.y = 50;

	CameraControl::GetIns()->SetCameraState(CameraControl::MAPCREATE);
}
#pragma endregion

#pragma region �X�V����
void MapCreateScene::Update()
{
	Input::MouseMove mouseMove = Input::GetIns()->GetMouseMove();
	
	//�J�����֌W�̏���
	CameraControl::GetIns()->GetCamera()->SetEye(CameraPosition);
	CameraControl::GetIns()->GetCamera()->SetTarget({
		CameraPosition.x, CameraPosition.y - 15, CameraPosition.z + 20
	});
	CameraControl::GetIns()->GetCamera()->Update();

	//�I�u�W�F�N�g�̍X�V����
	objUpdate(CameraControl::GetIns()->GetCamera());
	//�z�u�Ǘ��p
	MapCreate::GetIns()->ObjectArgment(CameraControl::GetIns()->GetCamera());
}
#pragma endregion

void MapCreateScene::LightUpdate()
{
}

void MapCreateScene::SpriteDraw()
{
	
}

//�X�v���C�ƈȊO�̕`��
void MapCreateScene::MyGameDraw()
{
	Field::GetIns()->Draw();
	MapCreate::GetIns()->ObjectDraw();
}

//���ɓ���
#pragma region �`��(imgui�ƃX�v���C�g�ƃ��f���܂Ƃ߂�����)
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
		BaseScene* scene = new PlayScene(sceneManager_); //���̃V�[���̃C���X�^���X����
		sceneManager_->SetnextScene(scene); //�V�[���̃Z�b�g
	}
	if (ImGui::RadioButton("Scene_Tittle", t))
	{
		BaseScene* scene = new TitleScene(sceneManager_); //���̃V�[���̃C���X�^���X����
		sceneManager_->SetnextScene(scene); //�V�[���̃Z�b�g
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
#pragma region �������
void MapCreateScene::Finalize()
{
	Destroy(postEffect);
	Destroy(lightGroup);
	CameraControl::GetIns()->Finalize();
	MapCreate::GetIns()->Finalize();
	Field::GetIns()->Finalize();
}
#pragma endregion
