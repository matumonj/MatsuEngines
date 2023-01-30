#include "BossScene.h"
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
#include"UltAttack.h"

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

	//�e�I�u�W�F�N�g�C���X�^���X�Ԃ�����
	//if (AllObjectControl.size() == 0) {
		AllObjectControl.emplace_back(CameraControl::GetInstance());
		AllObjectControl.emplace_back(PlayerControl::GetInstance());
		AllObjectControl.emplace_back(EnemyControl::GetInstance());
	//}
	lightGroup = LightGroup::Create();

	Object3d::SetLightGroup(lightGroup);
	// 3D�I�u�G�N�g�Ƀ��C�g���Z�b�g
	lightGroup->SetDirLightActive(0, false);

	lightGroup->SetCircleShadowActive(0, true);
	lightGroup->SetCircleShadowActive(1, true);

	lightGroup->SetCircleShadowActive(2, true);
	lightGroup->SetCircleShadowActive(3, true);
	
	Nail::GetInstance()->ModelSet();

	postEffect = new PostEffect();
	postEffect->Initialize();
	input = Input::GetInstance();

	circleShadowAtten[0] = -5.2f;
	circleShadowAtten[1] = -0.2f;
	circleShadowAtten[2] = 4.9f;
	circleShadowAtten2[0] = 1.f;
	circleShadowAtten2[1] =2.8f;
	circleShadowAtten2[2] = -0.27f;
}

/*------------------------*/
/*--------�X�V����--------*/
/*-----------------------*/
void BossScene::Update()
{
	//�ǂݍ���
	if (!LoadF&& !Play)
	{
		LoadF = true;
	}
	

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
		UI::GetInstance()->HUDUpdate(hudload, CameraControl::GetInstance()->GetCamera());
	}
	
	Field::GetInstance()->Update();
	//�e�I�u�W�F�N�g�̍X�V����
	//csv�ǂݍ��ݕ���(Camera�̍X�V��ɂ���̂�objUpdate()����ł���)
	LoadParam();

	lightGroup->Update();

	XMFLOAT3 ppos = PlayerControl::GetInstance()->GetPlayer()->GetPosition();

	//���C�e�B���O�X�V
	LightUpdate();
	
	//�Q�[���I�[�o�[���̏�����
	GameOver::GetIns()->Update();

	postEffect->SetVignette_GB(PlayerControl::GetInstance()->GetVignetteAlpha());

	//�|�X�g�G�t�F�N�g(�u���[��������)
	if (UltAttack::GetIns()->GetBloomAlpha() == 0.8f) {
		postEffect->SetBloomF(false);
	}
	else {
		postEffect->SetBloomF(true);
	}
	c_postEffect = Blur;
	//�l��Ult�̕��ł������Ă���[����ł����A��
	postEffect->SetBloomAlpha(UltAttack::GetIns()->GetBloomAlpha());

	ChangeScene();
}


/*------------------------*/
/*--------�`�揈��--------*/
/*-----------------------*/

void BossScene::SpriteDraw()
{
	AttackEffect::GetIns()->Draw();
	//UI
	if (CameraControl::GetInstance()->GetCameraState() != CameraControl::BOSSCUTSCENE)
	{
		UI::GetInstance()->HUDDraw();
	}

	GameOver::GetIns()->Draw();

	Feed::GetInstance()->Draw();
	}

void BossScene::MyGameDraw()
{
	if (!Play)return;

	if (Field::GetInstance() != nullptr)
	{
		Field::GetInstance()->Draw();
	}
		
		if (Feed::GetInstance()->GetAlpha() < 1.0f) {
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
		
		DirectXCommon::GetInstance()->BeginDraw();
		
		postEffect->Draw();
		//UI
		SpriteDraw();
		DirectXCommon::GetInstance()->EndDraw();
		break;

	case Default: //���ʂ̂���ɉ����������Ă��Ȃ�
	
		DirectXCommon::GetInstance()->BeginDraw();
		MyGameDraw();
		//ImGui::Begin("light");
		//ImGui::End();
		SpriteDraw();
		DirectXCommon::GetInstance()->EndDraw();
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
	for (int i = 0; i < AllObjectControl.size(); i++)
	{
		AllObjectControl[i]->Finalize();
	}
	AllObjectControl.clear();
	Field::GetInstance()->Finalize();
}


void BossScene::ChangeScene()
{
	XMFLOAT3 ClearStagePos = Field::GetInstance()->GetClearTexpos();

	bool nextscenejudg = EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]==nullptr&&
		Collision::GetLength(PlayerControl::GetInstance()->GetPlayer()->GetPosition(),
		ClearStagePos) < 5.f;

	if (nextscenejudg)
	{
		PlayerControl::GetInstance()->GetPlayer()->SetStopFlag(true);
		//SceneManager::GetInstance()->SetScene(SceneManager::GAMECLEAR, sceneManager_);
		Feed::GetInstance()->Update_White(Feed::FEEDIN);
		if (Feed::GetInstance()->GetAlpha() >= 1.0f) {

			Play = false;
			SceneManager::GetInstance()->SetScene(SceneManager::GAMECLEAR, sceneManager_);
		}
	}
	
}

void BossScene::LightUpdate()
{
	XMFLOAT3 ppos = PlayerControl::GetInstance()->GetPlayer()->GetPosition();
	lightGroup->SetCircleShadowCasterPos(2, { ppos.x,ppos.y,ppos.z });
	lightGroup->SetCircleShadowAtten(2, XMFLOAT3(circleShadowAtten));
	lightGroup->SetCircleShadowDir(2, XMVECTOR({ circleShadowDir[0], circleShadowDir[1], circleShadowDir[2], 0 }));
	lightGroup->SetCircleShadowFactorAngle(2, XMFLOAT2(1.4f,1.9f));

	if (EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0] != nullptr) {
		lightGroup->SetCircleShadowFactorAngle(3, XMFLOAT2(circleShadowFactorAngle2));
		XMFLOAT3 bpos = EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->GetPosition();
		lightGroup->SetCircleShadowCasterPos(3, { bpos.x,bpos.y + 7.f,bpos.z });
		lightGroup->SetCircleShadowAtten(3, XMFLOAT3(circleShadowAtten2));
		lightGroup->SetCircleShadowDir(3, XMVECTOR({ circleShadowDir[0], circleShadowDir[1], circleShadowDir[2], 0 }));
	}
	else
	{
		lightGroup->SetCircleShadowActive(3, false);
	}
	
	XMFLOAT3 summonpos[2];
	for(int i=0;i<=1;i++)
	{
		if (HalfAttack::GetInstance()->GetSummonEnemy(i) != nullptr)
		{
			lightGroup->SetCircleShadowFactorAngle(i, XMFLOAT2(circleShadowFactorAngle2));
			summonpos[i] = HalfAttack::GetInstance()->GetSummonEnemy(i)->GetPosition();
			lightGroup->SetCircleShadowCasterPos(i, { summonpos[i].x,summonpos[i].y + 7.f,summonpos[i].z});
			lightGroup->SetCircleShadowAtten(i, XMFLOAT3(circleShadowAtten2));
			lightGroup->SetCircleShadowDir(i, XMVECTOR({ circleShadowDir[0], circleShadowDir[1], circleShadowDir[2], 0 }));
		}
		else
		{
			lightGroup->SetCircleShadowActive(i, false);
		}
	}
	XMFLOAT3 latten = { 0.1f,0.1f,0.1f };

}