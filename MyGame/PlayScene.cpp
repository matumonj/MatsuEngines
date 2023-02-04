#include "PlayScene.h"
#include"Input.h"
#include"DirectXCommon.h"
#include"DropWeapon.h"
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
#include"Feed.h"
#include"PlayerControl.h"
#include"BossScene.h"
#include"DamageManager.h"
#include "GameOver.h"
#include "GuardianNAttack.h"
#include "GuardianShotAttack.h"
#include "HouseControl.h"
#include "SelectSword.h"
//�V�[���̃R���X�g���N�^
PlayScene::PlayScene(SceneManager* sceneManager)
	: BaseScene(sceneManager)
{
}

/*------------------------*/
/*--------����������-------*/
/*-----------------------*/
void PlayScene::Initialize()
{
	// ���C�g����
	lightGroup = LightGroup::Create();
	Object3d::SetLightGroup(lightGroup);

	/*�I�u�W�F�N�g���Ƃ̃C���X�^���X���i�[*/
		//�J������ԏ��
		AllObjectControl.emplace_back(CameraControl::GetInstance());
		AllObjectControl.emplace_back(EnemyControl::GetInstance());
		AllObjectControl.emplace_back(PlayerControl::GetInstance());
		AllObjectControl.emplace_back(ChestControl::GetInstance());
		AllObjectControl.emplace_back(HouseControl::GetInstance());
		AllObjectControl.emplace_back(WoodControl::GetInstance());
		AllObjectControl.emplace_back(FenceControl::GetInstance());

		circleShadowAtten[0] = -5.2f;
		circleShadowAtten[1] = -0.2f;
		circleShadowAtten[2] = 4.9f;
		
	//�|�X�g�G�t�F�N�g������
	postEffect = new PostEffect();
	postEffect->Initialize();
}

/*------------------------*/
/*--------�X�V����--------*/
/*-----------------------*/
void PlayScene::objUpdate()
{
	if (PlayGame)
	{
		for (int i = 0; i < AllObjectControl.size(); i++)
		{
			if (AllObjectControl[i] == nullptr)
			{
				continue;
			}
			AllObjectControl[i]->Update();
		}
	}
	TargetMarker::GetInstance()->Update_PlayScene(CameraControl::GetInstance()->GetCamera());

	if (CameraControl::GetInstance()->GetCamera() != nullptr)
	{
		Field::GetInstance()->Update();
		UI::GetInstance()->HUDUpdate(hudload, (CameraControl::GetInstance()->GetCamera()));
	}
	DropWeapon::GtIns()->Upda();
}

/*------------------------*/
/*--------�X�V����--------*/
/*-----------------------*/
void PlayScene::Update()
{
	lightGroup->Update();

	SistemConfig::GetInstance()->Update();

	if (!Load && !PlayGame)
	{
		Load = true;
	}

	//�I�u�W�F�N�g�̍X�V����
	objUpdate();

	//csv�ǂݍ���
	LoadParam();

	postEffect->SetVignette_GB(PlayerControl::GetInstance()->GetVignetteAlpha());

	LightUpdate();
	ChangeSceneJudg();
	GameOver::GetIns()->Update();
}

void PlayScene::LightUpdate()
{
	XMFLOAT3 ppos = PlayerControl::GetInstance()->GetPlayer()->GetPosition();

	lightGroup->SetCircleShadowDir(3, XMVECTOR({ circleShadowDir[0], circleShadowDir[1], circleShadowDir[2], 0 }));
	lightGroup->SetCircleShadowCasterPos(3, { ppos.x, ppos.y-3.f, ppos.z });
	lightGroup->SetCircleShadowAtten(3, XMFLOAT3(circleShadowAtten));
	//lightGroup->SetCircleShadowFactorAngle(3, XMFLOAT2(1.4f,1.9f));

	for (int i = 0; i < EnemyControl::GetInstance()->GetQuentity(); i++)
	{
		if (EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE)[i] == nullptr)
		{
			lightGroup->SetCircleShadowFactorAngle(i + 4, { 0, 0 });
			continue;
		}
		lightGroup->SetCircleShadowDir(i + 4, XMVECTOR({
										   circleShadowDir[0], circleShadowDir[1], circleShadowDir[2], 0
			}));
		lightGroup->SetCircleShadowCasterPos(i + 4, {
												 EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE)[i]->
												 GetPosition()
			});
		lightGroup->SetCircleShadowAtten(i + 4, XMFLOAT3(circleShadowAtten));
		lightGroup->SetCircleShadowFactorAngle(i + 4, XMFLOAT2(circleShadowFactorAngle));
	}
}
void PlayScene::ChangeSceneJudg()
{
	if (Task::GetInstance()->GetAllTaskClear())

	{
		if (Collision::GetLength(PlayerControl::GetInstance()->GetPlayer()->GetPosition(), {17, -35, 820}) < 30)
		{
			Feed::GetInstance()->Update_White(Feed::FEEDIN);
			PlayerControl::GetInstance()->GetPlayer()->SetStopFlag(TRUE);

			if (Feed::GetInstance()->GetAlpha() >= 1.0f)
			{
				PlayerControl::GetInstance()->GetPlayer()->SetStopFlag(FALSE);

				SceneManager::GetInstance()->SetScene(SceneManager::BOSS, sceneManager_);
			}
		}
	}
}

/*------------------------*/
/*--------�`�揈��--------*/
/*-----------------------*/
void PlayScene::MyGameDraw()
{
	if (Field::GetInstance() != nullptr)
	{
		Field::GetInstance()->Draw();
		for (int i = 0; i < AllObjectControl.size(); i++)
		{
			if (AllObjectControl[i] == nullptr)
			{
				continue;
			}
			AllObjectControl[i]->Draw();
		}

		GuardianShotAttack::GetIns()->Draw();
		GuardianNAttack::GetIns()->Draw();
		Field::GetInstance()->GuardAreaDraw();
	}
	GameOver::GetIns()->Draw_DestParticle();
	DropWeapon::GtIns()->Draw();
	//postEffect->Draw();
	Task::GetInstance()->TargetDraw();
	SelectSword::GetInstance()->SwordDraw();
	AttackEffect::GetIns()->Draw();
}

void PlayScene::SpriteDraw()
{
	
	
	SistemConfig::GetInstance()->Draw();

	Feed::GetInstance()->Draw();
	if (Feed::GetInstance()->GetAlpha() <= 0.0f)
	{
		UI::GetInstance()->HUDDraw();
	}
	GameOver::GetIns()->Draw();
	UI::GetInstance()->AreaNameDraw();
}

/*------------------------*/
/*--------�`�揈��--------*/
/*-----------------------*/
void PlayScene::Draw()
{
	//�|�X�g�G�t�F�N�g�̏ꍇ�킯(B�łڂ��� D���f�t�H���g)
	switch (c_postEffect)
	{
	case Blur: //�ڂ����@�`�揀�Ⴄ����
		
	case Default: //���ʂ̂���ɉ����������Ă��Ȃ�
		postEffect->PreDrawScene();
		
		postEffect->PostDrawScene();

		DirectXCommon::GetInstance()->BeginDraw();
		MyGameDraw();//postEffect->Draw();
		SpriteDraw();
		DirectXCommon::GetInstance()->EndDraw();
		break;
	}
}

/*------------------------*/
/*--------�Ǎ�����--------*/
/*-----------------------*/
void PlayScene::LoadParam()
{
	if (Load)
	{
		for (int i = 0; i < AllObjectControl.size(); i++)
		{
			AllObjectControl[i]->Initialize();
		}
		HUD::GetInstance()->playerini();
		PlayerControl::GetInstance()->GetPlayer()->SetHP(PlayerControl::GetInstance()->GetPlayer()->GetMaxHP());
		//�J�������Z�b�g
		f_Object3d::SetCamera(CameraControl::GetInstance()->GetCamera());
		//�O���t�B�b�N�p�C�v���C������
		f_Object3d::CreateGraphicsPipeline();

		for (int i = 3; i < EnemyControl::GetInstance()->GetQuentity() + 4; i++)
		{
			lightGroup->SetDirLightActive(i,false);
			lightGroup->SetPointLightActive(i, false);
			lightGroup->SetCircleShadowActive(i, true);
		}
		//�e�I�u�W�F�N�g������
		Field::GetInstance()->Initialize();
		//�J�����������v���C�J�b�g�V�[��
		CameraControl::GetInstance()->SetCameraState(CameraControl::PLAYCUTSCENE);


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
	for (int i = 0; i < AllObjectControl.size(); i++)
	{
		AllObjectControl[i]->Finalize();
	}
	Field::GetInstance()->Finalize();
	AllObjectControl.clear();
}
