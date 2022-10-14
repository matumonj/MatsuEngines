#include "PlayScene.h"
#include"Input.h"
#include"DirectXCommon.h"
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
#include"CustomButton.h"
#include"Feed.h"
#include"PlayerControl.h"
#include"BossScene.h"
#include"SelectSword.h"
//�V�[���̃R���X�g���N�^
PlayScene::PlayScene(SceneManager* sceneManager)
	:BaseScene(sceneManager)
{

}

/*------------------------*/
/*--------����������-------*/
/*-----------------------*/
void PlayScene::Initialize()
{
	if (AllObjectControl.size() == 0) {
		//�J������ԏ�Ɂ@����Control���J�����������ɂ��Ă邩��
		AllObjectControl.push_back(CameraControl::GetInstance());
		AllObjectControl.push_back(PlayerControl::GetInstance());
		AllObjectControl.push_back(EnemyControl::GetInstance());
		AllObjectControl.push_back(WoodControl::GetInstance());
		AllObjectControl.push_back(FenceControl::GetInstance());
		AllObjectControl.push_back(ChestControl::GetInstance());
	}

	//�e�I�u�W�F�N�g������
	Field::GetInstance()->Initialize((CameraControl::GetInstance()->GetCamera()));

	for (int i = 0; i < AllObjectControl.size(); i++) {
		AllObjectControl[i]->Initialize((CameraControl::GetInstance()->GetCamera()));
	}
	//�J�������Z�b�g
	f_Object3d::SetCamera(CameraControl::GetInstance()->GetCamera());
	//�O���t�B�b�N�p�C�v���C������
	f_Object3d::CreateGraphicsPipeline();

	//�|�X�g�G�t�F�N�g������
	postEffect = new PostEffect();
	postEffect->Initialize();

	//�J�����������v���C�J�b�g�V�[��
	CameraControl::GetInstance()->SetCameraState(CameraControl::PLAYCUTSCENE);

}

/*------------------------*/
/*--------�X�V����--------*/
/*-----------------------*/
void PlayScene::objUpdate(DebugCamera* camera)
{
	//PlayerControl::GetInstance()->GetPlayer()->SetPosition({ 150.0f,-42.0f,-379.0f });
	Field::GetInstance()->Update((CameraControl::GetInstance()->GetCamera()));
	if (PlayGame) {
		AllObjectControl[1]->Update(CameraControl::GetInstance()->GetCamera());
		AllObjectControl[0]->Update(CameraControl::GetInstance()->GetCamera());

		for (int i = 2; i < AllObjectControl.size(); i++) {
			AllObjectControl[i]->Update((CameraControl::GetInstance()->GetCamera()));
		}
	}
	UI::GetInstance()->HUDUpdate(hudload, (CameraControl::GetInstance()->GetCamera()));

	
}

/*------------------------*/
/*--------�X�V����--------*/
/*-----------------------*/
void PlayScene::Update()
{
	SistemConfig::GetInstance()->Update();

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
	if (CameraControl::GetInstance()->GetMoveBosAreaCam() == CameraControl::TARGETPLAYER) {
		if (Feed::GetInstance()->GetAlpha() >= 1.0f) {//��ʐ^�����Ȃ�����
			BaseScene* scene = new BossScene(sceneManager_);//���̃V�[���̃C���X�^���X����
			SceneManager::GetInstance()->SetScene(SceneManager::BOSS);
			sceneManager_->SetnextScene(scene);//�V�[���̃Z�b�g
		}
	}
}

/*------------------------*/
/*--------�`�揈��--------*/
/*-----------------------*/
void PlayScene::MyGameDraw()
{

	Field::GetInstance()->Draw();
//	if (EnemyControl::GetInstance()->GetQuentity() > 1) {
		for (int i = 0; i < AllObjectControl.size(); i++) {
			AllObjectControl[i]->Draw();
		}
	//}
}

/*------------------------*/
/*--------�`�揈��--------*/
/*-----------------------*/
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
		
		DirectXCommon::GetInstance()->EndDraw();
		break;

	case Default://���ʂ̂���ɉ����������Ă��Ȃ�
		postEffect->PreDrawScene();
		postEffect->Draw();
		postEffect->PostDrawScene();

		DirectXCommon::GetInstance()->BeginDraw();
		MyGameDraw();
		SistemConfig::GetInstance()->Draw();
	
		Feed::GetInstance()->Draw();
			if (Feed::GetInstance()->GetAlpha() <= 0.0f) {
				UI::GetInstance()->HUDDraw();
			}
		
		UI::GetInstance()->AreaNameDraw();
		AttackCollision::GetInstance()->Draw();
		DirectXCommon::GetInstance()->EndDraw();
		break;
	}
}

/*------------------------*/
/*--------�Ǎ�����--------*/
/*-----------------------*/
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

/*------------------------*/
/*--------�������--------*/
/*-----------------------*/
void PlayScene::Finalize()
{
	for (int i = 0; i < AllObjectControl.size(); i++) {
		AllObjectControl[i]->Finalize();
	}
	Field::GetInstance()->Finalize();
	AllObjectControl.clear();
}