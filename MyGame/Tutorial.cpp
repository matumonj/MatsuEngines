#include "Tutorial.h"
#include"Input.h"
#include"TitleScene.h"
#include"SceneManager.h"
#include"MapCreateScene.h"
#include"Field.h"
#include"PlayerAttackState.h"
#include"TutorialSprite.h"
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
#include "DropWeapon.h"
#include <SelectSword.h>
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
	//ライト初期化
	LightSet();
	// 3Dオブエクトにライトをセット
	Object3d::SetLightGroup(lightGroup);
	GrassObj::SetLightGroup(lightGroup);

	//Texture::LoadTexture(47, L"Resources/df.png");
	/*オブジェクト初期化*/
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

	postEffect = new PostEffect();
	postEffect->Initialize();
	//ミニマップ用のカメラ　後で別のところに移す
	//各種設定画面
	SistemConfig::GetInstance()->Initialize();

	SelectSword::GetInstance()->Initialize();
	
}

void Tutorial::LightSet()
{
	// ライト生成
	lightGroup = LightGroup::Create();

	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightActive(1, true);
	lightGroup->SetPointLightActive(0, false);
	lightGroup->SetPointLightActive(1, false);
	lightGroup->SetCircleShadowActive(0, true);
	lightGroup->SetCircleShadowActive(1, true);
}

/*------------------------*/
/*--------更新処理---------*/
/*------------------------*/
void Tutorial::objUpdate()
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
		UI::GetInstance()->HUDUpdate(hudload, CameraControl::GetInstance()->GetCamera());
	}
	//後で別所移す
	Field::GetInstance()->Update();
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
	objUpdate(); //オブジェクトの更新処理
	//csv読み込み部分(Cameraの更新後にするのでobjUpdate()挟んでから)
	LoadParam();

	LightUpdate();
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

	if (scenechange && Feed::GetInstance()->GetAlpha() >= 1.0f)
	{
		//画面真っ白なったら
		Play = false;
		SceneManager::GetInstance()->SetScene(SceneManager::PLAY, sceneManager_);
	}
	if(Input::GetInstance()->TriggerButton(Input::START))
	SceneManager::GetInstance()->SetScene(SceneManager::BOSS, sceneManager_);

}

void Tutorial::LightUpdate()
{
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
		postEffect->PostDrawScene();

		DirectXCommon::GetInstance()->BeginDraw();
		Field::GetInstance()->Draw();
		MyGameDraw();
		PlayerControl::GetInstance()->GetPlayer()->ParticleDraw();
		TutorialSprite::GetInstance()->DrawTargetPos();
		
		Sprite::PreDraw();
		if (EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0] != nullptr)
		{
			EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0]->DamageTexDisplay_Draw();
		}
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
bool Tutorial::LoadParam()
{
	if (Load)
	{
		for (int i = 0; i < AllObjectControl.size(); i++)
		{
			//初期化
			AllObjectControl[i]->Initialize();
		}
		//カメラをセット
		f_Object3d::SetCamera(CameraControl::GetInstance()->GetCamera());
		//グラフィックパイプライン生成
		f_Object3d::CreateGraphicsPipeline();

		CameraControl::GetInstance()->SetCameraState(CameraControl::PLAYER);

		UI::GetInstance()->Initialize();

		Field::GetInstance()->Initialize();

		grassfield = std::make_unique<GrassField>();
		grassfield->Initialize();
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
