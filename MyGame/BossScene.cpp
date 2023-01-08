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
#include <BossMap.h>
#include"AltAttack.h"
#include"KnockAttack.h"
#include "Nail.h"

BossScene::BossScene(SceneManager* sceneManager)
	: BaseScene(sceneManager)
{
}

/*------------------------*/
/*-------初期化処理-------*/
/*-----------------------*/
void BossScene::Initialize()
{
	//DebugTxt::GetInstance()->Initialize(47);
	//各オブジェクトの初期化
	if (AllObjectControl.size() == 0)
	{
		//各オブジェクトインスタンスぶちこむ
		AllObjectControl.emplace_back(CameraControl::GetInstance());
		AllObjectControl.emplace_back(PlayerControl::GetInstance());
		AllObjectControl.emplace_back(EnemyControl::GetInstance());
	}
	lightGroup = LightGroup::Create();

	Object3d::SetLightGroup(lightGroup);
	//lightGroup2 = LightGroup::Create();
	// 3Dオブエクトにライトをセット
	lightGroup->SetDirLightActive(0, false);
	lightGroup->SetDirLightActive(1, false);
	lightGroup->SetDirLightActive(2, false);

	lightGroup->SetDirLightActive(3, false);
	for (int i = 0; i < 4; i++)
	{
		lightGroup->SetPointLightActive(i, true);
	}
	//ボス攻撃用->できれば移す
	Nail::GetInstance()->ModelSet();
	BossMap::GetInstance()->Init();

	LightPos[0] = {0, 20, -100};
	LightPos[1] = {75, 20, 0};
	LightPos[2] = {0, 20, 100};
	LightPos[3] = {-75, 20, 0};

	lightangle[0] = 0;

	lightangle[1] = 90;
	lightangle[2] = 180;
	lightangle[3] = 270;
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
			AllObjectControl[1]->Update();
		}
		if (AllObjectControl[0] != nullptr)
		{
			AllObjectControl[0]->Update();
		}
		for (int i = 2; i < AllObjectControl.size(); i++)
		{
			if (AllObjectControl[i] != nullptr)
			{
				AllObjectControl[i]->Update();
			}
		}
		Nail::GetInstance()->Update();
		UI::GetInstance()->HUDUpdate(hudload, CameraControl::GetInstance()->GetCamera());
	}

	Field::GetInstance()->Update();
	//postEffect->SetCenterpos(HUD::GetInstance()->GetMinimapSprite()->GetPosition());

	//各オブジェクトの更新処理
	//csv読み込み部分(Cameraの更新後にするのでobjUpdate()挟んでから)
	LoadParam();

	lightGroup->Update();

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
	XMFLOAT3 ppos = PlayerControl::GetInstance()->GetPlayer()->GetPosition();
	if (Input::GetInstance()->TriggerButton(Input::RT))
	{
		//画面真っ白なったら
		Play = false;
		SceneManager::GetInstance()->SetScene(SceneManager::TITLE, sceneManager_);
	}
	LightUpDownT ++;

	for (int i = 0; i < 4; i++)
	{
		lightangle[i] += 0.1f;

		LightPos[i].x = sinf(lightangle[i] * (3.14f / 180.0f)) * 90.0f;
		LightPos[i].z = cosf(lightangle[i] * (3.14f / 180.0f)) * 130.0f;

		//	LightPos[i].y = -5 + sinf(3.14f * 2.f / 180.f *LightUpDownT )*10;

		lightGroup->SetPointLightPos(i, XMFLOAT3(LightPos[i]));
		lightGroup->SetPointLightColor(i, XMFLOAT3(1, 0.5, 0.5));
		lightGroup->SetPointLightAtten(i, XMFLOAT3(pointLightAtten));
	}
	BossMap::GetInstance()->Upda();
	AltAttack::GetInstance()->Upda();

	GameOver::GetIns()->Update();
}


/*------------------------*/
/*--------描画処理--------*/
/*-----------------------*/
void BossScene::MyGameDraw()
{
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
		if (Play)
		{
			if (Field::GetInstance() != nullptr)
			{
				Field::GetInstance()->Draw();
			}
			for (int i = 0; i < AllObjectControl.size(); i++)
			{
				if (AllObjectControl[i] == nullptr)
				{
					continue;
				}
				AllObjectControl[i]->Draw();
			}
			Nail::GetInstance()->Draw();
			BossMap::GetInstance()->Draw();
			EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->DamageTexDisplay_Draw();

			for (int i = 0; i < 2; i++)
			{
				EnemyControl::GetInstance()->GetSummonEnemy(i)->DamageTexDisplay_Draw();
			}
		}

		PlayerControl::GetInstance()->GetPlayer()->ParticleDraw();

		Sprite::PreDraw();
		DebugTextSprite::GetInstance()->DrawAll();
		Sprite::PostDraw();
		PlayerControl::GetInstance()->DamageTexDraw();

	//UI
		if (CameraControl::GetInstance()->GetCameraState() != CameraControl::BOSSCUTSCENE)
		{
			UI::GetInstance()->HUDDraw();
		}
		GameOver::GetIns()->Draw();
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
bool BossScene::LoadParam()
{
	if (LoadEnemy)
	{
		for (int i = 0; i < AllObjectControl.size(); i++)
		{
			AllObjectControl[i]->Initialize();
		}
		//カメラをセット
		f_Object3d::SetCamera(CameraControl::GetInstance()->GetCamera());
		//グラフィックパイプライン生成
		f_Object3d::CreateGraphicsPipeline();

		//カメラ挙動をボスカットシーン
		CameraControl::GetInstance()->SetCameraState(CameraControl::BOSSCUTSCENE);

		Field::GetInstance()->Initialize();
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
	for (int i = 0; i < AllObjectControl.size(); i++)
	{
		AllObjectControl[i]->Finalize();
	}
	AllObjectControl.clear();
	Field::GetInstance()->Finalize();
}
