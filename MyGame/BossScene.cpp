#include "BossScene.h"
#include"TitleScene.h"
#include"SceneManager.h"
#include"Field.h"
#include"SistemConfig.h"
#include"EnemyControl.h"
#include"WoodControl.h"
#include"FenceControl.h"
#include"ChestControl.h"
#include"CameraControl.h"
#include"UI.h"
#include"PlayScene.h"
#include"Feed.h"
#include"PlayerControl.h"
#include"DamageManager.h"
#include "GameOver.h"

BossScene::BossScene(SceneManager* sceneManager)
	: BaseScene(sceneManager)
{
}

/*------------------------*/
/*-------初期化処理-------*/
/*-----------------------*/
void BossScene::Initialize()
{
	DebugTxt::GetInstance()->Initialize(47);
	//各オブジェクトの初期化
	if (AllObjectControl.size() == 0)
	{
		//各オブジェクトインスタンスぶちこむ
		AllObjectControl.emplace_back(CameraControl::GetInstance());
		AllObjectControl.emplace_back(PlayerControl::GetInstance());
		AllObjectControl.emplace_back(EnemyControl::GetInstance());
	}

	//ボス攻撃用->できれば移す
	Nail::GetInstance()->ModelSet();

	//postEffect = new MinimapSprite();
	//postEffect->Initialize();
	//	dc = new DebugCamera(WinApp::window_width, WinApp::window_height);
}

/*------------------------*/
/*--------更新処理--------*/
/*-----------------------*/
void BossScene::Update()
{
	//読み込み
	if (!LoadEnemy && !Play)
	{
		LoadEnemy = true;
	}

	SistemConfig::GetInstance()->Update();

	if (Play)
	{
		//csvからの読み込み終わってから更新処理

		if (AllObjectControl[1] != nullptr)
		{
			AllObjectControl[1]->Update(CameraControl::GetInstance()->GetCamera());
		}
		if (AllObjectControl[0] != nullptr)
		{
			AllObjectControl[0]->Update(CameraControl::GetInstance()->GetCamera());
		}
		for (int i = 2; i < AllObjectControl.size(); i++)
		{
			if (AllObjectControl[i] != nullptr)
			{
				AllObjectControl[i]->Update(CameraControl::GetInstance()->GetCamera());
			}
		}
	
		Nail::GetInstance()->Update();
		UI::GetInstance()->HUDUpdate(hudload, CameraControl::GetInstance()->GetCamera());
	}

	Field::GetInstance()->Update(CameraControl::GetInstance()->GetCamera());
	//postEffect->SetCenterpos(HUD::GetInstance()->GetMinimapSprite()->GetPosition());

	//各オブジェクトの更新処理
	//csv読み込み部分(Cameraの更新後にするのでobjUpdate()挟んでから)
	LoadParam(CameraControl::GetInstance()->GetCamera());

	if (scenechange)
	{
		Feed::GetInstance()->Update_White(Feed::FEEDIN); //白くなります
	}
	if (SistemConfig::GetInstance()->GetConfigJudgMent())
	{
		c_postEffect = Blur;
	}
	else
	{
		c_postEffect = Default;
	}
	if (PlayerControl::GetInstance()->GetPlayer()->GetHP() <= 0)
	{
		//画面真っ白なったら
		BaseScene* scene = new GameOver(sceneManager_); //次のシーンのインスタンス生成
		Play = false;
		SceneManager::GetInstance()->SetScene(SceneManager::GAMEOVER);
		sceneManager_->SetnextScene(scene); //シーンのセット

	}
}

/*------------------------*/
/*--------描画処理--------*/
/*-----------------------*/
void BossScene::MyGameDraw()
{
	if (Play)
	{

		Field::GetInstance()->Draw();
		for (int i = 0; i < AllObjectControl.size(); i++)
		{
			AllObjectControl[i]->Draw();
		}
	}
}

/*------------------------*/
/*--------描画処理--------*/
/*-----------------------*/
void BossScene::Draw()
{
	//ポストエフェクトの場合わけ(Bでぼかし Dがデフォルト)
	switch (c_postEffect)
	{
	case Blur: //ぼかし　描画準違うだけ
		postEffect->PreDrawScene();
		postEffect->PostDrawScene();

		DirectXCommon::GetInstance()->BeginDraw();
	//設定画面
		SistemConfig::GetInstance()->Draw();

		DirectXCommon::GetInstance()->EndDraw();
		break;

	case Default: //普通のやつ特に何もかかっていない
		DirectXCommon::GetInstance()->BeginDraw();
		MyGameDraw();
	//postEffect->Draw();
		
		EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->DamageTexDisplay_Draw();

		for (int i = 0; i <2; i++) {
			EnemyControl::GetInstance()->GetSummonEnemy(i)->DamageTexDisplay_Draw();
		}
		Sprite::PreDraw();
		DebugTextSprite::GetInstance()->DrawAll();
		Sprite::PostDraw();
		PlayerControl::GetInstance()->DamageTexDraw();

	//UI
		if (CameraControl::GetInstance()->GetCameraState() != CameraControl::BOSSCUTSCENE)
		{
			UI::GetInstance()->HUDDraw();
		}
		Feed::GetInstance()->Draw();
		Field::GetInstance()->WarningDraw();
		SistemConfig::GetInstance()->Draw();

		DirectXCommon::GetInstance()->EndDraw();
		break;
	}
}
#include"ExpPointSystem.h"
/*------------------------*/
/*--------読込処理--------*/
/*-----------------------*/
bool BossScene::LoadParam(DebugCamera* camera)
{
	if (LoadEnemy)
	{
		for (int i = 0; i < AllObjectControl.size(); i++)
		{
			AllObjectControl[i]->Initialize(CameraControl::GetInstance()->GetCamera());
		}
	
		//カメラをセット
		f_Object3d::SetCamera(CameraControl::GetInstance()->GetCamera());
		//グラフィックパイプライン生成
		f_Object3d::CreateGraphicsPipeline();

		//カメラ挙動をボスカットシーン
		CameraControl::GetInstance()->SetCameraState(CameraControl::BOSSCUTSCENE);

		Field::GetInstance()->Initialize(CameraControl::GetInstance()->GetCamera());
		ExpPointSystem::GetInstance()->Init();
		hudload = true;
		Play = true;
		LoadEnemy = false;
	}
	return true;
}

/*------------------------*/
/*--------解放処理--------*/
/*-----------------------*/
void BossScene::Finalize()
{
	AllObjectControl.clear();
	Field::GetInstance()->Finalize();
}
