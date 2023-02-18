#include "Tutorial.h"
#include"SceneManager.h"
#include"Field.h"
#include"TutorialSprite.h"
#include"SistemConfig.h"
#include"EnemyControl.h"
#include"WoodControl.h"
#include"FenceControl.h"
#include"ChestControl.h"
#include"CameraControl.h"
#include"UI.h"
#include"Feed.h"
#include"PlayerControl.h"
#include"StoneControl.h"
#include <SelectSword.h>
#include "GrassFieldControl.h"
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

	feedout = true;
	//Texture::LoadTexture(47, L"Resources/df.png");
	/*オブジェクト初期化*/
	{
		AllObjectControl.emplace_back(CameraControl::GetIns()); //Camera
		AllObjectControl.emplace_back(EnemyControl::GetIns()); //Enemy
		AllObjectControl.emplace_back(PlayerControl::GetIns()); //Player
		AllObjectControl.emplace_back(FenceControl::GetIns()); //Fence
		AllObjectControl.emplace_back(ChestControl::GetIns()); //Chest
		AllObjectControl.emplace_back(WoodControl::GetIns()); //Wood
		AllObjectControl.emplace_back(StoneControl::GetIns()); //Wood
		AllObjectControl.emplace_back(GrassFieldControl::GetIns()); //Wood
	}

	postEffect = new PostEffect();
	postEffect->Initialize();
	c_postEffect = Blur;
	//各種設定画面
	SistemConfig::GetIns()->Initialize();

	SelectSword::GetIns()->Initialize();
}

void Tutorial::LightSet()
{
	// ライト生成
	lightGroup = LightGroup::Create();

	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightActive(1, true);
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

		for (auto obj : AllObjectControl)
		{
			obj->Update();
		}
		UI::GetIns()->HUDUpdate(hudload, CameraControl::GetIns()->GetCamera());
	}
	//後で別所移す
	Field::GetIns()->Update();
}

void Tutorial::Update()
{
	if (feedout)
	{
		Feed::GetIns()->Update_White(Feed::FEEDOUT); //白くなります
		if (Feed::GetIns()->GetAlpha() <= 0.0f)
		{
			feedout = false;
		}
	}
	lightGroup->Update();

	SistemConfig::GetIns()->Update();
	//各オブジェクトの更新処理
	objUpdate(); //オブジェクトの更新処理
	//csv読み込み部分(Cameraの更新後にするのでobjUpdate()挟んでから)
	LoadParam();

	postEffect->SetVignette_GB(PlayerControl::GetIns()->GetVignetteAlpha());
	LightUpdate();
	//一定数進んだらシーンチェンジ
	bool ArrivalJudg = PlayerControl::GetIns()->GetPlayer()->GetPosition().z > -210.0f;
	if (ArrivalJudg)
	{
		scenechange = true;
	}

	if (scenechange)
	{
		Feed::GetIns()->Update_White(Feed::FEEDIN); //白くなります
	}


	if (scenechange && Feed::GetIns()->GetAlpha() >= 1.0f)
	{
		//画面真っ白なったら
		Play = false;
		SceneManager::GetIns()->SetScene(SceneManager::PLAY, sceneManager_);
	}
	if (Input::GetIns()->TriggerButton(Input::START))
	{
		SceneManager::GetIns()->SetScene(SceneManager::BOSS, sceneManager_);
	}
}

void Tutorial::LightUpdate()
{
	XMFLOAT3 ppos = PlayerControl::GetIns()->GetPlayer()->GetPosition();
	if (EnemyControl::GetIns()->GetEnemy(EnemyControl::TUTORIAL)[0] != nullptr)
	{
		lightGroup->SetCircleShadowDir(0, XMVECTOR({circleShadowDir[0], circleShadowDir[1], circleShadowDir[2], 0}));
		lightGroup->SetCircleShadowCasterPos(0, {
			                                     EnemyControl::GetIns()->GetEnemy(EnemyControl::TUTORIAL)[0]->
			                                     GetPosition()
		                                     });
		lightGroup->SetCircleShadowAtten(0, XMFLOAT3(circleShadowAtten));
		lightGroup->SetCircleShadowFactorAngle(0, XMFLOAT2(circleShadowFactorAngle2));
	}
	lightGroup->SetCircleShadowDir(1, XMVECTOR({circleShadowDir[0], circleShadowDir[1], circleShadowDir[2], 0}));
	lightGroup->SetCircleShadowCasterPos(1, {ppos.x, ppos.y + 5.f, ppos.z});
	lightGroup->SetCircleShadowAtten(1, XMFLOAT3(circleShadowAtten));
	lightGroup->SetCircleShadowFactorAngle(1, XMFLOAT2(circleShadowFactorAngle));
}

/*------------------------*/
/*--------描画処理---------*/
/*----------obj----------*/
void Tutorial::MyGameDraw()
{
	Field::GetIns()->Draw();
	if (Play)
	{
		//敵やきのげーむおぶじぇくと
		for (auto obj : AllObjectControl)
		{
			obj->Draw();
		}
	}
	//チュートリアルの案内スプライト
	TutorialSprite::GetIns()->DrawTargetPos();
	//プレイヤーの持つ剣
	SelectSword::GetIns()->SwordDraw();
}

void Tutorial::SpriteDraw()
{
	Sprite::PreDraw();
	if (EnemyControl::GetIns()->GetEnemy(EnemyControl::TUTORIAL)[0] != nullptr)
	{
		EnemyControl::GetIns()->GetEnemy(EnemyControl::TUTORIAL)[0]->DamageTexDisplay_Draw();
	}
	DebugTextSprite::GetIns()->DrawAll();
	Sprite::PostDraw();
	UI::GetIns()->HUDDraw();
	Feed::GetIns()->Draw();
	SistemConfig::GetIns()->Draw();
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

		DirectXCommon::GetIns()->BeginDraw();
		MyGameDraw();
		SpriteDraw();
		DirectXCommon::GetIns()->EndDraw();

		break;

	case Default: //普通のやつ特に何もかかっていない

		postEffect->PreDrawScene();
		postEffect->PostDrawScene();

		DirectXCommon::GetIns()->BeginDraw();
		MyGameDraw();
		SpriteDraw();
		DirectXCommon::GetIns()->EndDraw();

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
		for (auto obj : AllObjectControl)
		{
			//初期化
			obj->Initialize();
		}
		//カメラをセット
		f_Object3d::SetCamera(CameraControl::GetIns()->GetCamera());
		//グラフィックパイプライン生成
		f_Object3d::CreateGraphicsPipeline();

		CameraControl::GetIns()->SetCameraState(CameraControl::PLAYER);

		UI::GetIns()->Initialize();

		Field::GetIns()->Initialize();

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
	for (auto obj : AllObjectControl)
	{
		obj->Finalize();
	}
	AllObjectControl.clear();
	Field::GetIns()->Finalize();
	Destroy(postEffect);
}
