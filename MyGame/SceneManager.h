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
	//更新
	void Update();
	//描画
	void Draw();
	//次のシーンへ移行するためのセッター
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


	// 非同期処理
	std::thread m_th = {};
	// ロード状態
	int m_loadType = NoLoad;
	// スレッド間で使用する共有リソースを排他制御する
	std::mutex isLoadedMutex = {};
	//ロードのタイプ
	enum LoadType
	{
		NoLoad,
		LoadStart,
		LoadEnd
	};
	bool m_Load;

	// 非同期ロード
	void AsyncLoad();

	
public:void LoadScene();
	//getter setter
	void SetLoad(bool Load) { this->m_Load = Load; }
	bool GetLoad() { return  m_Load; }
};
