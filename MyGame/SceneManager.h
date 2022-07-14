#pragma once
#include"BaseScene.h"
#include<thread>

class SceneManager
{
public:

	//更新
	void Update();
	//描画
	void Draw();
	//次のシーンへ移行するためのセッター
	void SetnextScene(BaseScene* nextScene_) { nextScene = nextScene_; }

	void Finalize();
private:
	std::thread t;
	BaseScene* Scene;
	BaseScene* nextScene = nullptr;
};

