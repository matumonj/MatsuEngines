#include "BossScene.h"
#include"TitleScene.h"
#include"SceneManager.h"
#include"Field.h"
#include"SistemConfig.h"
#include"EnemyControl.h"
#include"WoodControl.h"
#include"CameraControl.h"
#include"UI.h"
#include"PlayScene.h"
#include"Feed.h"
#include"PlayerControl.h"
#include "GameOver.h"
#include <BossMap.h>
#include"AltAttack.h"
#include "FrontCircleAttack.h"
#include "HalfAttack.h"
#include"KnockAttack.h"
#include "Nail.h"
#include"UltAttack.h"
#include "BronzeAttack.h"

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
	
	//各オブジェクトインスタンスぶちこむ
	AllObjectControl.emplace_back(CameraControl::GetInstance());
	AllObjectControl.emplace_back(PlayerControl::GetInstance());
	AllObjectControl.emplace_back(EnemyControl::GetInstance());
	
	lightGroup = LightGroup::Create();

	Object3d::SetLightGroup(lightGroup);
	//lightGroup2 = LightGroup::Create();
	// 3Dオブエクトにライトをセット
	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightActive(1, true);
	lightGroup->SetCircleShadowActive(0, true);
	lightGroup->SetCircleShadowActive(1, true);
	
	//ボス攻撃用->できれば移す
	Nail::GetInstance()->ModelSet();
	BossMap::GetInstance()->Init();
	
	postEffect = new PostEffect();
	postEffect->Initialize();
	input = Input::GetInstance();
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
	UltAttack::GetIns()->Upda();
	Field::GetInstance()->Update();
	//各オブジェクトの更新処理
	//csv読み込み部分(Cameraの更新後にするのでobjUpdate()挟んでから)
	LoadParam();

	lightGroup->Update();
	
	if (input->TriggerButton(input->RB))
	{
		c_postEffect = Blur;
	}
	else if(input->TriggerButton(input->LB))
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
		lightGroup->SetCircleShadowCasterPos(0,PlayerControl::GetInstance()->GetPlayer()->GetPosition());
		lightGroup->SetCircleShadowAtten(0, XMFLOAT3(circleShadowAtten));
		lightGroup->SetCircleShadowDir(0, XMVECTOR({ circleShadowDir[0], circleShadowDir[1], circleShadowDir[2], 0 }));
		lightGroup->SetCircleShadowFactorAngle(0, XMFLOAT2(circleShadowFactorAngle2));

		lightGroup->SetCircleShadowFactorAngle(1, XMFLOAT2(circleShadowFactorAngle2));
		XMFLOAT3 bpos = EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->GetPosition();
		lightGroup->SetCircleShadowCasterPos(1, { bpos.x,bpos.y,bpos.z });
		lightGroup->SetCircleShadowAtten(1, XMFLOAT3(circleShadowAtten));
		lightGroup->SetCircleShadowDir(1, XMVECTOR({ circleShadowDir[0], circleShadowDir[1], circleShadowDir[2], 0 }));
		
	BossMap::GetInstance()->Upda();
	AltAttack::GetInstance()->Upda();

	GameOver::GetIns()->Update();

	ChangeScene();
}


/*------------------------*/
/*--------描画処理--------*/
/*-----------------------*/

void BossScene::SpriteDraw()
{
	EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->DamageTexDisplay_Draw();

	for (int i = 0; i < 2; i++)
	{
		EnemyControl::GetInstance()->GetSummonEnemy(i)->DamageTexDisplay_Draw();
	}

	Field::GetInstance()->WarningDraw();
	PlayerControl::GetInstance()->GetPlayer()->ParticleDraw();

	GameOver::GetIns()->Draw_DestParticle();
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
	SistemConfig::GetInstance()->Draw();

}

void BossScene::MyGameDraw()
{
	if (!Play)return;

	if (Field::GetInstance() != nullptr)
	{
		Field::GetInstance()->Draw();
	}

		Nail::GetInstance()->Draw();
		BossMap::GetInstance()->Draw();

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
		//UI
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
	/*	if (Collision::GetLength(PlayerControl::GetInstance()->GetPlayer()->GetPosition(),ClearStagePos)<5.f)
		{
			feedend = true;
		}
		if(feedend)
		{
			Feed::GetInstance()->Update_White(Feed::FEEDIN);
			if (Feed::GetInstance()->GetAlpha() >= 1.0f) {
				SceneManager::GetInstance()->SetScene(SceneManager::GAMECLEAR, sceneManager_);
			}
		}
	}*/
}

void BossScene::LightUpdate()
{

}