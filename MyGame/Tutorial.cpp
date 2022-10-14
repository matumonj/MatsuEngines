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
#include"PlayerControl.h"
#include"PlayScene.h"
#include"DamageManager.h"
//�V�[���̃R���X�g���N�^
Tutorial::Tutorial(SceneManager* sceneManager)
	:BaseScene(sceneManager)
{

}

/*------------------------*/
/*--------����������-------*/
/*------------------------*/
void Tutorial::Initialize()
{
	Texture::LoadTexture(47, L"Resources/df.png");
	{
		AllObjectControl.push_back(CameraControl::GetInstance());//Camera
		AllObjectControl.push_back(PlayerControl::GetInstance());//Player
		AllObjectControl.push_back(EnemyControl::GetInstance());//Enemy
		AllObjectControl.push_back(FenceControl::GetInstance());//Fence
		AllObjectControl.push_back(ChestControl::GetInstance());//Chest
		AllObjectControl.push_back(WoodControl::GetInstance());//Wood

	for (int i = 0; i < AllObjectControl.size(); i++) {//������
		AllObjectControl[i]->Initialize(CameraControl::GetInstance()->GetCamera());
		}
	}
	//Field
	Field::GetInstance()->Initialize(CameraControl::GetInstance()->GetCamera());

	//�J�������Z�b�g
	f_Object3d::SetCamera(CameraControl::GetInstance()->GetCamera());
	//�O���t�B�b�N�p�C�v���C������
	f_Object3d::CreateGraphicsPipeline();

	//�t�F�[�h��UI�X�v���C�g������
	Feed::GetInstance()->initialize();
	UI::GetInstance()->Initialize();

	postEffect = new MinimapSprite();
	postEffect->Initialize();
	//�~�j�}�b�v�p�̃J�����@��ŕʂ̂Ƃ���Ɉڂ�
	dc = new DebugCamera(WinApp::window_width, WinApp::window_height);
	//�e��ݒ���
	SistemConfig::GetInstance()->Initialize();
}

/*------------------------*/
/*--------�X�V����---------*/
/*------------------------*/
void Tutorial::objUpdate(DebugCamera* camera)
{
	//�J�����̒����_���v���C���[�ɃZ�b�g
	CameraControl::GetInstance()->SetCameraState(CameraControl::PLAYER);
	if (!Load && !Play) {
		Load = true;
	}
	if (Play) {//csv����̓ǂݍ��ݏI����Ă���X�V����
		AllObjectControl[1]->Update(CameraControl::GetInstance()->GetCamera());
		AllObjectControl[0]->Update(CameraControl::GetInstance()->GetCamera());
		for (int i = 2; i < AllObjectControl.size(); i++) {
			if (AllObjectControl[i] != nullptr) {
				AllObjectControl[i]->Update(CameraControl::GetInstance()->GetCamera());
			}
		}
		UI::GetInstance()->HUDUpdate(hudload, CameraControl::GetInstance()->GetCamera());
	}
	//��ŕʂ񏊈ڂ�
	dc->Update();

	dc->SetTarget({ CameraControl::GetInstance()->GetCamera()->GetTarget() });
	dc->SetEye({ PlayerControl::GetInstance()->GetPlayer()->GetPosition().x,
		 PlayerControl::GetInstance()->GetPlayer()->GetPosition().y + 300.0f,
		PlayerControl::GetInstance()->GetPlayer()->GetPosition().z - 1 });
	Field::GetInstance()->SetCamera(dc);

	Field::GetInstance()->Update(CameraControl::GetInstance()->GetCamera());
}

void Tutorial::Update()
{
	SistemConfig::GetInstance()->Update();
	//�e�I�u�W�F�N�g�̍X�V����
	objUpdate(CameraControl::GetInstance()->GetCamera());//�I�u�W�F�N�g�̍X�V����
	//csv�ǂݍ��ݕ���(Camera�̍X�V��ɂ���̂�objUpdate()����ł���)
	LoadParam(CameraControl::GetInstance()->GetCamera());

	//��萔�i�񂾂�V�[���`�F���W
	bool ArrivalJudg = PlayerControl::GetInstance()->GetPlayer()->GetPosition().z > -470.0f;
	if (ArrivalJudg) {
		scenechange = true;
	}

	if (scenechange) {
		Feed::GetInstance()->Update_White(Feed::FEEDIN);//�����Ȃ�܂�
	}
	if (SistemConfig::GetInstance()->GetConfigJudgMent()) {
		c_postEffect = Blur;
	} else {
		c_postEffect = Default;
	}
	//postEffect->SetSize({ 10,10 });
	
if (scenechange&& Feed::GetInstance()->GetAlpha() >= 1.0f) {//��ʐ^�����Ȃ�����
		BaseScene* scene = new BossScene(sceneManager_);//���̃V�[���̃C���X�^���X����
		Play = false;
		SceneManager::GetInstance()->SetScene(SceneManager::BOSS);
		sceneManager_->SetnextScene(scene);//�V�[���̃Z�b�g
	}
	DamageManager::GetIns()->Upda();

}

/*------------------------*/
/*--------�`�揈��---------*/
/*----------obj----------*/
void Tutorial::MyGameDraw()
{
	if (Play) {
		for (int i = 0; i < AllObjectControl.size(); i++) {
			if (AllObjectControl[i] != nullptr) {
				AllObjectControl[i]->Draw();
			}
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
	case Blur://�ڂ����@�`�揀�Ⴄ����
		postEffect->PreDrawScene();
		SistemConfig::GetInstance()->SwordPedestalDraw();
		postEffect->PostDrawScene();

		DirectXCommon::GetInstance()->BeginDraw();
		SistemConfig::GetInstance()->SwordPedestalDraw();
		if (HUD::GetInstance()->GetLayOutMode()) {
			UI::GetInstance()->HUDDraw();
		}
		SistemConfig::GetInstance()->Draw();
		DirectXCommon::GetInstance()->EndDraw();

		break;

	case Default://���ʂ̂���ɉ����������Ă��Ȃ�
		
		postEffect->PreDrawScene();
		Field::GetInstance()->MiniFieldDraw();
		postEffect->PostDrawScene();

		DirectXCommon::GetInstance()->BeginDraw();
		Field::GetInstance()->Draw();
		MyGameDraw();
		postEffect->Draw();
		DamageManager::GetIns()->Draw();
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
	if (Load) {
		for (int i = 0; i < AllObjectControl.size(); i++) {
			AllObjectControl[i]->Load(CameraControl::GetInstance()->GetCamera());
		}
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
	for (int i = 0; i < AllObjectControl.size(); i++) {//������
		AllObjectControl[i]->Finalize();
	}
	AllObjectControl.clear();
	Field::GetInstance()->Finalize();
	Destroy(postEffect);
}
