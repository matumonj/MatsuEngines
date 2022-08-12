#include "Framework.h"
void Framework::Run()
{
	Initialize();

	while (true) {
		//�E�B���h�E���b�Z�[�W����
		Update();

		//�I�����N�G�X�g�������甲����
		if (IsEndRequst()) {
			break;
		}

		Draw();
	}
	//Finalize();
}
/// <summary>
/// ������
/// </summary>
void Framework::Initialize()
{
	//���ʕ����̃C���X�^���X�����Ə�����
	winapp = new WinApp();
	winapp->Createwindow();
	DirectXCommon::GetInstance()->Initialize(winapp);
	input = Input::GetInstance();
	input->Initialize(winapp);
	audio = new Audio();

	// ���C�g�ÓI������
	Object3d::StaticInitialize(WinApp::window_width, WinApp::window_height);
	//MyGame* MyGame = new MyGame();
	sceneManger = new SceneManager();
	
}

/// <summary>
/// �X�V
/// </summary>
void Framework::Update()
{
	if (winapp->Processmassage()) {
		winRequest = true;
		return;
	}
	input->update();
	sceneManger->Update();
}

/// <summary>
/// �`��
/// </summary>
/// <param name="cmdList"></param>
void Framework::Draw()
{
	sceneManger->Draw();
}

void Framework::Finalize()
{
	FbxLoader::GetInstance()->Finalize();
	//delete sceneManger;
	delete audio;
	//delete dxcomn;
	delete input;
	delete winapp;
}