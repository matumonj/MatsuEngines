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
#include "GameOver.h"

BossScene::BossScene(SceneManager* sceneManager)
	: BaseScene(sceneManager)
{
}

/*------------------------*/
/*-------����������-------*/
/*-----------------------*/
void BossScene::Initialize()
{
	DebugTxt::GetInstance()->Initialize(47);
	//�e�I�u�W�F�N�g�̏�����
	if (AllObjectControl.size() == 0)
	{
		//�e�I�u�W�F�N�g�C���X�^���X�Ԃ�����
		AllObjectControl.emplace_back(CameraControl::GetInstance());
		AllObjectControl.emplace_back(PlayerControl::GetInstance());
		AllObjectControl.emplace_back(EnemyControl::GetInstance());
	}

	//�{�X�U���p->�ł���Έڂ�
	Nail::GetInstance()->ModelSet();

	//postEffect = new MinimapSprite();
	//postEffect->Initialize();
	//	dc = new DebugCamera(WinApp::window_width, WinApp::window_height);
}

/*------------------------*/
/*--------�X�V����--------*/
/*-----------------------*/
void BossScene::Update()
{
	//�ǂݍ���
	if (!LoadEnemy && !Play)
	{
		LoadEnemy = true;
	}

	SistemConfig::GetInstance()->Update();

	if (Play)
	{
		//csv����̓ǂݍ��ݏI����Ă���X�V����

		if (AllObjectControl[1] != nullptr)
		{
			AllObjectControl[1]->Update(CameraControl::GetInstance()->GetCamera());
		}
		if (AllObjectControl[0] != nullptr)
		{
			AllObjectControl[0]->Update(CameraControl::GetInstance()->GetCamera());
		}
		for (int i = 2; i < AllObjectControl.size(); i++)
		{
			if (AllObjectControl[i] != nullptr)
			{
				AllObjectControl[i]->Update(CameraControl::GetInstance()->GetCamera());
			}
		}
	
		Nail::GetInstance()->Update();
		UI::GetInstance()->HUDUpdate(hudload, CameraControl::GetInstance()->GetCamera());
	}

	Field::GetInstance()->Update(CameraControl::GetInstance()->GetCamera());
	//postEffect->SetCenterpos(HUD::GetInstance()->GetMinimapSprite()->GetPosition());

	//�e�I�u�W�F�N�g�̍X�V����
	//csv�ǂݍ��ݕ���(Camera�̍X�V��ɂ���̂�objUpdate()����ł���)
	LoadParam(CameraControl::GetInstance()->GetCamera());

	if (scenechange)
	{
		Feed::GetInstance()->Update_White(Feed::FEEDIN); //�����Ȃ�܂�
	}
	if (SistemConfig::GetInstance()->GetConfigJudgMent())
	{
		c_postEffect = Blur;
	}
	else
	{
		c_postEffect = Default;
	}
	if (PlayerControl::GetInstance()->GetPlayer()->GetHP() <= 0)
	{
		//��ʐ^�����Ȃ�����
		BaseScene* scene = new GameOver(sceneManager_); //���̃V�[���̃C���X�^���X����
		Play = false;
		SceneManager::GetInstance()->SetScene(SceneManager::GAMEOVER);
		sceneManager_->SetnextScene(scene); //�V�[���̃Z�b�g

	}
}

/*------------------------*/
/*--------�`�揈��--------*/
/*-----------------------*/
void BossScene::MyGameDraw()
{
	if (Play)
	{

		Field::GetInstance()->Draw();
		for (int i = 0; i < AllObjectControl.size(); i++)
		{
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
	case Blur: //�ڂ����@�`�揀�Ⴄ����
		postEffect->PreDrawScene();
		postEffect->PostDrawScene();

		DirectXCommon::GetInstance()->BeginDraw();
	//�ݒ���
		SistemConfig::GetInstance()->Draw();

		DirectXCommon::GetInstance()->EndDraw();
		break;

	case Default: //���ʂ̂���ɉ����������Ă��Ȃ�
		DirectXCommon::GetInstance()->BeginDraw();
		MyGameDraw();
	//postEffect->Draw();
		
		EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->DamageTexDisplay_Draw();

		for (int i = 0; i <2; i++) {
			EnemyControl::GetInstance()->GetSummonEnemy(i)->DamageTexDisplay_Draw();
		}
		Sprite::PreDraw();
		DebugTextSprite::GetInstance()->DrawAll();
		Sprite::PostDraw();
		PlayerControl::GetInstance()->DamageTexDraw();

	//UI
		if (CameraControl::GetInstance()->GetCameraState() != CameraControl::BOSSCUTSCENE)
		{
			UI::GetInstance()->HUDDraw();
		}
		Feed::GetInstance()->Draw();
		Field::GetInstance()->WarningDraw();
		SistemConfig::GetInstance()->Draw();

		DirectXCommon::GetInstance()->EndDraw();
		break;
	}
}
#include"ExpPointSystem.h"
/*------------------------*/
/*--------�Ǎ�����--------*/
/*-----------------------*/
bool BossScene::LoadParam(DebugCamera* camera)
{
	if (LoadEnemy)
	{
		for (int i = 0; i < AllObjectControl.size(); i++)
		{
			AllObjectControl[i]->Initialize(CameraControl::GetInstance()->GetCamera());
		}
	
		//�J�������Z�b�g
		f_Object3d::SetCamera(CameraControl::GetInstance()->GetCamera());
		//�O���t�B�b�N�p�C�v���C������
		f_Object3d::CreateGraphicsPipeline();

		//�J�����������{�X�J�b�g�V�[��
		CameraControl::GetInstance()->SetCameraState(CameraControl::BOSSCUTSCENE);

		Field::GetInstance()->Initialize(CameraControl::GetInstance()->GetCamera());
		ExpPointSystem::GetInstance()->Init();
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
	AllObjectControl.clear();
	Field::GetInstance()->Finalize();
}
