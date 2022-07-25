#include "PlayScene.h"
#include"Input.h"
#include"DirectXCommon.h"
#include"TitleScene.h"
#include"SceneManager.h"
#include"MeshCollider.h"
#include"TouchableObject.h"
#include"CollisionManager.h"
#include"SphereCollider.h"
#include"MapCreateScene.h"
#include"Field.h"
#include"HUD.h"
#include"PlayerAttackState.h"
#include "PhysicsAction.h"
#include"SistemConfig.h"
#include"EnemyAttackJudgement.h"
#include"EnemyControl.h"
#include"WoodControl.h"
#include"FenceControl.h"
#include"UI.h"
//�V�[���̃R���X�g���N�^
PlayScene::PlayScene(SceneManager* sceneManager)
	:BaseScene(sceneManager)
{

}

#pragma region �I�u�W�F�N�g+���C�g�̍X�V����
void PlayScene::objUpdate(DebugCamera* camera)
{
	XMFLOAT3 oldp = Player::GetInstance()->GetPosition();
	
	Player::GetInstance()->Update({ 1,1,1,p_alpha }, camera);
	if (EnemyControl::GetInstance()->GetQuentity() > 1) {
		WoodControl::GetInstance()->Update(camera);
		EnemyControl::GetInstance()->Update(camera);
		FenceControl::GetInstance()->Update(camera);
		PlayerAttackState::GetInstance()->Update(EnemyControl::GetInstance()->GetEnemyindex(1));
		UI::GetInstance()->HUDUpdate(hudload, camera);

	}
	//TargetMarker::GetInstance()->Update(enemys, camera, Player::GetInstance());
			
	}

#pragma endregion

#pragma region ������
void PlayScene::Initialize()
{
	
	input = Input::GetInstance();
	TargetMarker::GetInstance()->Initialize();

	// �J��������
	camera = new DebugCamera(WinApp::window_width, WinApp::window_height/*input*/);
	// 3D�I�u�W�F�N�g�ɃJ�������Z�b�g
	Object3d::SetCamera(camera);

	camera->SetEye({ Player_Pos.x,Player_Pos.y + 5,Player_Pos.z - 15 });

	UI::GetInstance()->Initialize();
	SistemConfig::GetInstance()->Initialize();
	
	// ���C�g����
	lightGroup = LightGroup::Create();
	// 3D�I�u�G�N�g�Ƀ��C�g���Z�b�g
	Object3d::SetLightGroup(lightGroup);
	//�p�����[�^�̐ݒ�
	lightGroup->LightSetting();

	lightGroup->SetSpotLightPos(SpotLightPos);
	
	lightGroup->SetSpotLightColor(SpotLightColor);

	Field::GetInstance()->Initialize(camera);

	collisionManager = CollisionManager::GetInstance();

	//f_Object3d::SetDevice();
	//�J�������Z�b�g
	f_Object3d::SetCamera(camera);
	//�O���t�B�b�N�p�C�v���C������
	f_Object3d::CreateGraphicsPipeline();

	WoodControl::GetInstance()->Initialize(camera);
	EnemyControl::GetInstance()->Initialize(camera);
	FenceControl::GetInstance()->Initialize(camera);

	Player::GetInstance()->Initialize(camera);

	postEffect = new PostEffect();
	postEffect->Initialize();

}
#pragma endregion

#pragma region �X�V����
void PlayScene::Update()
{
	lightGroup->SpotLightUpdate();
	SistemConfig::GetInstance()->Update();

	LoadParam();
	
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
	//FBX���f���̍X�V
	//object1->Updata(TRUE);
	CameraPosition.x = Player::GetInstance()->GetPosition().x + cosf((float)(cameraAngle) * 3.14f / 180.0f) * CameraDis;
	CameraPosition.z = Player::GetInstance()->GetPosition().z + sinf((float)(cameraAngle) * 3.14f / 180.0f) * CameraDis;
	CameraPosition.y = Player::GetInstance()->GetPosition().y + CameraHeight;

	//�J�����֌W�̏���
	camera->SetDistance(distance);//
	camera->SetEye(CameraPosition);
	camera->SetTarget({ Player::GetInstance()->GetPosition() });
	camera->Update();

	Field::GetInstance()->Update(camera);

	objUpdate(camera);//�I�u�W�F�N�g�̍X�V����
	
	if (input->TriggerKey(DIK_R)) {//�����ꂽ��
		BaseScene* scene = new MapCreateScene(sceneManager_);//���̃V�[���̃C���X�^���X����
		sceneManager_->SetnextScene(scene);//�V�[���̃Z�b�g
	}

}
#pragma endregion 

//�X�v���C�g�̕`��
#pragma region ���f���̕`��
void PlayScene::SpriteDraw()
{
	if (SistemConfig::GetInstance()->GetConfigJudgMent()) {
		c_postEffect = Blur;
	} else {
		c_postEffect = Default;
	}
	Player::GetInstance()->PreDraw();
	if (!turnoff_player) {//�f�o�b�O�p
		Player::GetInstance()->Draw();
	}
	Player::GetInstance()->PostDraw();

	if (EnemyControl::GetInstance()->GetQuentity()>1) {
		WoodControl::GetInstance()->Draw();
		EnemyControl::GetInstance()->Draw();
		FenceControl::GetInstance()->Draw();

	}
}
//s�v���C�ƈȊO�̕`��
void PlayScene::MyGameDraw()
{

	Field::GetInstance()->Draw();

	SpriteDraw();

	Texture::PreDraw();
	TargetMarker::GetInstance()->Draw();
	Texture::PostDraw();

}
#pragma endregion
//���ɓ���
#pragma region �`��(imgui�ƃX�v���C�g�ƃ��f���܂Ƃ߂�����)
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
			UI::GetInstance()->HUDDraw();
		}
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
#pragma region �������
void PlayScene::Finalize()
{
	//delete model, model5, model2;
}

void PlayScene::LoadParam()
{
	if(LoadEnemy){
		WoodControl::GetInstance()->Load(camera);
		EnemyControl::GetInstance()->Load(camera);
		FenceControl::GetInstance()->Load(camera);

		hudload = true;
		LoadEnemy = false;
	}
}
