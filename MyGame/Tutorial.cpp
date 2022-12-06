#include "Tutorial.h"
#include"Input.h"
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
#include"BossScene.h"
#include"Feed.h"
#include"BossMap.h"
#include"PlayerControl.h"
#include"PlayScene.h"
#include"DamageManager.h"
#include"StoneControl.h"
#include"GrassFieldControl.h"
#include"SelectSword.h"
//�V�[���̃R���X�g���N�^
Tutorial::Tutorial(SceneManager* sceneManager)
	: BaseScene(sceneManager)
{
}

/*------------------------*/
/*--------����������-------*/
/*------------------------*/
void Tutorial::Initialize()
{
	// ���C�g����
	lightGroup = LightGroup::Create();
	//lightGroup2 = LightGroup::Create();
	// 3D�I�u�G�N�g�Ƀ��C�g���Z�b�g
	Object3d::SetLightGroup(lightGroup);
	GrassObj::SetLightGroup(lightGroup);
	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightActive(1, true);
	lightGroup->SetPointLightActive(0, false);
	lightGroup->SetPointLightActive(1, false);
	lightGroup->SetCircleShadowActive(0, true);
	lightGroup->SetCircleShadowActive(1, true);
	Texture::LoadTexture(47, L"Resources/df.png");
	{
		AllObjectControl.emplace_back(CameraControl::GetInstance()); //Camera
		AllObjectControl.emplace_back(EnemyControl::GetInstance()); //Enemy
		AllObjectControl.emplace_back(PlayerControl::GetInstance()); //Player
		AllObjectControl.emplace_back(FenceControl::GetInstance()); //Fence
		AllObjectControl.emplace_back(ChestControl::GetInstance()); //Chest
		AllObjectControl.emplace_back(WoodControl::GetInstance()); //Wood
		AllObjectControl.emplace_back(StoneControl::GetInstance()); //Wood
		AllObjectControl.emplace_back(GrassFieldControl::GetInstance()); //Wood
	}

	postEffect = new MinimapSprite();
	postEffect->Initialize();
	//�~�j�}�b�v�p�̃J�����@��ŕʂ̂Ƃ���Ɉڂ�
	//�e��ݒ���
	SistemConfig::GetInstance()->Initialize();
	SelectSword::GetInstance()->Initialize();

	Sprite::LoadTexture(0, L"Resources/2d/LevelUp/debugfont.png");
	DebugTextSprite::GetInstance()->Initialize(0);
}

/*------------------------*/
/*--------�X�V����---------*/
/*------------------------*/
void Tutorial::objUpdate(DebugCamera* camera)
{
	if (!Load && !Play)
	{
		Load = true;
	}
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
		UI::GetInstance()->HUDUpdate(hudload, CameraControl::GetInstance()->GetCamera());
	}
	//��ŕʂ񏊈ڂ�

	Field::GetInstance()->Update(CameraControl::GetInstance()->GetCamera());
}

void Tutorial::Update()
{
	if (feedout)
	{
		Feed::GetInstance()->Update_White(Feed::FEEDOUT); //�����Ȃ�܂�
		if (Feed::GetInstance()->GetAlpha() <= 0.0f)
		{
			feedout = false;
		}
	}
	lightGroup->Update();

	SistemConfig::GetInstance()->Update();
	//�e�I�u�W�F�N�g�̍X�V����
	objUpdate(CameraControl::GetInstance()->GetCamera()); //�I�u�W�F�N�g�̍X�V����
	//csv�ǂݍ��ݕ���(Camera�̍X�V��ɂ���̂�objUpdate()����ł���)
	LoadParam(CameraControl::GetInstance()->GetCamera());

	//��萔�i�񂾂�V�[���`�F���W
	bool ArrivalJudg = PlayerControl::GetInstance()->GetPlayer()->GetPosition().z > -470.0f;
	if (ArrivalJudg)
	{
		scenechange = true;
	}

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
	if (EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0] != nullptr)
	{
		lightGroup->SetCircleShadowDir(0, XMVECTOR({circleShadowDir[0], circleShadowDir[1], circleShadowDir[2], 0}));
		lightGroup->SetCircleShadowCasterPos(0, {
			                                     EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0]->
			                                     GetPosition()
		                                     });
		lightGroup->SetCircleShadowAtten(0, XMFLOAT3(circleShadowAtten));
		lightGroup->SetCircleShadowFactorAngle(0, XMFLOAT2(circleShadowFactorAngle2));
	}
	lightGroup->SetCircleShadowDir(1, XMVECTOR({circleShadowDir[0], circleShadowDir[1], circleShadowDir[2], 0}));
	lightGroup->SetCircleShadowCasterPos(1, {ppos});
	lightGroup->SetCircleShadowAtten(1, XMFLOAT3(circleShadowAtten));
	lightGroup->SetCircleShadowFactorAngle(1, XMFLOAT2(circleShadowFactorAngle));
	// BossMap::GetInstance()->Upda();

if (scenechange && Feed::GetInstance()->GetAlpha() >= 1.0f)
	{
		//��ʐ^�����Ȃ�����
		BaseScene* scene = new PlayScene(sceneManager_); //���̃V�[���̃C���X�^���X����
		Play = false;
		SceneManager::GetInstance()->SetScene(SceneManager::PLAY);
		sceneManager_->SetnextScene(scene); //�V�[���̃Z�b�g
	}
if (Input::GetInstance()->TriggerButton(Input::RT)) {
	//��ʐ^�����Ȃ�����
	BaseScene* scene = new BossScene(sceneManager_); //���̃V�[���̃C���X�^���X����
	Play = false;
	SceneManager::GetInstance()->SetScene(SceneManager::BOSS);
	sceneManager_->SetnextScene(scene); //�V�[���̃Z�b�g

	}
}

/*------------------------*/
/*--------�`�揈��---------*/
/*----------obj----------*/
void Tutorial::MyGameDraw()
{
	if (Play)
	{
		for (int i = 0; i < AllObjectControl.size(); i++)
		{
			if (AllObjectControl[i] == nullptr)
			{
				continue;
			}
			AllObjectControl[i]->Draw();
		}
	}
}

/*------------------------*/
/*--------�`�揈��---------*/
/*---------�܂Ƃ�---------*/
void Tutorial::Draw()
{
	//�|�X�g�G�t�F�N�g�̏ꍇ�킯(B�łڂ��� D���f�t�H���g)
	switch (c_postEffect)
	{
	case Blur: //�ڂ����@�`�揀�Ⴄ����
		postEffect->PreDrawScene();
		postEffect->PostDrawScene();

		DirectXCommon::GetInstance()->BeginDraw();

		//SistemConfig::GetInstance()->Draw();
		DirectXCommon::GetInstance()->EndDraw();

		break;

	case Default: //���ʂ̂���ɉ����������Ă��Ȃ�

		postEffect->PreDrawScene();
	//Field::GetInstance()->MiniFieldDraw();
		postEffect->PostDrawScene();

		DirectXCommon::GetInstance()->BeginDraw();
		Field::GetInstance()->Draw();
		MyGameDraw();
	//postEffect->Draw();
		Sprite::PreDraw();
		if (EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0] != nullptr) {
			EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0]->DamageTexDisplay_Draw();
		}
		//BossMap::GetInstance()->Draw();

			DebugTextSprite::GetInstance()->DrawAll();
		Sprite::PostDraw();
		PlayerControl::GetInstance()->DamageTexDraw();
		UI::GetInstance()->HUDDraw();
		Feed::GetInstance()->Draw();
		SistemConfig::GetInstance()->Draw();

		DirectXCommon::GetInstance()->EndDraw();

		break;
	}
}

/*------------------------*/
/*--------�Ǎ�����--------*/
/*-----------------------*/
bool Tutorial::LoadParam(DebugCamera* camera)
{
	if (Load)
	{
		for (int i = 0; i < AllObjectControl.size(); i++)
		{
			//������
			AllObjectControl[i]->Initialize(CameraControl::GetInstance()->GetCamera());
		}
		//BossMap::GetInstance()->Init();
		//�J�������Z�b�g
		f_Object3d::SetCamera(CameraControl::GetInstance()->GetCamera());
		//�O���t�B�b�N�p�C�v���C������
		f_Object3d::CreateGraphicsPipeline();

		CameraControl::GetInstance()->SetCameraState(CameraControl::PLAYER);

		UI::GetInstance()->Initialize();

		Field::GetInstance()->Initialize(CameraControl::GetInstance()->GetCamera());

		grassfield = std::make_unique<GrassField>();
		grassfield->Initialize(CameraControl::GetInstance()->GetCamera());
		hudload = true;
		Play = true;
		Load = false;
	}
	return true;
}

/*------------------------*/
/*--------�������---------*/
/*-----------------------*/
void Tutorial::Finalize()
{
	for (int i = 0; i < AllObjectControl.size(); i++)
	{
		//������
		AllObjectControl[i]->Finalize();
	}
	AllObjectControl.clear();
	Field::GetInstance()->Finalize();
	Destroy(postEffect);
	
}
