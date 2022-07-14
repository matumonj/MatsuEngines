#pragma once
#include"BaseScene.h"
#include<thread>

class SceneManager
{
public:

	//�X�V
	void Update();
	//�`��
	void Draw();
	//���̃V�[���ֈڍs���邽�߂̃Z�b�^�[
	void SetnextScene(BaseScene* nextScene_) { nextScene = nextScene_; }

	void Finalize();
private:
	std::thread t;
	BaseScene* Scene;
	BaseScene* nextScene = nullptr;
};

