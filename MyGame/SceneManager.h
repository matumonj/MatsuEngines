#pragma once
#include"BaseScene.h"
#include<thread>

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
	
	enum NowScene{
		TITLE,
		TUTORIAL,
		PLAY,
		BOSS
	};
	NowScene GetScene() { return scene; }
	void SetScene(NowScene scene) { this->scene=scene; }
private:
	
	NowScene scene = TITLE;
	std::thread t;
	BaseScene* Scene;
	BaseScene* nextScene = nullptr;
};

