#include "Tutorial.h"
#include"SceneManager.h"
#include"Field.h"
#include"TutorialSprite.h"
#include"SistemConfig.h"
#include"EnemyControl.h"
#include"WoodControl.h"
#include"FenceControl.h"
#include"ChestControl.h"
#include"CameraControl.h"
#include"UI.h"
#include"Feed.h"
#include"PlayerControl.h"
#include"StoneControl.h"
#include <SelectSword.h>
#include "GrassFieldControl.h"
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
	//���C�g������
	LightSet();
	// 3D�I�u�G�N�g�Ƀ��C�g���Z�b�g
	Object3d::SetLightGroup(lightGroup);
	GrassObj::SetLightGroup(lightGroup);

	feedout = true;
	//Texture::LoadTexture(47, L"Resources/df.png");
	/*�I�u�W�F�N�g������*/
	{
		AllObjectControl.emplace_back(CameraControl::GetIns()); //Camera
		AllObjectControl.emplace_back(EnemyControl::GetIns()); //Enemy
		AllObjectControl.emplace_back(PlayerControl::GetIns()); //Player
		AllObjectControl.emplace_back(FenceControl::GetIns()); //Fence
		AllObjectControl.emplace_back(ChestControl::GetIns()); //Chest
		AllObjectControl.emplace_back(WoodControl::GetIns()); //Wood
		AllObjectControl.emplace_back(StoneControl::GetIns()); //Wood
		AllObjectControl.emplace_back(GrassFieldControl::GetIns()); //Wood
	}

	postEffect = new PostEffect();
	postEffect->Initialize();
	c_postEffect = Blur;
	//�e��ݒ���
	SistemConfig::GetIns()->Initialize();

	SelectSword::GetIns()->Initialize();
}

void Tutorial::LightSet()
{
	// ���C�g����
	lightGroup = LightGroup::Create();

	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightActive(1, true);
	lightGroup->SetCircleShadowActive(0, true);
	lightGroup->SetCircleShadowActive(1, true);
}

/*------------------------*/
/*--------�X�V����---------*/
/*------------------------*/
void Tutorial::objUpdate()
{
	if (!Load && !Play)
	{
		Load = true;
	}
	if (Play)
	{
		//csv����̓ǂݍ��ݏI����Ă���X�V����

		for (auto obj : AllObjectControl)
		{
			obj->Update();
		}
		UI::GetIns()->HUDUpdate(hudload, CameraControl::GetIns()->GetCamera());
	}
	//��ŕʏ��ڂ�
	Field::GetIns()->Update();
}

void Tutorial::Update()
{
	if (feedout)
	{
		Feed::GetIns()->Update_White(Feed::FEEDOUT); //�����Ȃ�܂�
		if (Feed::GetIns()->GetAlpha() <= 0.0f)
		{
			feedout = false;
		}
	}
	lightGroup->Update();

	SistemConfig::GetIns()->Update();
	//�e�I�u�W�F�N�g�̍X�V����
	objUpdate(); //�I�u�W�F�N�g�̍X�V����
	//csv�ǂݍ��ݕ���(Camera�̍X�V��ɂ���̂�objUpdate()����ł���)
	LoadParam();

	postEffect->SetVignette_GB(PlayerControl::GetIns()->GetVignetteAlpha());
	LightUpdate();
	//��萔�i�񂾂�V�[���`�F���W
	bool ArrivalJudg = PlayerControl::GetIns()->GetPlayer()->GetPosition().z > -210.0f;
	if (ArrivalJudg)
	{
		scenechange = true;
	}

	if (scenechange)
	{
		Feed::GetIns()->Update_White(Feed::FEEDIN); //�����Ȃ�܂�
	}


	if (scenechange && Feed::GetIns()->GetAlpha() >= 1.0f)
	{
		//��ʐ^�����Ȃ�����
		Play = false;
		SceneManager::GetIns()->SetScene(SceneManager::PLAY, sceneManager_);
	}
	if (Input::GetIns()->TriggerButton(Input::START))
	{
		SceneManager::GetIns()->SetScene(SceneManager::BOSS, sceneManager_);
	}
}

void Tutorial::LightUpdate()
{
	XMFLOAT3 ppos = PlayerControl::GetIns()->GetPlayer()->GetPosition();
	if (EnemyControl::GetIns()->GetEnemy(EnemyControl::TUTORIAL)[0] != nullptr)
	{
		lightGroup->SetCircleShadowDir(0, XMVECTOR({circleShadowDir[0], circleShadowDir[1], circleShadowDir[2], 0}));
		lightGroup->SetCircleShadowCasterPos(0, {
			                                     EnemyControl::GetIns()->GetEnemy(EnemyControl::TUTORIAL)[0]->
			                                     GetPosition()
		                                     });
		lightGroup->SetCircleShadowAtten(0, XMFLOAT3(circleShadowAtten));
		lightGroup->SetCircleShadowFactorAngle(0, XMFLOAT2(circleShadowFactorAngle2));
	}
	lightGroup->SetCircleShadowDir(1, XMVECTOR({circleShadowDir[0], circleShadowDir[1], circleShadowDir[2], 0}));
	lightGroup->SetCircleShadowCasterPos(1, {ppos.x, ppos.y + 5.f, ppos.z});
	lightGroup->SetCircleShadowAtten(1, XMFLOAT3(circleShadowAtten));
	lightGroup->SetCircleShadowFactorAngle(1, XMFLOAT2(circleShadowFactorAngle));
}

/*------------------------*/
/*--------�`�揈��---------*/
/*----------obj----------*/
void Tutorial::MyGameDraw()
{
	Field::GetIns()->Draw();
	if (Play)
	{
		//�G�₫�̂��[�ނ��Ԃ�������
		for (auto obj : AllObjectControl)
		{
			obj->Draw();
		}
	}
	//�`���[�g���A���̈ē��X�v���C�g
	TutorialSprite::GetIns()->DrawTargetPos();
	//�v���C���[�̎���
	SelectSword::GetIns()->SwordDraw();
}

void Tutorial::SpriteDraw()
{
	Sprite::PreDraw();
	if (EnemyControl::GetIns()->GetEnemy(EnemyControl::TUTORIAL)[0] != nullptr)
	{
		EnemyControl::GetIns()->GetEnemy(EnemyControl::TUTORIAL)[0]->DamageTexDisplay_Draw();
	}
	DebugTextSprite::GetIns()->DrawAll();
	Sprite::PostDraw();
	UI::GetIns()->HUDDraw();
	Feed::GetIns()->Draw();
	SistemConfig::GetIns()->Draw();
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

		DirectXCommon::GetIns()->BeginDraw();
		MyGameDraw();
		SpriteDraw();
		DirectXCommon::GetIns()->EndDraw();

		break;

	case Default: //���ʂ̂���ɉ����������Ă��Ȃ�

		postEffect->PreDrawScene();
		postEffect->PostDrawScene();

		DirectXCommon::GetIns()->BeginDraw();
		MyGameDraw();
		SpriteDraw();
		DirectXCommon::GetIns()->EndDraw();

		break;
	}
}

/*------------------------*/
/*--------�Ǎ�����--------*/
/*-----------------------*/
bool Tutorial::LoadParam()
{
	if (Load)
	{
		for (auto obj : AllObjectControl)
		{
			//������
			obj->Initialize();
		}
		//�J�������Z�b�g
		f_Object3d::SetCamera(CameraControl::GetIns()->GetCamera());
		//�O���t�B�b�N�p�C�v���C������
		f_Object3d::CreateGraphicsPipeline();

		CameraControl::GetIns()->SetCameraState(CameraControl::PLAYER);

		UI::GetIns()->Initialize();

		Field::GetIns()->Initialize();

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
	for (auto obj : AllObjectControl)
	{
		obj->Finalize();
	}
	AllObjectControl.clear();
	Field::GetIns()->Finalize();
	Destroy(postEffect);
}
