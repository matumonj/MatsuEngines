#include "TitleScene.h"
#include"Input.h"
#include"PlayScene.h"
#include"MapCreateScene.h"
#include"SceneManager.h"
#include"Tutorial.h"
#include"Feed.h"
#include"BossScene.h"
TitleScene::TitleScene(SceneManager* sceneManager)
	:BaseScene(sceneManager)
{

}
/// <summary>
/// ������
/// </summary>
void TitleScene::Initialize()
{
	Sprite::LoadTexture(3, L"Resources/background.png");
	titlesprite = Sprite::Create(3, { 0.0f,0.0f });
	Feed::GetInstance()->initialize();
}

/// <summary>
/// �X�V����
/// </summary>
void TitleScene::Update()
{
	//ENTER�Ŏ��̃V�[����
	if (Input::GetInstance()->TriggerButton(Input::Button_B)) {//�����ꂽ��
		BaseScene* scene = new BossScene(sceneManager_);//���̃V�[���̃C���X�^���X����
		SceneManager::GetInstance()->SetScene(SceneManager::BOSS);
		sceneManager_->SetnextScene(scene);//�V�[���̃Z�b�g
	}
	titlesprite->SetSize({ WinApp::window_width,WinApp::window_height });
}

/// <summary>
/// �X�v���C�g�`��
/// </summary>
/// <param name="cmdList"></param>
void TitleScene::SpriteDraw()
{
	Sprite::PreDraw();
	titlesprite->Draw();
	Sprite::PostDraw();
}

/// <summary>
/// �`��
/// </summary>
/// <param name="cmdList"></param>
void TitleScene::Draw()
{
	//�|�X�g�G�t�F�N�g�̕`��
	DirectXCommon::GetInstance()->BeginDraw();//�`��R�}���h�̏��ւ��
	SpriteDraw();
	ImGui::Begin("Scene");

	if (ImGui::RadioButton("Scene_Play", t)) {

		BaseScene* scene = new BossScene(sceneManager_);//���̃V�[���̃C���X�^���X����
		SceneManager::GetInstance()->SetScene(SceneManager::BOSS);
		sceneManager_->SetnextScene(scene);//�V�[���̃Z�b�g
	}
	if (ImGui::RadioButton("Scene_MapCreate", y)) {
		BaseScene* scene = new MapCreateScene(sceneManager_);//���̃V�[���̃C���X�^���X����
		sceneManager_->SetnextScene(scene);//�V�[���̃Z�b�g
	}
	ImGui::End();
	DirectXCommon::GetInstance()->EndDraw();

}
void TitleScene::Finalize()
{
	//delete postEffect;
	delete titlesprite;
}