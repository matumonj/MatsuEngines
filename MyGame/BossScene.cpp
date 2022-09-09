#include "BossScene.h"
#include"Input.h"
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
#include"PlayScene.h"
#include"Feed.h"
#include"PlayerControl.h"
#include"AttackCollision.h"
#include"SelectSword.h"
#include"KnockAttack.h"
#include"CircleAttack.h"
#include"HalfAttack.h"
BossScene::BossScene(SceneManager* sceneManager)
	:BaseScene(sceneManager)
{

}

void BossScene::Initialize()
{
	nails = new Nail();
	input = Input::GetInstance();
	if (AllObjectControl.size() == 0) {//�e�I�u�W�F�N�g�C���X�^���X�Ԃ�����
		AllObjectControl.push_back(CameraControl::GetInstance());
		AllObjectControl.push_back(PlayerControl::GetInstance());
		AllObjectControl.push_back(EnemyControl::GetInstance());
		AllObjectControl.push_back(FenceControl::GetInstance());
		AllObjectControl.push_back(ChestControl::GetInstance());
		AllObjectControl.push_back(WoodControl::GetInstance());
	}
	for (int i = 0; i < AllObjectControl.size(); i++) {//������
		AllObjectControl[i]->Initialize(CameraControl::GetInstance()->GetCamera());
	}
	AttackCollision::GetInstance()->Init();
	TargetMarker::GetInstance()->Initialize();
	KnockAttack::GetInstance()->Initialize();
	CircleAttack::GetInstance()->Initialize();
	HalfAttack::GetInstance()->Initialize();
	// 3D�I�u�W�F�N�g�ɃJ�������Z�b�g
	Object3d::SetCamera(CameraControl::GetInstance()->GetCamera());

	//�J�������Z�b�g
	f_Object3d::SetCamera(CameraControl::GetInstance()->GetCamera());
	//�O���t�B�b�N�p�C�v���C������
	f_Object3d::CreateGraphicsPipeline();

	UI::GetInstance()->Initialize();
	SistemConfig::GetInstance()->Initialize();
	Field::GetInstance()->Initialize(CameraControl::GetInstance()->GetCamera());
	postEffect = new PostEffect();
	postEffect->Initialize();

	Nail::GetInstance()->ModelSet();
	//Feed::GetInstance()->initialize();
}

void BossScene::Update()
{
	if (!LoadEnemy&&!Play) {
		LoadEnemy = true;
	}
	SistemConfig::GetInstance()->Update();

	CameraControl::GetInstance()->SetCameraState(CameraControl::PLAYER);

	if (Play) {//csv����̓ǂݍ��ݏI����Ă���X�V����
		//1->Player  0->Camera �J�����̒����_Player�ɍ��킷�̂�Player����
		AllObjectControl[1]->Update(CameraControl::GetInstance()->GetCamera());
		AllObjectControl[0]->Update(CameraControl::GetInstance()->GetCamera());
		for (int i = 2; i < AllObjectControl.size(); i++) {
			AllObjectControl[i]->Update(CameraControl::GetInstance()->GetCamera());
		}
		AttackCollision::GetInstance()->Update();
		PlayerAttackState::GetInstance()->Update();
		//HalfAttack::GetInstance()->ActionJudg();
		KnockAttack::GetInstance()->ActionJudg();
		//CircleAttack::GetInstance()->ActionJudg();
		Nail::GetInstance()->Update();
		UI::GetInstance()->HUDUpdate(hudload, CameraControl::GetInstance()->GetCamera());
		}
	Feed::GetInstance()->Update_White(Feed::FEEDOUT);

	Field::GetInstance()->Update(CameraControl::GetInstance()->GetCamera());
	//�e�I�u�W�F�N�g�̍X�V����
	//objUpdate(CameraControl::GetInstance()->GetCamera());//�I�u�W�F�N�g�̍X�V����
	//csv�ǂݍ��ݕ���(Camera�̍X�V��ɂ���̂�objUpdate()����ł���)
	LoadParam(CameraControl::GetInstance()->GetCamera());

	if (scenechange) {
		Feed::GetInstance()->Update_White(Feed::FEEDIN);//�����Ȃ�܂�
	}
	if (SistemConfig::GetInstance()->GetConfigJudgMent()) {
		c_postEffect = Blur;
	} else {
		c_postEffect = Default;
	}
	//if (Feed::GetInstance()->GetAlpha() >= 1.0f) {//��ʐ^�����Ȃ�����
		//BaseScene* scene = new PlayScene(sceneManager_);//���̃V�[���̃C���X�^���X����
		//SceneManager::GetInstance()->SetScene(SceneManager::PLAY);
		//sceneManager_->SetnextScene(scene);//�V�[���̃Z�b�g
	//}
}

void BossScene::MyGameDraw()
{
	Field::GetInstance()->Draw();
	if (Play) {
		for (int i = 0; i < AllObjectControl.size(); i++) {
			AllObjectControl[i]->Draw();
		}
	}
}
void BossScene::Draw()
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
		CircleAttack::GetInstance()->Draw();
		HalfAttack::GetInstance()->Draw();
		KnockAttack::GetInstance()->Draw();
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

bool BossScene::LoadParam(DebugCamera* camera)
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

void BossScene::ImGuiDraw()
{
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
	//		//CameraDis = 25;
	//		//CameraHeight = 9;
	//	}
	//	ImGui::SliderFloat("rotationX", &cameraAngle, -360, 360);

	//	ImGui::End();
	//}
	////
	//{
	//	unsigned long current_time = timeGetTime();
	//	float fps = float(count_frame) / (current_time - prev_time) * 1000;
	//	ImGui::SliderFloat("FPS", &fps, -10, 50);
	//	count_frame++;
	//	ImGui::End();
	//}
}

void BossScene::Finalize()
{

}