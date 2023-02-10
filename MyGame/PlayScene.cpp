#include "PlayScene.h"
#include"DirectXCommon.h"
#include"DropWeapon.h"
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
#include"Feed.h"
#include"PlayerControl.h"
#include"DamageManager.h"
#include "GameOver.h"
#include "GuardianNAttack.h"
#include "GuardianShotAttack.h"
#include "HouseControl.h"
#include "SelectSword.h"
//シーンのコンストラクタ
PlayScene::PlayScene(SceneManager* sceneManager)
	: BaseScene(sceneManager)
{
}

/*------------------------*/
/*--------初期化処理-------*/
/*-----------------------*/
void PlayScene::Initialize()
{
	// ライト生成
	lightGroup = LightGroup::Create();
	Object3d::SetLightGroup(lightGroup);

	/*オブジェクトごとのインスタンスを格納*/
		//カメラ一番上に
		AllObjectControl.emplace_back(CameraControl::GetIns());
		AllObjectControl.emplace_back(EnemyControl::GetIns());
		AllObjectControl.emplace_back(PlayerControl::GetIns());
		AllObjectControl.emplace_back(ChestControl::GetIns());
		AllObjectControl.emplace_back(HouseControl::GetIns());
		AllObjectControl.emplace_back(WoodControl::GetIns());
		AllObjectControl.emplace_back(FenceControl::GetIns());

		circleShadowAtten[0] = -5.2f;
		circleShadowAtten[1] = -0.2f;
		circleShadowAtten[2] = 4.9f;
		
	//ポストエフェクト初期化
	postEffect = new PostEffect();
	postEffect->Initialize();
}

/*------------------------*/
/*--------更新処理--------*/
/*-----------------------*/
void PlayScene::objUpdate()
{
	if (PlayGame)
	{
		for (int i = 0; i < AllObjectControl.size(); i++)
		{
			if (AllObjectControl[i] == nullptr)
			{
				continue;
			}
			AllObjectControl[i]->Update();
		}
	}
	TargetMarker::GetIns()->Update_PlayScene(CameraControl::GetIns()->GetCamera());

	if (CameraControl::GetIns()->GetCamera() != nullptr)
	{
		Field::GetIns()->Update();
		UI::GetIns()->HUDUpdate(hudload, (CameraControl::GetIns()->GetCamera()));
	}
	DropWeapon::GtIns()->Upda();
}

/*------------------------*/
/*--------更新処理--------*/
/*-----------------------*/
void PlayScene::Update()
{
	lightGroup->Update();

	SistemConfig::GetIns()->Update();

	if (!Load && !PlayGame)
	{
		Load = true;
	}

	//オブジェクトの更新処理
	objUpdate();

	//csv読み込み
	LoadParam();

	postEffect->SetVignette_GB(PlayerControl::GetIns()->GetVignetteAlpha());

	LightUpdate();
	ChangeSceneJudg();
	GameOver::GetIns()->Update();
}

void PlayScene::LightUpdate()
{
	XMFLOAT3 ppos = PlayerControl::GetIns()->GetPlayer()->GetPosition();

	lightGroup->SetCircleShadowDir(3, XMVECTOR({ circleShadowDir[0], circleShadowDir[1], circleShadowDir[2], 0 }));
	lightGroup->SetCircleShadowCasterPos(3, { ppos.x, ppos.y-3.f, ppos.z });
	lightGroup->SetCircleShadowAtten(3, XMFLOAT3(circleShadowAtten));
	//lightGroup->SetCircleShadowFactorAngle(3, XMFLOAT2(1.4f,1.9f));

	for (int i = 0; i < EnemyControl::GetIns()->GetQuentity(); i++)
	{
		if (EnemyControl::GetIns()->GetEnemy(EnemyControl::PLAYSCENE)[i] == nullptr)
		{
			lightGroup->SetCircleShadowFactorAngle(i + 4, { 0, 0 });
			continue;
		}
		lightGroup->SetCircleShadowDir(i + 4, XMVECTOR({
										   circleShadowDir[0], circleShadowDir[1], circleShadowDir[2], 0
			}));
		lightGroup->SetCircleShadowCasterPos(i + 4, {
												 EnemyControl::GetIns()->GetEnemy(EnemyControl::PLAYSCENE)[i]->
												 GetPosition()
			});
		lightGroup->SetCircleShadowAtten(i + 4, XMFLOAT3(circleShadowAtten));
		lightGroup->SetCircleShadowFactorAngle(i + 4, XMFLOAT2(circleShadowFactorAngle));
	}
}
void PlayScene::ChangeSceneJudg()
{
	if (Task::GetIns()->GetAllTaskClear())

	{
		if (Collision::GetLength(PlayerControl::GetIns()->GetPlayer()->GetPosition(), {17, -35, 820}) < 30)
		{
			Feed::GetIns()->Update_White(Feed::FEEDIN);
			PlayerControl::GetIns()->GetPlayer()->SetStopFlag(TRUE);

			if (Feed::GetIns()->GetAlpha() >= 1.0f)
			{
				PlayerControl::GetIns()->GetPlayer()->SetStopFlag(FALSE);

				SceneManager::GetIns()->SetScene(SceneManager::BOSS, sceneManager_);
			}
		}
	}
}

/*------------------------*/
/*--------描画処理--------*/
/*-----------------------*/
void PlayScene::MyGameDraw()
{
	if (Field::GetIns() != nullptr)
	{
		Field::GetIns()->Draw();
		for (int i = 0; i < AllObjectControl.size(); i++)
		{
			if (AllObjectControl[i] == nullptr)
			{
				continue;
			}
			AllObjectControl[i]->Draw();
		}

		GuardianShotAttack::GetIns()->Draw();
		GuardianNAttack::GetIns()->Draw();
		Field::GetIns()->GuardAreaDraw();
	}
	GameOver::GetIns()->Draw_DestParticle();
	DropWeapon::GtIns()->Draw();

	Task::GetIns()->TargetDraw();
	SelectSword::GetIns()->SwordDraw();
}

void PlayScene::SpriteDraw()
{
	
	
	SistemConfig::GetIns()->Draw();

	Feed::GetIns()->Draw();
	if (Feed::GetIns()->GetAlpha() <= 0.0f)
	{
		UI::GetIns()->HUDDraw();
	}
	GameOver::GetIns()->Draw();
	UI::GetIns()->AreaNameDraw();
}

/*------------------------*/
/*--------描画処理--------*/
/*-----------------------*/
void PlayScene::Draw()
{
	//ポストエフェクトの場合わけ(Bでぼかし Dがデフォルト)
	switch (c_postEffect)
	{
	case Blur: //ぼかし　描画準違うだけ
		
	case Default: //普通のやつ特に何もかかっていない
		postEffect->PreDrawScene();
		
		postEffect->PostDrawScene();

		DirectXCommon::GetIns()->BeginDraw();
		MyGameDraw();//postEffect->Draw();
		SpriteDraw();
		DirectXCommon::GetIns()->EndDraw();
		break;
	}
}

/*------------------------*/
/*--------読込処理--------*/
/*-----------------------*/
void PlayScene::LoadParam()
{
	if (Load)
	{
		for (int i = 0; i < AllObjectControl.size(); i++)
		{
			AllObjectControl[i]->Initialize();
		}
		HUD::GetIns()->playerini();
		PlayerControl::GetIns()->GetPlayer()->SetHP(PlayerControl::GetIns()->GetPlayer()->GetMaxHP());
		//カメラをセット
		f_Object3d::SetCamera(CameraControl::GetIns()->GetCamera());
		//グラフィックパイプライン生成
		f_Object3d::CreateGraphicsPipeline();

		for (int i = 3; i < EnemyControl::GetIns()->GetQuentity() + 4; i++)
		{
			lightGroup->SetDirLightActive(i,false);
			lightGroup->SetPointLightActive(i, false);
			lightGroup->SetCircleShadowActive(i, true);
		}
		//各オブジェクト初期化
		Field::GetIns()->Initialize();
		//カメラ挙動をプレイカットシーン
		CameraControl::GetIns()->SetCameraState(CameraControl::PLAYCUTSCENE);


		hudload = true;
		Load = false;
		PlayGame = true;
	}
}

/*------------------------*/
/*--------解放処理--------*/
/*-----------------------*/
void PlayScene::Finalize()
{
	for (int i = 0; i < AllObjectControl.size(); i++)
	{
		AllObjectControl[i]->Finalize();
	}
	Field::GetIns()->Finalize();
	AllObjectControl.clear();
}
