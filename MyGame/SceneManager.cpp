#include "SceneManager.h"

#include "ClearScene.h"
#include "ImageManager.h"

SceneManager* SceneManager::GetIns()
{
	static SceneManager instance;
	return &instance;
}

void SceneManager::SetScene(NowScene scene, SceneManager* scman)
{
	this->scene = scene;
	BaseScene* nextscene = nullptr;
	if (scene == TUTORIAL)
	{
		nextscene = new Tutorial(scman); //���̃V�[���̃C���X�^���X����
	}
	if (scene == PLAY)
	{
		nextscene = new PlayScene(scman); //���̃V�[���̃C���X�^���X����
	}
	if (scene == BOSS)
	{
		nextscene = new BossScene(scman); //���̃V�[���̃C���X�^���X����
	}
	if (scene == MAPCREATE)
	{
		nextscene = new MapCreateScene(scman); //���̃V�[���̃C���X�^���X����
	}
	if (scene == TITLE)
	{
		nextscene = new TitleScene(scman); //���̃V�[���̃C���X�^���X����
	}
	if (scene == GAMECLEAR)
	{
		nextscene = new ClearScene(scman); //���̃V�[���̃C���X�^���X����
	}
	scman->SetnextScene(nextscene);
}

/// <summary>
/// �X�V
/// </summary>
void SceneManager::Update()
{
	if (nextScene)
	{
		if (Scene)
		{
			//�O�̃V�[����j��
			Scene->Finalize();
			delete Scene;
		}
		//���̃V�[���ֈڍs
		Scene = nextScene;
		nextScene = nullptr;

		Scene->Initialize(); //�ڍs�����珉����
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



void SceneManager::AsyncLoad()
{
	std::thread t = std::thread([&] {ImageManager::GetIns()->Init(), ModelManager::GetIns()->Initialize();
		});

	//�_�~�[��1�b�҂�
	auto sleepTime = std::chrono::seconds(1);
	std::this_thread::sleep_for(sleepTime);

	t.join();
	// ���[�h���=���[�h�I��
	m_loadType = LoadEnd;
}

void SceneManager::LoadScene()
{
	if (m_Load == true) {
		switch (m_loadType)
		{
		case SceneManager::NoLoad://���[�h���Ă��Ȃ��Ƃ�
			m_th = std::thread([&] {AsyncLoad(); });
			m_loadType = LoadStart;
			break;
		case SceneManager::LoadStart://���[�h���Ă���Ƃ�
			break;
		case SceneManager::LoadEnd://���[�h�I�������
			m_th.join();
			m_loadType = NoLoad;
			m_Load = false;
			break;
		default:
			break;
		}
	}
}
