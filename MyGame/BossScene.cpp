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
#include <BossMap.h>
#include"AltAttack.h"
#include"KnockAttack.h"
#include "Nail.h"

BossScene::BossScene(SceneManager* sceneManager)
	: BaseScene(sceneManager)
{
}

/*------------------------*/
/*-------����������-------*/
/*-----------------------*/
void BossScene::Initialize()
{
	//DebugTxt::GetInstance()->Initialize(47);
	//�e�I�u�W�F�N�g�̏�����
	if (AllObjectControl.size() == 0)
	{
		//�e�I�u�W�F�N�g�C���X�^���X�Ԃ�����
		AllObjectControl.emplace_back(CameraControl::GetInstance());
		AllObjectControl.emplace_back(PlayerControl::GetInstance());
		AllObjectControl.emplace_back(EnemyControl::GetInstance());
	}
	lightGroup = LightGroup::Create();

	Object3d::SetLightGroup(lightGroup);
	//lightGroup2 = LightGroup::Create();
	// 3D�I�u�G�N�g�Ƀ��C�g���Z�b�g
	lightGroup->SetDirLightActive(0, false);
	lightGroup->SetDirLightActive(1, false);
	lightGroup->SetDirLightActive(2, false);

	lightGroup->SetDirLightActive(3, false);
	for (int i = 0; i < 4; i++)
	{
		lightGroup->SetPointLightActive(i, true);
	}
	//�{�X�U���p->�ł���Έڂ�
	Nail::GetInstance()->ModelSet();
	BossMap::GetInstance()->Init();

	LightPos[0] = {0, 20, -100};
	LightPos[1] = {75, 20, 0};
	LightPos[2] = {0, 20, 100};
	LightPos[3] = {-75, 20, 0};

	lightangle[0] = 0;

	lightangle[1] = 90;
	lightangle[2] = 180;
	lightangle[3] = 270;
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
			AllObjectControl[1]->Update();
		}
		if (AllObjectControl[0] != nullptr)
		{
			AllObjectControl[0]->Update();
		}
		for (int i = 2; i < AllObjectControl.size(); i++)
		{
			if (AllObjectControl[i] != nullptr)
			{
				AllObjectControl[i]->Update();
			}
		}
		Nail::GetInstance()->Update();
		UI::GetInstance()->HUDUpdate(hudload, CameraControl::GetInstance()->GetCamera());
	}

	Field::GetInstance()->Update();
	//postEffect->SetCenterpos(HUD::GetInstance()->GetMinimapSprite()->GetPosition());

	//�e�I�u�W�F�N�g�̍X�V����
	//csv�ǂݍ��ݕ���(Camera�̍X�V��ɂ���̂�objUpdate()����ł���)
	LoadParam();

	lightGroup->Update();

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
	XMFLOAT3 ppos = PlayerControl::GetInstance()->GetPlayer()->GetPosition();
	if (Input::GetInstance()->TriggerButton(Input::RT))
	{
		//��ʐ^�����Ȃ�����
		Play = false;
		SceneManager::GetInstance()->SetScene(SceneManager::TITLE, sceneManager_);
	}
	LightUpDownT ++;

	for (int i = 0; i < 4; i++)
	{
		lightangle[i] += 0.1f;

		LightPos[i].x = sinf(lightangle[i] * (3.14f / 180.0f)) * 90.0f;
		LightPos[i].z = cosf(lightangle[i] * (3.14f / 180.0f)) * 130.0f;

		//	LightPos[i].y = -5 + sinf(3.14f * 2.f / 180.f *LightUpDownT )*10;

		lightGroup->SetPointLightPos(i, XMFLOAT3(LightPos[i]));
		lightGroup->SetPointLightColor(i, XMFLOAT3(1, 0.5, 0.5));
		lightGroup->SetPointLightAtten(i, XMFLOAT3(pointLightAtten));
	}
	BossMap::GetInstance()->Upda();
	AltAttack::GetInstance()->Upda();

	GameOver::GetIns()->Update();
}


/*------------------------*/
/*--------�`�揈��--------*/
/*-----------------------*/
void BossScene::MyGameDraw()
{
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
		if (Play)
		{
			if (Field::GetInstance() != nullptr)
			{
				Field::GetInstance()->Draw();
			}
			for (int i = 0; i < AllObjectControl.size(); i++)
			{
				if (AllObjectControl[i] == nullptr)
				{
					continue;
				}
				AllObjectControl[i]->Draw();
			}
			Nail::GetInstance()->Draw();
			BossMap::GetInstance()->Draw();
			EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->DamageTexDisplay_Draw();

			for (int i = 0; i < 2; i++)
			{
				EnemyControl::GetInstance()->GetSummonEnemy(i)->DamageTexDisplay_Draw();
			}
		}

		PlayerControl::GetInstance()->GetPlayer()->ParticleDraw();

		Sprite::PreDraw();
		DebugTextSprite::GetInstance()->DrawAll();
		Sprite::PostDraw();
		PlayerControl::GetInstance()->DamageTexDraw();

	//UI
		if (CameraControl::GetInstance()->GetCameraState() != CameraControl::BOSSCUTSCENE)
		{
			UI::GetInstance()->HUDDraw();
		}
		GameOver::GetIns()->Draw();
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
bool BossScene::LoadParam()
{
	if (LoadEnemy)
	{
		for (int i = 0; i < AllObjectControl.size(); i++)
		{
			AllObjectControl[i]->Initialize();
		}
		//�J�������Z�b�g
		f_Object3d::SetCamera(CameraControl::GetInstance()->GetCamera());
		//�O���t�B�b�N�p�C�v���C������
		f_Object3d::CreateGraphicsPipeline();

		//�J�����������{�X�J�b�g�V�[��
		CameraControl::GetInstance()->SetCameraState(CameraControl::BOSSCUTSCENE);

		Field::GetInstance()->Initialize();
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
	for (int i = 0; i < AllObjectControl.size(); i++)
	{
		AllObjectControl[i]->Finalize();
	}
	AllObjectControl.clear();
	Field::GetInstance()->Finalize();
}
