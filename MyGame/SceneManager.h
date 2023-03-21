#pragma once
#include <mutex>

#include"BaseScene.h"
#include<thread>
#include"Tutorial.h"
#include"PlayScene.h"
#include"BossScene.h"
#include"MapCreateScene.h"
#include"TitleScene.h"

class SceneManager
{
public:
	static SceneManager* GetIns();
	//�X�V
	void Update();
	//�`��
	void Draw();
	//���̃V�[���ֈڍs���邽�߂̃Z�b�^�[
	void SetnextScene(BaseScene* nextScene_) { nextScene = nextScene_; }

	void Finalize();

	enum NowScene
	{
		TITLE,
		TUTORIAL,
		PLAY,
		BOSS,
		MAPCREATE,
		GAMEOVER,
		GAMECLEAR
	};

	NowScene GetScene() { return scene; }
	void SetScene(NowScene scene, SceneManager* scman = nullptr);
private:
	NowScene scene = TITLE;
	std::thread t;
	BaseScene* Scene;
	BaseScene* nextScene = nullptr;


	// �񓯊�����
	std::thread m_th = {};
	// ���[�h���
	int m_loadType = NoLoad;
	// �X���b�h�ԂŎg�p���鋤�L���\�[�X��r�����䂷��
	std::mutex isLoadedMutex = {};
	//���[�h�̃^�C�v
	enum LoadType
	{
		NoLoad,
		LoadStart,
		LoadEnd
	};
	bool m_Load;

	// �񓯊����[�h
	void AsyncLoad();

	
public:void LoadScene();
	//getter setter
	void SetLoad(bool Load) { this->m_Load = Load; }
	bool GetLoad() { return  m_Load; }
};
