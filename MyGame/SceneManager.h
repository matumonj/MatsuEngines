#pragma once
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
	static SceneManager* GetInstance();
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
};
