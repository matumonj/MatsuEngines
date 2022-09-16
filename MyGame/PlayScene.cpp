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

void PlayScene::objUpdate(DebugCamera* camera)
{
	for (int i = 0; i < AllObjectControl.size(); i++) {
		AllObjectControl[i]->Update((CameraControl::GetInstance()->GetCamera()));
	}
	UI::GetInstance()->HUDUpdate(hudload, (CameraControl::GetInstance()->GetCamera()));

	Field::GetInstance()->Update((CameraControl::GetInstance()->GetCamera()));
	CustomButton::GetInstance()->Update();

	SistemConfig::GetInstance()->Update();

}

//����������
void PlayScene::Initialize()
{
	if (AllObjectControl.size() == 0) {
		//�J������ԏ�Ɂ@����Control���J�����������ɂ��Ă邩��
		AllObjectControl.push_back(CameraControl::GetInstance());
		AllObjectControl.push_back(WoodControl::GetInstance());
		AllObjectControl.push_back(FenceControl::GetInstance());
		AllObjectControl.push_back(EnemyControl::GetInstance());
		AllObjectControl.push_back(ChestControl::GetInstance());
		AllObjectControl.push_back(PlayerControl::GetInstance());
	}
	//�e�I�u�W�F�N�g������
	Field::GetInstance()->Initialize((CameraControl::GetInstance()->GetCamera()));
	for (int i = 0; i < AllObjectControl.size(); i++) {
		AllObjectControl[i]->Initialize((CameraControl::GetInstance()->GetCamera()));
	}
	//�I�u�W�F�N�g�ɃJ�����Z�b�g
	Object3d::SetCamera((CameraControl::GetInstance()->GetCamera()));
	//�J�������Z�b�g
	f_Object3d::SetCamera((CameraControl::GetInstance()->GetCamera()));
	//�O���t�B�b�N�p�C�v���C������
	f_Object3d::CreateGraphicsPipeline();

	//�|�X�g�G�t�F�N�g������
	postEffect = new PostEffect();
	postEffect->Initialize();

	//�J�����������v���C�J�b�g�V�[��
	CameraControl::GetInstance()->SetCameraState(CameraControl::PLAYCUTSCENE);

}

//�X�V����
void PlayScene::Update()
{
	if (!Load && !PlayGame) {
		Load = true;
	}
	//�I�u�W�F�N�g�̍X�V����
	objUpdate((CameraControl::GetInstance()->GetCamera()));
	//csv�ǂݍ���
	LoadParam((CameraControl::GetInstance()->GetCamera()));

	//�ݒ��ʒ��|�X�g�G�t�F�N�g�i���͐F���]�̂݁j
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
//�`��i�I�u�W�F�N�g�j
void PlayScene::MyGameDraw()
{
	Field::GetInstance()->Draw();

	if (EnemyControl::GetInstance()->GetQuentity() > 1) {
		for (int i = 0; i < AllObjectControl.size(); i++) {
			AllObjectControl[i]->Draw();
		}
	}
}

//�`��܂Ƃ�
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

		DirectXCommon::GetInstance()->EndDraw();
		break;
	}
}

//csv�ǂݍ���
void PlayScene::LoadParam(DebugCamera* camera)
{
	if (Load) {
		for (int i = 0; i < AllObjectControl.size(); i++) {
			AllObjectControl[i]->Load((CameraControl::GetInstance()->GetCamera()));
		}
		hudload = true;
		Load = false;
		PlayGame = true;
	}
}

//�������
void PlayScene::Finalize()
{
	for (int i = 0; i < AllObjectControl.size(); i++) {
		AllObjectControl[i]->Finalize();
	}
	Field::GetInstance()->Finalize();
	AllObjectControl.clear();

}