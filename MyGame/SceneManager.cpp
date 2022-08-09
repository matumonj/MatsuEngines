#include "SceneManager.h"
SceneManager* SceneManager::GetInstance()
{
	static SceneManager instance;
	return &instance;
}
/// <summary>
/// �X�V
/// </summary>
void SceneManager::Update()
{
	if (nextScene) {
		if (Scene) {//�O�̃V�[����j��
			Scene->Finalize();
			delete Scene;
		}
		//���̃V�[���ֈڍs
		Scene = nextScene;
		nextScene = nullptr;
		
		Scene->Initialize();//�ڍs�����珉����
	}
	//�X�V
	Scene->Update();
}

/// <summary>
/// �`��
/// </summary>
/// <param name="cmdList"></param>
void SceneManager::Draw()
{
	Scene->Draw();
}
