#include "BossScene.h"

#include "AttackEffect.h"
#include"TitleScene.h"
#include"SceneManager.h"
#include"Field.h"
#include"EnemyControl.h"
#include"WoodControl.h"
#include"CameraControl.h"
#include"UI.h"
#include"Feed.h"
#include"PlayerControl.h"
#include "GameOver.h"
#include "HalfAttack.h"
#include "Nail.h"
#include "SelectSword.h"
#include "UltAttack.h"

BossScene::BossScene(SceneManager* sceneManager)
	: BaseScene(sceneManager)
{
}

/*------------------------*/
/*-------����������-------*/
/*-----------------------*/
void BossScene::Initialize()
{
	//�e�I�u�W�F�N�g�̏�����
	AllObjectControl.emplace_back(CameraControl::GetIns());
	AllObjectControl.emplace_back(PlayerControl::GetIns());
	AllObjectControl.emplace_back(EnemyControl::GetIns());

	//���C�g�̐ݒ�
	lightGroup = LightGroup::Create();

	Object3d::SetLightGroup(lightGroup);

	// 3D�I�u�G�N�g�Ƀ��C�g���Z�b�g
	lightGroup->SetDirLightActive(0, false);

	lightGroup->SetCircleShadowActive(SUMMONENEY_A, true);
	lightGroup->SetCircleShadowActive(SUMMONENEY_B, true);

	lightGroup->SetCircleShadowActive(PLAYER, true);
	lightGroup->SetCircleShadowActive(BOSSENEMY, true);

	for (int i = 0; i < 4; i++) {
		lightGroup->SetSpotLightActive(i, false);
	}
	for (int i = 4; i < 10; i++) {
		lightGroup->SetSpotLightActive(i, true);
	}
	circleShadowAtten[0] = -5.2f;
	circleShadowAtten[1] = -0.2f;
	circleShadowAtten[2] = 4.9f;
	circleShadowAtten2[0] = 2.2f;
	circleShadowAtten2[1] = 5.2f;
	circleShadowAtten2[2] = -0.27f;
}

/*------------------------*/
/*--------�X�V����--------*/
/*-----------------------*/
void BossScene::Update()
{
	//�ǂݍ���
	if (!LoadF && !Play)
	{
		LoadF = true;
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

	Field::GetIns()->Update();
	//�e�I�u�W�F�N�g�̍X�V����
	//csv�ǂݍ��ݕ���(Camera�̍X�V��ɂ���̂�objUpdate()����ł���)
	LoadParam();

	lightGroup->Update();

	XMFLOAT3 ppos = PlayerControl::GetIns()->GetPlayer()->GetPosition();

	//���C�e�B���O�X�V
	LightUpdate();

	//�Q�[���I�[�o�[���̏�����
	GameOver::GetIns()->Update();

	ChangeScene();
}


/*------------------------*/
/*--------�`�揈��--------*/
/*-----------------------*/

void BossScene::SpriteDraw()
{
	Field::GetIns()->NameDraw();
	AttackEffect::GetIns()->Draw();
	//UI
	if (CameraControl::GetIns()->GetCameraState() != CameraControl::BOSSCUTSCENE)
	{
		UI::GetIns()->HUDDraw();
	}
	//�Q�[���I�[�o�[���̂Ă�������
	GameOver::GetIns()->Draw();
	//�t�F�[�h�X�v���C�g
	Feed::GetIns()->Draw();
}

void BossScene::MyGameDraw()
{
	if (!Play)
	{
		return;
	}

	if (Field::GetIns() != nullptr)
	{
		Field::GetIns()->Draw();
	}

	if (Feed::GetIns()->GetAlpha() < 1.0f)
	{
		for (auto obj : AllObjectControl)
		{
			obj->Draw();
		}
	}
	SelectSword::GetIns()->SwordDraw();
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
		MyGameDraw();

		postEffect->PostDrawScene();

		DirectXCommon::GetIns()->BeginDraw();
		postEffect->Draw();
	//MyGameDraw();
		SpriteDraw();

		DirectXCommon::GetIns()->EndDraw();
		break;

	case Default: //���ʂ̂���ɉ����������Ă��Ȃ�

		DirectXCommon::GetIns()->BeginDraw();
		MyGameDraw();
		SpriteDraw();
		ImGui::Begin("k");
		ImGui::SliderFloat("atten", &atten.x, -0.2f, 0.f);
		ImGui::SliderFloat("posy", &posy, -10, 30);
		ImGui::SliderFloat("spotx", &spotangle.x, -1, 180);
		ImGui::SliderFloat("spoty", &spotangle.y, -1, 180);
		DirectXCommon::GetIns()->EndDraw();
		break;
	}
}

/*------------------------*/
/*--------�Ǎ�����--------*/
/*-----------------------*/
bool BossScene::LoadParam()
{
	if (LoadF)
	{
		for (auto obj : AllObjectControl)
		{
			obj->Initialize();
		}
		//�J�������Z�b�g
		f_Object3d::SetCamera(CameraControl::GetIns()->GetCamera());
		//�O���t�B�b�N�p�C�v���C������
		f_Object3d::CreateGraphicsPipeline();

		//�J�����������{�X�J�b�g�V�[��
		CameraControl::GetIns()->SetCameraState(CameraControl::BOSSCUTSCENE);

		Field::GetIns()->Initialize();
		hudload = true;
		Play = true;
		LoadF = false;
	}
	return true;
}

/*------------------------*/
/*--------�������--------*/
/*-----------------------*/
void BossScene::Finalize()
{
	for (auto obj : AllObjectControl)
	{
		obj->Finalize();
	}
	AllObjectControl.clear();
	Field::GetIns()->Finalize();
}


void BossScene::ChangeScene()
{
	XMFLOAT3 ClearStagePos = Field::GetIns()->GetClearTexpos();

	bool nextscenejudg = EnemyControl::GetIns()->GetEnemy(EnemyControl::BOSS)[0] == nullptr &&
		Collision::GetLength(PlayerControl::GetIns()->GetPlayer()->GetPosition(),
		                     ClearStagePos) < 5.f;

	if (nextscenejudg)
	{
		PlayerControl::GetIns()->GetPlayer()->SetStopFlag(true);
		//SceneManager::GetIns()->SetScene(SceneManager::GAMECLEAR, sceneManager_);
		Feed::GetIns()->Update_White(Feed::FEEDIN);
		if (Feed::GetIns()->GetAlpha() >= 1.0f)
		{
			Play = false;
			SceneManager::GetIns()->SetScene(SceneManager::GAMECLEAR, sceneManager_);
		}
	}
}

void BossScene::LightUpdate()
{
	//�v���C���[�̉e
	constexpr XMFLOAT2 PlayerCShadowAngle = { 1.4f, 1.9f };
	XMFLOAT3 ppos = PlayerControl::GetIns()->GetPlayer()->GetPosition();
	LightSetParam(PLAYER, true, ppos, XMVECTOR({ circleShadowDir[0], circleShadowDir[1], circleShadowDir[2], 0 }),
		XMFLOAT3(circleShadowAtten), PlayerCShadowAngle);

	//�{�X�̉e

	if (EnemyControl::GetIns()->GetEnemy(EnemyControl::BOSS)[0] != nullptr)
	{
		XMFLOAT3 bpos = EnemyControl::GetIns()->GetEnemy(EnemyControl::BOSS)[0]->GetPosition();
		XMFLOAT3 BossCShadowPos = { bpos.x, bpos.y + 20.f, bpos.z };

		LightSetParam(BOSSENEMY, true, BossCShadowPos,
			XMVECTOR({ circleShadowDir[0], circleShadowDir[1], circleShadowDir[2], 0 }),
			XMFLOAT3(circleShadowAtten2), XMFLOAT2(circleShadowFactorAngle2));
	} else
	{
		lightGroup->SetCircleShadowActive(BOSSENEMY, false);
	}

	//�{�X��������G�̉e
	std::array<XMFLOAT3, 2> summonpos;
	for (int i = 0; i < 2; i++)
	{
		if (HalfAttack::GetIns()->GetSummonEnemy(i) != nullptr)
		{
			LightSetParam(i, true, { summonpos[i].x, summonpos[i].y + 7.f, summonpos[i].z },
				XMVECTOR({ circleShadowDir[0], circleShadowDir[1], circleShadowDir[2], 0 }),
				XMFLOAT3(circleShadowAtten2),
				XMFLOAT2(circleShadowFactorAngle2));
		} else
		{
			lightGroup->SetCircleShadowActive(i, false);
		}
	}
	for (int i = 4; i <=10; i++) {
		lightGroup->SetSpotLightPos(i, { Field::GetIns()->GetTorchPos(i-4).x,Field::GetIns()->GetTorchPos(i-4).y + posy,Field::GetIns()->GetTorchPos(i-4).z });
		lightGroup->SetSpotLightAtten(i, { atten.x,0,atten.x });
		lightGroup->SetSpotLightDir(i, { 0.f,-1.f,0.f });
		lightGroup->SetSpotLightColor(i, { 1.f,0.5f,0.f });
		lightGroup->SetSpotLightFactorAngle(i, { spotangle });
		if (UltAttack::GetIns()->GetFieldDestG())
		{
			lightGroup->SetSpotLightActive(i, false);
		}
	}
}
