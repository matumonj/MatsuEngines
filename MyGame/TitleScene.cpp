#include "TitleScene.h"
#include"Input.h"
#include"PlayScene.h"
#include"MapCreateScene.h"
#include"SceneManager.h"
#include"Tutorial.h"
#include"Feed.h"
#include"BossScene.h"
#include"UI.h"
TitleScene::TitleScene(SceneManager* sceneManager)
	:BaseScene(sceneManager)
{

}

/*------------------------*/
/*-------����������--------*/
/*-----------------------*/
void TitleScene::Initialize()
{
	Sprite::LoadTexture(3, L"Resources/title.png");
	titlesprite = Sprite::Create(3, { 0.0f,0.0f });

	Sprite::LoadTexture(4, L"Resources/title2.png");
	titlesprite2 = Sprite::Create(4, {  WinApp::window_width / 2,0.0f });
	Feed::GetInstance()->initialize();
}

/*------------------------*/
/*-------�X�V����--------*/
/*-----------------------*/
void TitleScene::Update()
{

	if (Input::GetInstance()->TriggerButton(Input::Button_A)) {//�����ꂽ��
		BaseScene* scene = new MapCreateScene(sceneManager_);//���̃V�[���̃C���X�^���X����
		SceneManager::GetInstance()->SetScene(SceneManager::MAPCREATE);
		sceneManager_->SetnextScene(scene);//�V�[���̃Z�b�g
	}
	if (Input::GetInstance()->TriggerButton(Input::Button_B)) {//�����ꂽ��
		BaseScene* scene = new Tutorial(sceneManager_);//���̃V�[���̃C���X�^���X����
		SceneManager::GetInstance()->SetScene(SceneManager::TUTORIAL);
		sceneManager_->SetnextScene(scene);//�V�[���̃Z�b�g
	}
	//titlesprite2->SetRotation(180);
	titlesprite->SetSize({ WinApp::window_width/2,WinApp::window_height });
	titlesprite2->SetSize({ WinApp::window_width / 2,WinApp::window_height });
}

/*------------------------*/
/*-------�`�揈��--------*/
/*-----------------------*/
void TitleScene::SpriteDraw()
{
	Sprite::PreDraw();
	titlesprite2->Draw();
	titlesprite->Draw();
	Sprite::PostDraw();
}

void TitleScene::Draw()
{
	//�|�X�g�G�t�F�N�g�̕`��
	DirectXCommon::GetInstance()->BeginDraw();//�`��R�}���h�̏��ւ��
	SpriteDraw();
	DirectXCommon::GetInstance()->EndDraw();
}


/*------------------------*/
/*-------�������--------*/
/*-----------------------*/
void TitleScene::Finalize()
{
	//delete postEffect;
	delete titlesprite;
}