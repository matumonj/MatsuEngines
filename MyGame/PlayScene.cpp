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
#include"CustomButton.h"
#include"Feed.h"
#include"PlayerControl.h"
#include"BossScene.h"
#include"SelectSword.h"
#include"DamageManager.h"
#include "GameOver.h"
#include "HouseControl.h"
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

	if (AllObjectControl.size() == 0)
	{
		//�J������ԏ�Ɂ@����Control���J�����������ɂ��Ă邩��
		AllObjectControl.push_back(CameraControl::GetInstance());
		AllObjectControl.push_back(EnemyControl::GetInstance());
		AllObjectControl.push_back(PlayerControl::GetInstance());
		AllObjectControl.push_back(ChestControl::GetInstance());
		AllObjectControl.push_back(HouseControl::GetInstance());
		AllObjectControl.push_back(WoodControl::GetInstance());
		AllObjectControl.push_back(FenceControl::GetInstance());
	}

	//�|�X�g�G�t�F�N�g������
	postEffect = new PostEffect();
	postEffect->Initialize();
}

/*------------------------*/
/*--------�X�V����--------*/
/*-----------------------*/
void PlayScene::objUpdate(DebugCamera* camera)
{
	if (PlayGame)
	{
		for (int i = 0; i < AllObjectControl.size(); i++)
		{
			if (AllObjectControl[i] == nullptr)
			{
				continue;
			}
			AllObjectControl[i]->Update((CameraControl::GetInstance()->GetCamera()));
		}
	}

	TargetMarker::GetInstance()->Update_PlayScene(CameraControl::GetInstance()->GetCamera());

	if (CameraControl::GetInstance()->GetCamera() != nullptr)
	{
		Field::GetInstance()->Update((CameraControl::GetInstance()->GetCamera()));
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
	objUpdate((CameraControl::GetInstance()->GetCamera()));

	//csv�ǂݍ���
	LoadParam((CameraControl::GetInstance()->GetCamera()));

	//�ݒ��ʒ��|�X�g�G�t�F�N�g�i���͐F���]�̂݁j
	if (SistemConfig::GetInstance()->GetConfigJudgMent())
	{
		c_postEffect = Blur;
	}
	else
	{
		c_postEffect = Default;
	}


	XMFLOAT3 ppos = PlayerControl::GetInstance()->GetPlayer()->GetPosition();

	lightGroup->SetCircleShadowDir(3, XMVECTOR({circleShadowDir[0], circleShadowDir[1], circleShadowDir[2], 0}));
	lightGroup->SetCircleShadowCasterPos(3, {ppos.x, ppos.y + 2.0f, ppos.z});
	lightGroup->SetCircleShadowAtten(3, XMFLOAT3(circleShadowAtten));
	lightGroup->SetCircleShadowFactorAngle(3, XMFLOAT2(circleShadowFactorAngle2));

	for (int i = 0; i < EnemyControl::GetInstance()->GetQuentity(); i++)
	{
		if (EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE)[i] == nullptr)
		{
			lightGroup->SetCircleShadowFactorAngle(i + 4, {0, 0});
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
	if (Input::GetInstance()->TriggerButton(Input::RT))
	{
		//��ʐ^�����Ȃ�����

		SceneManager::GetInstance()->SetScene(SceneManager::BOSS, sceneManager_);
	}
	ChangeSceneJudg();
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
		postEffect->PreDrawScene();
	//MyGameDraw();
		postEffect->PostDrawScene();

		DirectXCommon::GetInstance()->BeginDraw();
		postEffect->Draw();

		SistemConfig::GetInstance()->Draw();

		DirectXCommon::GetInstance()->EndDraw();
		break;

	case Default: //���ʂ̂���ɉ����������Ă��Ȃ�
		postEffect->PreDrawScene();
		Field::GetInstance()->MiniFieldDraw();
		postEffect->PostDrawScene();

		DirectXCommon::GetInstance()->BeginDraw();
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
		}
		PlayerControl::GetInstance()->GetPlayer()->ParticleDraw();

		DropWeapon::GtIns()->Draw();
	//postEffect->Draw();
		Task::GetInstance()->TargetDraw();
		PlayerControl::GetInstance()->DamageTexDraw();
		for (int i = 0; i < EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE).size(); i++)
		{
			if (EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE)[i] == nullptr)
			{
				continue;
			}
			EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE)[i]->DamageTexDisplay_Draw();
		}

		SistemConfig::GetInstance()->Draw();

		Feed::GetInstance()->Draw();
		if (Feed::GetInstance()->GetAlpha() <= 0.0f)
		{
			UI::GetInstance()->HUDDraw();
		}
		UI::GetInstance()->AreaNameDraw();
		DirectXCommon::GetInstance()->EndDraw();
		break;
	}
}

/*------------------------*/
/*--------�Ǎ�����--------*/
/*-----------------------*/
void PlayScene::LoadParam(DebugCamera* camera)
{
	if (Load)
	{
		for (int i = 0; i < AllObjectControl.size(); i++)
		{
			AllObjectControl[i]->Initialize((CameraControl::GetInstance()->GetCamera()));
		}
		//�J�������Z�b�g
		f_Object3d::SetCamera(CameraControl::GetInstance()->GetCamera());
		//�O���t�B�b�N�p�C�v���C������
		f_Object3d::CreateGraphicsPipeline();

		for (int i = 3; i < EnemyControl::GetInstance()->GetQuentity() + 4; i++)
		{
			lightGroup->SetDirLightActive(i, true);
			lightGroup->SetPointLightActive(i, false);
			lightGroup->SetCircleShadowActive(i, true);
		}
		//�e�I�u�W�F�N�g������
		Field::GetInstance()->Initialize((CameraControl::GetInstance()->GetCamera()));
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
