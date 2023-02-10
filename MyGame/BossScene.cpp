#include "BossScene.h"
#include"TitleScene.h"
#include"SceneManager.h"
#include"Field.h"
#include"EnemyControl.h"
#include"WoodControl.h"
#include"CameraControl.h"
#include"UI.h"
#include"Feed.h"
#include"PlayerControl.h"
#include "GameOver.h"
#include "HalfAttack.h"
#include "Nail.h"
#include "SelectSword.h"

BossScene::BossScene(SceneManager* sceneManager)
	: BaseScene(sceneManager)
{
}

/*------------------------*/
/*-------初期化処理-------*/
/*-----------------------*/
void BossScene::Initialize()
{
	//各オブジェクトの初期化
	AllObjectControl.emplace_back(CameraControl::GetInstance());
	AllObjectControl.emplace_back(PlayerControl::GetInstance());
	AllObjectControl.emplace_back(EnemyControl::GetInstance());

	lightGroup = LightGroup::Create();

	Object3d::SetLightGroup(lightGroup);

	// 3Dオブエクトにライトをセット
	lightGroup->SetDirLightActive(0, false);

	lightGroup->SetCircleShadowActive(ShadowChara::SUMMONENEY_A, true);
	lightGroup->SetCircleShadowActive(ShadowChara::SUMMONENEY_B, true);

	lightGroup->SetCircleShadowActive(ShadowChara::PLAYER, true);
	lightGroup->SetCircleShadowActive(ShadowChara::BOSSENEMY, true);
	
	Nail::GetInstance()->ModelSet();

	circleShadowAtten[0] = -5.2f;
	circleShadowAtten[1] = -0.2f;
	circleShadowAtten[2] = 4.9f;
	circleShadowAtten2[0] =2.2f;
	circleShadowAtten2[1] =5.2f;
	circleShadowAtten2[2] = -0.27f;
}

/*------------------------*/
/*--------更新処理--------*/
/*-----------------------*/
void BossScene::Update()
{
	//読み込み
	if (!LoadF&& !Play)
	{
		LoadF = true;
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
	
	Field::GetInstance()->Update();
	//各オブジェクトの更新処理
	//csv読み込み部分(Cameraの更新後にするのでobjUpdate()挟んでから)
	LoadParam();

	lightGroup->Update();
	 
	XMFLOAT3 ppos = PlayerControl::GetInstance()->GetPlayer()->GetPosition();

	 
	//ライティング更新
	LightUpdate();
	
	//ゲームオーバー時の初期化
	GameOver::GetIns()->Update();

	ChangeScene();
}


/*------------------------*/
/*--------描画処理--------*/
/*-----------------------*/

void BossScene::SpriteDraw()
{
	Field::GetInstance()->NameDraw();
	AttackEffect::GetIns()->Draw();
	//UI
	if (CameraControl::GetInstance()->GetCameraState() != CameraControl::BOSSCUTSCENE)
	{
		UI::GetInstance()->HUDDraw();
	}

	GameOver::GetIns()->Draw();

	Feed::GetInstance()->Draw();
	}

void BossScene::MyGameDraw()
{
	if (!Play)return;

	if (Field::GetInstance() != nullptr)
	{
		Field::GetInstance()->Draw();
	}
		
		if (Feed::GetInstance()->GetAlpha() < 1.0f) {
			for (int i = 0; i < AllObjectControl.size(); i++)
			{
				if (AllObjectControl[i] == nullptr)
				{
					continue;
				}
				AllObjectControl[i]->Draw();
			}
		}
		SelectSword::GetInstance()->SwordDraw();
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
		MyGameDraw();
		
		postEffect->PostDrawScene();
		
		DirectXCommon::GetInstance()->BeginDraw();
		postEffect->Draw();
		//MyGameDraw();
		SpriteDraw();
		
		DirectXCommon::GetInstance()->EndDraw();
		break;

	case Default: //普通のやつ特に何もかかっていない
	
		DirectXCommon::GetInstance()->BeginDraw();
		MyGameDraw();
		SpriteDraw();
		DirectXCommon::GetInstance()->EndDraw();
		break;
	}
}
/*------------------------*/
/*--------読込処理--------*/
/*-----------------------*/
bool BossScene::LoadParam()
{
	if (LoadF)
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
		hudload = true;
		Play = true;
		LoadF = false;
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


void BossScene::ChangeScene()
{
	XMFLOAT3 ClearStagePos = Field::GetInstance()->GetClearTexpos();

	bool nextscenejudg = EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]==nullptr&&
		Collision::GetLength(PlayerControl::GetInstance()->GetPlayer()->GetPosition(),
		ClearStagePos) < 5.f;

	if (nextscenejudg)
	{
		PlayerControl::GetInstance()->GetPlayer()->SetStopFlag(true);
		//SceneManager::GetInstance()->SetScene(SceneManager::GAMECLEAR, sceneManager_);
		Feed::GetInstance()->Update_White(Feed::FEEDIN);
		if (Feed::GetInstance()->GetAlpha() >= 1.0f) {

			Play = false;
			SceneManager::GetInstance()->SetScene(SceneManager::GAMECLEAR, sceneManager_);
		}
	}
	
}

void BossScene::LightUpdate()
{
	//プレイヤーの影
	constexpr XMFLOAT2 PlayerCShadowAngle = { 1.4f,1.9f };
	XMFLOAT3 ppos = PlayerControl::GetInstance()->GetPlayer()->GetPosition();
	LightSetParam(ShadowChara::PLAYER, true, ppos, XMVECTOR({ circleShadowDir[0], circleShadowDir[1], circleShadowDir[2], 0 }),
		XMFLOAT3(circleShadowAtten), PlayerCShadowAngle);

	//ボスの影
	XMFLOAT3 bpos = EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->GetPosition();
	XMFLOAT3 BossCShadowPos = { bpos.x,bpos.y + 20.f,bpos.z };
	
	if (EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0] != nullptr) {
		LightSetParam(ShadowChara::BOSSENEMY, true, BossCShadowPos, XMVECTOR({ circleShadowDir[0], circleShadowDir[1], circleShadowDir[2], 0 }),
			XMFLOAT3(circleShadowAtten2), XMFLOAT2(circleShadowFactorAngle2));
	}
	else
	{
		lightGroup->SetCircleShadowActive(ShadowChara::BOSSENEMY, false);
	}

	//ボス召喚する敵の影
	std::array<XMFLOAT3,2>summonpos;
	for (int i = 0; i < 2; i++)
	{
		if (HalfAttack::GetInstance()->GetSummonEnemy(i) != nullptr)
		{
			LightSetParam(i, true, { summonpos[i].x,summonpos[i].y + 7.f,summonpos[i].z },
				XMVECTOR({ circleShadowDir[0], circleShadowDir[1], circleShadowDir[2], 0 }),
				XMFLOAT3(circleShadowAtten2),
				XMFLOAT2(circleShadowFactorAngle2));
		} else
		{
			lightGroup->SetCircleShadowActive(i, false);
		}
	}
}