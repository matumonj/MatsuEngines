#include "SceneManager.h"

#include "ClearScene.h"

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
		nextscene = new Tutorial(scman); //次のシーンのインスタンス生成
	}
	if (scene == PLAY)
	{
		nextscene = new PlayScene(scman); //次のシーンのインスタンス生成
	}
	if (scene == BOSS)
	{
		nextscene = new BossScene(scman); //次のシーンのインスタンス生成
	}
	if (scene == MAPCREATE)
	{
		nextscene = new MapCreateScene(scman); //次のシーンのインスタンス生成
	}
	if (scene == TITLE)
	{
		nextscene = new TitleScene(scman); //次のシーンのインスタンス生成
	}
	if (scene == GAMECLEAR)
	{
		nextscene = new ClearScene(scman); //次のシーンのインスタンス生成
	}
	scman->SetnextScene(nextscene);
}

/// <summary>
/// 更新
/// </summary>
void SceneManager::Update()
{
	if (nextScene)
	{
		if (Scene)
		{
			//前のシーンを破棄
			Scene->Finalize();
			delete Scene;
		}
		//次のシーンへ移行
		Scene = nextScene;
		nextScene = nullptr;

		Scene->Initialize(); //移行したら初期化
	}
	//更新
	Scene->Update();
}

/// <summary>
/// 描画
/// </summary>
/// <param name="cmdList"></param>
void SceneManager::Draw()
{
	Scene->Draw();
}
