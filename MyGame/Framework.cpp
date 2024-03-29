#include "Framework.h"

void Framework::Run()
{
	Initialize();

	while (true)
	{
		//ウィンドウメッセージ処理
		Update();

		//終了リクエストが来たら抜ける
		if (IsEndRequst())
		{
			break;
		}

		Draw();
	}
}

/// <summary>
/// 初期化
/// </summary>
void Framework::Initialize()
{
	//共通部分のインスタンス生成と初期化
	winapp = new WinApp();
	winapp->Createwindow();
	DirectXCommon::GetIns()->Initialize(winapp);
	input = Input::GetIns();
	input->Initialize(winapp);
	audio = new Audio();

	// ライト静的初期化
	Object3d::StaticInitialize(WinApp::window_width, WinApp::window_height);
	GrassObj::StaticInitialize(WinApp::window_width, WinApp::window_height);
	//MyGame* MyGame = new MyGame();
	sceneManger = new SceneManager();
}

/// <summary>
/// 更新
/// </summary>
void Framework::Update()
{
	input->update();
	if (winapp->Processmassage())
	{
		winRequest = true;
		return;
	}
	sceneManger->Update();
}

/// <summary>
/// 描画
/// </summary>
/// <param name="cmdList"></param>
void Framework::Draw()
{
	sceneManger->Draw();
}

void Framework::Finalize()
{
	FbxLoader::GetIns()->Finalize();
	//delete sceneManger;
	delete audio;
	delete input;
	delete winapp;
}
