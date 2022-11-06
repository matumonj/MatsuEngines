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
#include"DamageManager.h"
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
	// ���C�g����
	lightGroup = LightGroup::Create();
	Object3d::SetLightGroup(lightGroup);

	if (AllObjectControl.size() == 0) {
		//�J������ԏ�Ɂ@����Control���J�����������ɂ��Ă邩��
		AllObjectControl.push_back(CameraControl::GetInstance());
		AllObjectControl.push_back(EnemyControl::GetInstance());
		AllObjectControl.push_back(PlayerControl::GetInstance());
		AllObjectControl.push_back(ChestControl::GetInstance());
		AllObjectControl.push_back(WoodControl::GetInstance());
		AllObjectControl.push_back(FenceControl::GetInstance());
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
	//�~�j�}�b�v�p�̃J�����@��ŕʂ̂Ƃ���Ɉڂ�
	dc = new DebugCamera(WinApp::window_width, WinApp::window_height);

	//�|�X�g�G�t�F�N�g������
	postEffect = new MinimapSprite();
	postEffect->Initialize();

	//�J�����������v���C�J�b�g�V�[��
	CameraControl::GetInstance()->SetCameraState(CameraControl::PLAYCUTSCENE);

}

/*------------------------*/
/*--------�X�V����--------*/
/*-----------------------*/
void PlayScene::objUpdate(DebugCamera* camera)
{
	if (PlayGame) {
		AllObjectControl[1]->Update(CameraControl::GetInstance()->GetCamera());
		AllObjectControl[0]->Update(CameraControl::GetInstance()->GetCamera());

		for (int i = 2; i < AllObjectControl.size(); i++) {
			AllObjectControl[i]->Update((CameraControl::GetInstance()->GetCamera()));
		}
	}
	dc->Update();

	dc->SetTarget({ PlayerControl::GetInstance()->GetPlayer()->GetPosition() });
	dc->SetEye({ PlayerControl::GetInstance()->GetPlayer()->GetPosition().x,
		 300.0f,
		PlayerControl::GetInstance()->GetPlayer()->GetPosition().z - 1 });
	Field::GetInstance()->SetCamera(dc);
	
	Field::GetInstance()->Update((CameraControl::GetInstance()->GetCamera()));

	UI::GetInstance()->HUDUpdate(hudload, (CameraControl::GetInstance()->GetCamera()));
}

/*------------------------*/
/*--------�X�V����--------*/
/*-----------------------*/
void PlayScene::Update()
{
	lightGroup->Update();

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
	

	XMFLOAT3 ppos = PlayerControl::GetInstance()->GetPlayer()->GetPosition();

	lightGroup->SetCircleShadowDir(3, XMVECTOR({ circleShadowDir[0], circleShadowDir[1], circleShadowDir[2], 0 }));
	//for (int i = 0; i < 3; i++) {
	lightGroup->SetCircleShadowCasterPos(3, { ppos});
	//}
	//lightGroup->SetCircleShadowCasterPos(1, XMFLOAT3(PlayerControl::GetInstance()->GetPlayer()->GetPosition()));
	lightGroup->SetCircleShadowAtten(3, XMFLOAT3(circleShadowAtten));
	lightGroup->SetCircleShadowFactorAngle(3, XMFLOAT2(circleShadowFactorAngle2));
	postEffect->SetCenterpos(HUD::GetInstance()->GetMinimapSprite()->GetPosition());
	for (int i = 0; i < EnemyControl::GetInstance()->GetQuentity(); i++) {
		if (EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE)[i] == nullptr) { 
			lightGroup->SetCircleShadowFactorAngle(i + 4, {0,0});
			continue;
		}
		lightGroup->SetCircleShadowDir(i+4, XMVECTOR({ circleShadowDir[0], circleShadowDir[1], circleShadowDir[2], 0 }));
		//for (int i = 0; i < 3; i++) {
		lightGroup->SetCircleShadowCasterPos(i+4, {EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE)[i]->GetPosition()});
		//}
		//lightGroup->SetCircleShadowCasterPos(1, XMFLOAT3(PlayerControl::GetInstance()->GetPlayer()->GetPosition()));
		lightGroup->SetCircleShadowAtten(i+4, XMFLOAT3(circleShadowAtten));
		lightGroup->SetCircleShadowFactorAngle(i+4, XMFLOAT2(circleShadowFactorAngle));

	}
	DamageManager::GetIns()->Upda();
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
	for (int i = 0; i < AllObjectControl.size(); i++) {
		AllObjectControl[i]->Draw();
	}
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
		//MyGameDraw();
		postEffect->PostDrawScene();

		DirectXCommon::GetInstance()->BeginDraw();
		postEffect->Draw();
		
		SistemConfig::GetInstance()->Draw();
		
		DirectXCommon::GetInstance()->EndDraw();
		break;

	case Default://���ʂ̂���ɉ����������Ă��Ȃ�
		postEffect->PreDrawScene();
		Field::GetInstance()->MiniFieldDraw();
		postEffect->PostDrawScene();

		DirectXCommon::GetInstance()->BeginDraw();
		MyGameDraw();
		DamageManager::GetIns()->Draw();
		postEffect->Draw();
		PlayerControl::GetInstance()->DamageTexDraw();

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
			for (int i = 3; i < EnemyControl::GetInstance()->GetQuentity()+4; i++) {
				lightGroup->SetDirLightActive(i, true);
				lightGroup->SetPointLightActive(i, false);
				lightGroup->SetCircleShadowActive(i, true);
			}
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