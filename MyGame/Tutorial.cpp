#include "Tutorial.h"
#include"Input.h"
#include"TitleScene.h"
#include"SceneManager.h"
#include"MapCreateScene.h"
#include"Field.h"
#include"PlayerAttackState.h"
#include"SistemConfig.h"
#include"EnemyControl.h"
#include"WoodControl.h"
#include"FenceControl.h"
#include"ChestControl.h"
#include"CameraControl.h"
#include"UI.h"
#include"BossScene.h"
#include"Feed.h"
#include"BossMap.h"
#include"PlayerControl.h"
#include"PlayScene.h"
#include"DamageManager.h"
#include"StoneControl.h"
#include"GrassFieldControl.h"
#include"SelectSword.h"
//シーンのコンストラクタ
Tutorial::Tutorial(SceneManager* sceneManager)
	: BaseScene(sceneManager)
{
}

/*------------------------*/
/*--------初期化処理-------*/
/*------------------------*/
void Tutorial::Initialize()
{
	// ライト生成
	lightGroup = LightGroup::Create();
	//lightGroup2 = LightGroup::Create();
	// 3Dオブエクトにライトをセット
	Object3d::SetLightGroup(lightGroup);
	GrassObj::SetLightGroup(lightGroup);
	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightActive(1, true);
	lightGroup->SetPointLightActive(0, false);
	lightGroup->SetPointLightActive(1, false);
	lightGroup->SetCircleShadowActive(0, true);
	lightGroup->SetCircleShadowActive(1, true);
	Texture::LoadTexture(47, L"Resources/df.png");
	{
		AllObjectControl.emplace_back(CameraControl::GetInstance()); //Camera
		AllObjectControl.emplace_back(EnemyControl::GetInstance()); //Enemy
		AllObjectControl.emplace_back(PlayerControl::GetInstance()); //Player
		AllObjectControl.emplace_back(FenceControl::GetInstance()); //Fence
		AllObjectControl.emplace_back(ChestControl::GetInstance()); //Chest
		AllObjectControl.emplace_back(WoodControl::GetInstance()); //Wood
		AllObjectControl.emplace_back(StoneControl::GetInstance()); //Wood
		AllObjectControl.emplace_back(GrassFieldControl::GetInstance()); //Wood
	}

	postEffect = new MinimapSprite();
	postEffect->Initialize();
	//ミニマップ用のカメラ　後で別のところに移す
	//各種設定画面
	SistemConfig::GetInstance()->Initialize();
	SelectSword::GetInstance()->Initialize();

	Sprite::LoadTexture(0, L"Resources/2d/LevelUp/debugfont.png");
	DebugTextSprite::GetInstance()->Initialize(0);
}

/*------------------------*/
/*--------更新処理---------*/
/*------------------------*/
void Tutorial::objUpdate(DebugCamera* camera)
{
	if (!Load && !Play)
	{
		Load = true;
	}
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
		UI::GetInstance()->HUDUpdate(hudload, CameraControl::GetInstance()->GetCamera());
	}
	//後で別ん所移す

	Field::GetInstance()->Update(CameraControl::GetInstance()->GetCamera());
}

void Tutorial::Update()
{
	if (feedout)
	{
		Feed::GetInstance()->Update_White(Feed::FEEDOUT); //白くなります
		if (Feed::GetInstance()->GetAlpha() <= 0.0f)
		{
			feedout = false;
		}
	}
	lightGroup->Update();

	SistemConfig::GetInstance()->Update();
	//各オブジェクトの更新処理
	objUpdate(CameraControl::GetInstance()->GetCamera()); //オブジェクトの更新処理
	//csv読み込み部分(Cameraの更新後にするのでobjUpdate()挟んでから)
	LoadParam(CameraControl::GetInstance()->GetCamera());

	//一定数進んだらシーンチェンジ
	bool ArrivalJudg = PlayerControl::GetInstance()->GetPlayer()->GetPosition().z > -470.0f;
	if (ArrivalJudg)
	{
		scenechange = true;
	}

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
	if (EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0] != nullptr)
	{
		lightGroup->SetCircleShadowDir(0, XMVECTOR({circleShadowDir[0], circleShadowDir[1], circleShadowDir[2], 0}));
		lightGroup->SetCircleShadowCasterPos(0, {
			                                     EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0]->
			                                     GetPosition()
		                                     });
		lightGroup->SetCircleShadowAtten(0, XMFLOAT3(circleShadowAtten));
		lightGroup->SetCircleShadowFactorAngle(0, XMFLOAT2(circleShadowFactorAngle2));
	}
	lightGroup->SetCircleShadowDir(1, XMVECTOR({circleShadowDir[0], circleShadowDir[1], circleShadowDir[2], 0}));
	lightGroup->SetCircleShadowCasterPos(1, {ppos});
	lightGroup->SetCircleShadowAtten(1, XMFLOAT3(circleShadowAtten));
	lightGroup->SetCircleShadowFactorAngle(1, XMFLOAT2(circleShadowFactorAngle));
	// BossMap::GetInstance()->Upda();

if (scenechange && Feed::GetInstance()->GetAlpha() >= 1.0f)
	{
		//画面真っ白なったら
		BaseScene* scene = new PlayScene(sceneManager_); //次のシーンのインスタンス生成
		Play = false;
		SceneManager::GetInstance()->SetScene(SceneManager::PLAY);
		sceneManager_->SetnextScene(scene); //シーンのセット
	}
if (Input::GetInstance()->TriggerButton(Input::RT)) {
	//画面真っ白なったら
	BaseScene* scene = new BossScene(sceneManager_); //次のシーンのインスタンス生成
	Play = false;
	SceneManager::GetInstance()->SetScene(SceneManager::BOSS);
	sceneManager_->SetnextScene(scene); //シーンのセット

	}
}

/*------------------------*/
/*--------描画処理---------*/
/*----------obj----------*/
void Tutorial::MyGameDraw()
{
	if (Play)
	{
		for (int i = 0; i < AllObjectControl.size(); i++)
		{
			if (AllObjectControl[i] == nullptr)
			{
				continue;
			}
			AllObjectControl[i]->Draw();
		}
	}
}

/*------------------------*/
/*--------描画処理---------*/
/*---------まとめ---------*/
void Tutorial::Draw()
{
	//ポストエフェクトの場合わけ(Bでぼかし Dがデフォルト)
	switch (c_postEffect)
	{
	case Blur: //ぼかし　描画準違うだけ
		postEffect->PreDrawScene();
		postEffect->PostDrawScene();

		DirectXCommon::GetInstance()->BeginDraw();

		//SistemConfig::GetInstance()->Draw();
		DirectXCommon::GetInstance()->EndDraw();

		break;

	case Default: //普通のやつ特に何もかかっていない

		postEffect->PreDrawScene();
	//Field::GetInstance()->MiniFieldDraw();
		postEffect->PostDrawScene();

		DirectXCommon::GetInstance()->BeginDraw();
		Field::GetInstance()->Draw();
		MyGameDraw();
	//postEffect->Draw();
		Sprite::PreDraw();
		if (EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0] != nullptr) {
			EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0]->DamageTexDisplay_Draw();
		}
		//BossMap::GetInstance()->Draw();

			DebugTextSprite::GetInstance()->DrawAll();
		Sprite::PostDraw();
		PlayerControl::GetInstance()->DamageTexDraw();
		UI::GetInstance()->HUDDraw();
		Feed::GetInstance()->Draw();
		SistemConfig::GetInstance()->Draw();

		DirectXCommon::GetInstance()->EndDraw();

		break;
	}
}

/*------------------------*/
/*--------読込処理--------*/
/*-----------------------*/
bool Tutorial::LoadParam(DebugCamera* camera)
{
	if (Load)
	{
		for (int i = 0; i < AllObjectControl.size(); i++)
		{
			//初期化
			AllObjectControl[i]->Initialize(CameraControl::GetInstance()->GetCamera());
		}
		//BossMap::GetInstance()->Init();
		//カメラをセット
		f_Object3d::SetCamera(CameraControl::GetInstance()->GetCamera());
		//グラフィックパイプライン生成
		f_Object3d::CreateGraphicsPipeline();

		CameraControl::GetInstance()->SetCameraState(CameraControl::PLAYER);

		UI::GetInstance()->Initialize();

		Field::GetInstance()->Initialize(CameraControl::GetInstance()->GetCamera());

		grassfield = std::make_unique<GrassField>();
		grassfield->Initialize(CameraControl::GetInstance()->GetCamera());
		hudload = true;
		Play = true;
		Load = false;
	}
	return true;
}

/*------------------------*/
/*--------解放処理---------*/
/*-----------------------*/
void Tutorial::Finalize()
{
	for (int i = 0; i < AllObjectControl.size(); i++)
	{
		//初期化
		AllObjectControl[i]->Finalize();
	}
	AllObjectControl.clear();
	Field::GetInstance()->Finalize();
	Destroy(postEffect);
	
}
