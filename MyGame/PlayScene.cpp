#include "PlayScene.h"
#include"DirectXCommon.h"
#include"DropWeapon.h"
#include"TitleScene.h"
#include"SceneManager.h"
#include"MapCreateScene.h"
#include"Field.h"
#include"PlayerAttackState.h"
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
	AllObjectControl.emplace_back(CameraControl::GetIns());
	AllObjectControl.emplace_back(EnemyControl::GetIns());
	AllObjectControl.emplace_back(PlayerControl::GetIns());
	AllObjectControl.emplace_back(ChestControl::GetIns());
	AllObjectControl.emplace_back(HouseControl::GetIns());
	AllObjectControl.emplace_back(WoodControl::GetIns());
	AllObjectControl.emplace_back(FenceControl::GetIns());
	for (int i = 3; i < 14; i++)
	{
		//lightGroup->SetDirLightActive(i, true);
		lightGroup->SetCircleShadowActive(i, true);
	}

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
		for (auto obj : AllObjectControl)
		{
			obj->Update();
		}
	}
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

	if (!Load && !PlayGame)
	{
		Load = true;
	}

	//オブジェクトの更新処理
	objUpdate();

	//csv読み込み
	LoadParam();

	//postEffect->SetVignette_GB(PlayerControl::GetIns()->GetVignetteAlpha());
	if (Input::GetIns()->TriggerButton(Input::START))
	{
		SceneManager::GetIns()->SetScene(SceneManager::BOSS, sceneManager_);
	}
	LightUpdate();
	ChangeSceneJudg();
	GameOver::GetIns()->Update();
}

void PlayScene::LightUpdate()
{
	XMFLOAT3 ppos = PlayerControl::GetIns()->GetPlayer()->GetPosition();

	lightGroup->SetCircleShadowDir(3, XMVECTOR({circleShadowDir[0], circleShadowDir[1], circleShadowDir[2], 0}));
	lightGroup->SetCircleShadowCasterPos(3, {ppos.x, ppos.y + 5.f, ppos.z});
	lightGroup->SetCircleShadowAtten(3, XMFLOAT3(0, 0, 0));
	lightGroup->SetCircleShadowFactorAngle(3, XMFLOAT2(circleShadowFactorAngle));

	for (int i = 0; i < 10; i++)
	{
		if (EnemyControl::GetIns()->GetEnemy(EnemyControl::PLAYSCENE)[i] == nullptr||
			Collision::GetLength(ppos, EnemyControl::GetIns()->GetEnemy(EnemyControl::PLAYSCENE)[i]->GetPosition())>200.f)
		{
			//lightGroup->SetCircleShadowFactorAngle(i + 4, {0, 0});
			lightGroup->SetCircleShadowActive(i+4, false);
			//continue;
		}
		else
		{
			lightGroup->SetCircleShadowActive(i + 4, true);
		}
		lightGroup->SetCircleShadowDir(i + 4, XMVECTOR({
			                               circleShadowDir[0], circleShadowDir[1], circleShadowDir[2], 0
		                               }));
		lightGroup->SetCircleShadowCasterPos(i + 4, {
			                                     EnemyControl::GetIns()->GetEnemy(EnemyControl::PLAYSCENE)[i]->
			                                     GetPosition().x,
			                                     EnemyControl::GetIns()->GetEnemy(EnemyControl::PLAYSCENE)[i]->
			                                     GetPosition().y + 15.f,
			                                     EnemyControl::GetIns()->GetEnemy(EnemyControl::PLAYSCENE)[i]->
			                                     GetPosition().z
		                                     });
		lightGroup->SetCircleShadowAtten(i + 4, XMFLOAT3(0, 0, 0));
		lightGroup->SetCircleShadowFactorAngle(i + 4, XMFLOAT2(circleShadowFactorAngle2));
	}
}

void PlayScene::ChangeSceneJudg()
{
	constexpr XMFLOAT3 ChangeScenePos = {60.f, -35, 400.f};

	if (Collision::GetLength(PlayerControl::GetIns()->GetPlayer()->GetPosition(), ChangeScenePos) < 30.f)
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

/*------------------------*/
/*--------描画処理--------*/
/*-----------------------*/
void PlayScene::MyGameDraw()
{
	if (Field::GetIns() != nullptr)
	{
		Field::GetIns()->Draw();
		for (auto obj : AllObjectControl)
		{
			obj->Draw();
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
		MyGameDraw(); //postEffect->Draw();
		SpriteDraw();
		ImGui::Begin("lightnu");
		ImGui::Text("%d", EnemyControl::GetIns()->GetQuentity());
		ImGui::End();
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
		for (auto obj : AllObjectControl)
		{
			obj->Initialize();
		}

		HUD::GetIns()->playerini();
		PlayerControl::GetIns()->GetPlayer()->SetHP(PlayerControl::GetIns()->GetPlayer()->GetMaxHP());
		//カメラをセット
		f_Object3d::SetCamera(CameraControl::GetIns()->GetCamera());
		//グラフィックパイプライン生成
		f_Object3d::CreateGraphicsPipeline();


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
	for (auto obj : AllObjectControl)
	{
		obj->Finalize();
	}
	AllObjectControl.clear();
	Field::GetIns()->Finalize();
}
