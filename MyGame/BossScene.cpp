#include "BossScene.h"
#include"TitleScene.h"
#include"SceneManager.h"
#include"Field.h"
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
#include"DamageManager.h"
BossScene::BossScene(SceneManager* sceneManager)
	:BaseScene(sceneManager)
{

}

/*------------------------*/
/*-------����������-------*/
/*-----------------------*/
void BossScene::Initialize()
{
	DebugTxt::GetInstance()->Initialize(47);
	//�e�I�u�W�F�N�g�̏�����
	if (AllObjectControl.size() == 0) {//�e�I�u�W�F�N�g�C���X�^���X�Ԃ�����
		AllObjectControl.push_back(CameraControl::GetInstance());
		AllObjectControl.push_back(PlayerControl::GetInstance());
		AllObjectControl.push_back(EnemyControl::GetInstance());
	}
	for (int i = 0; i < AllObjectControl.size(); i++) {
		AllObjectControl[i]->Initialize(CameraControl::GetInstance()->GetCamera());
	}

	Field::GetInstance()->Initialize(CameraControl::GetInstance()->GetCamera());
	
	//�{�X�U���p->�ł���Έڂ�
	Nail::GetInstance()->ModelSet();
	
	postEffect = new MinimapSprite();
	postEffect->Initialize();
	//�J�������Z�b�g
	f_Object3d::SetCamera(CameraControl::GetInstance()->GetCamera());
	//�O���t�B�b�N�p�C�v���C������
	f_Object3d::CreateGraphicsPipeline();

	//�J�����������{�X�J�b�g�V�[��
	CameraControl::GetInstance()->SetCameraState(CameraControl::BOSSCUTSCENE);
	dc = new DebugCamera(WinApp::window_width, WinApp::window_height);

}

/*------------------------*/
/*--------�X�V����--------*/
/*-----------------------*/
void BossScene::Update()
{
	//�ǂݍ���
	if (!LoadEnemy&&!Play) {
		LoadEnemy = true;
	}

	SistemConfig::GetInstance()->Update();

	if (Play) {//csv����̓ǂݍ��ݏI����Ă���X�V����
		//1->Player  0->Camera �J�����̒����_Player�ɍ��킷�̂�Player����
		AllObjectControl[1]->Update(CameraControl::GetInstance()->GetCamera());
		AllObjectControl[0]->Update(CameraControl::GetInstance()->GetCamera());
		for (int i = 2; i < AllObjectControl.size(); i++) {
			AllObjectControl[i]->Update(CameraControl::GetInstance()->GetCamera());
		}
		Nail::GetInstance()->Update();
		UI::GetInstance()->HUDUpdate(hudload, CameraControl::GetInstance()->GetCamera());
		}
	dc->Update();

	dc->SetTarget({ CameraControl::GetInstance()->GetCamera()->GetTarget() });
	dc->SetEye({ PlayerControl::GetInstance()->GetPlayer()->GetPosition().x,
		 PlayerControl::GetInstance()->GetPlayer()->GetPosition().y + 300.0f,
		PlayerControl::GetInstance()->GetPlayer()->GetPosition().z - 1 });
	Field::GetInstance()->SetCamera(dc);

	Field::GetInstance()->Update(CameraControl::GetInstance()->GetCamera());
	DamageManager::GetIns()->Upda();

	//�e�I�u�W�F�N�g�̍X�V����
	//csv�ǂݍ��ݕ���(Camera�̍X�V��ɂ���̂�objUpdate()����ł���)
	LoadParam(CameraControl::GetInstance()->GetCamera());
	if (scenechange && Feed::GetInstance()->GetAlpha() >= 1.0f) {//��ʐ^�����Ȃ�����
		BaseScene* scene = new BossScene(sceneManager_);//���̃V�[���̃C���X�^���X����
		Play = false;
		SceneManager::GetInstance()->SetScene(SceneManager::BOSS);
		sceneManager_->SetnextScene(scene);//�V�[���̃Z�b�g
	}
	if (scenechange) {
		Feed::GetInstance()->Update_White(Feed::FEEDIN);//�����Ȃ�܂�
	}
	if (SistemConfig::GetInstance()->GetConfigJudgMent()) {
		c_postEffect = Blur;
	} else {
		c_postEffect = Default;
	}
}

/*------------------------*/
/*--------�`�揈��--------*/
/*-----------------------*/
void BossScene::MyGameDraw()
{
	Field::GetInstance()->Draw();
	if (Play) {
		for (int i = 0; i < AllObjectControl.size(); i++) {
			AllObjectControl[i]->Draw();
		}
	}
}

/*------------------------*/
/*--------�`�揈��--------*/
/*-----------------------*/
void BossScene::Draw()
{
	//�|�X�g�G�t�F�N�g�̏ꍇ�킯(B�łڂ��� D���f�t�H���g)
	switch (c_postEffect)
	{
	case Blur://�ڂ����@�`�揀�Ⴄ����
		postEffect->PreDrawScene();
		//MyGameDraw();
		postEffect->PostDrawScene();

		DirectXCommon::GetInstance()->BeginDraw();
	//	postEffect->Draw();
		SistemConfig::GetInstance()->SwordPedestalDraw();

		//UI
		if (HUD::GetInstance()->GetLayOutMode()) {
			UI::GetInstance()->HUDDraw();
		}
		//�ݒ���
		SistemConfig::GetInstance()->Draw();
		
		DirectXCommon::GetInstance()->EndDraw();
		break;

	case Default://���ʂ̂���ɉ����������Ă��Ȃ�
		postEffect->PreDrawScene();

		Field::GetInstance()->MiniFieldDraw();

		//Field::GetInstance()->Draw();

		postEffect->PostDrawScene();

		DirectXCommon::GetInstance()->BeginDraw();
		MyGameDraw();
		DamageManager::GetIns()->Draw();

		postEffect->Draw();
		//UI
		if (CameraControl::GetInstance()->GetCameraState() != CameraControl::BOSSCUTSCENE) {
			UI::GetInstance()->HUDDraw();
		}
		Feed::GetInstance()->Draw();
		Field::GetInstance()->WarningDraw();
		SistemConfig::GetInstance()->Draw();

		DirectXCommon::GetInstance()->EndDraw();
		break;
	}
}

/*------------------------*/
/*--------�Ǎ�����--------*/
/*-----------------------*/
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

/*------------------------*/
/*--------�������--------*/
/*-----------------------*/
void BossScene::Finalize()
{
	UI::GetInstance()->Finalize();
	SistemConfig::GetInstance()->Finalize();
	AttackCollision::GetInstance()->Finalize();
	AllObjectControl.clear();
	Field::GetInstance()->Finalize();
}