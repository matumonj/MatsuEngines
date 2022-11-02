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
#include"PlayerControl.h"
#include"PlayScene.h"
#include"DamageManager.h"
#include"StoneControl.h"
#include"GrassFieldControl.h"
#include"SelectSword.h"
//シーンのコンストラクタ
Tutorial::Tutorial(SceneManager* sceneManager)
	:BaseScene(sceneManager)
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
	lightGroup->SetDirLightActive(2, true);
	lightGroup->SetPointLightActive(0, false);
	lightGroup->SetPointLightActive(1, false);
	lightGroup->SetPointLightActive(2, false);
	lightGroup->SetCircleShadowActive(0, true);
	lightGroup->SetCircleShadowActive(1, true);
	lightGroup->SetCircleShadowActive(2, true);
	Texture::LoadTexture(47, L"Resources/df.png");
	{
		AllObjectControl.push_back(CameraControl::GetInstance());//Camera
		AllObjectControl.push_back(EnemyControl::GetInstance());//Enemy
		AllObjectControl.push_back(PlayerControl::GetInstance());//Player
		AllObjectControl.push_back(FenceControl::GetInstance());//Fence
		AllObjectControl.push_back(ChestControl::GetInstance());//Chest
		AllObjectControl.push_back(WoodControl::GetInstance());//Wood
		AllObjectControl.push_back(StoneControl::GetInstance());//Wood
		AllObjectControl.push_back(GrassFieldControl::GetInstance());//Wood

	for (int i = 0; i < AllObjectControl.size(); i++) {//初期化
		AllObjectControl[i]->Initialize(CameraControl::GetInstance()->GetCamera());
		}
	}
	//Field
	
	//カメラをセット
	f_Object3d::SetCamera(CameraControl::GetInstance()->GetCamera());
	//グラフィックパイプライン生成
	f_Object3d::CreateGraphicsPipeline();

	//フェードとUIスプライト初期化
	Feed::GetInstance()->initialize();
	UI::GetInstance()->Initialize();

	postEffect = new MinimapSprite();
	postEffect->Initialize();
	//ミニマップ用のカメラ　後で別のところに移す
	dc = new DebugCamera(WinApp::window_width, WinApp::window_height);
	//各種設定画面
	SistemConfig::GetInstance()->Initialize();

		//カメラの注視点をプレイヤーにセット
		CameraControl::GetInstance()->SetCameraState(CameraControl::PLAYER);
		SelectSword::GetInstance()->Initialize();

}

/*------------------------*/
/*--------更新処理---------*/
/*------------------------*/
void Tutorial::objUpdate(DebugCamera* camera)
{
	if (!Load && !Play) {
		Load = true;
	}
	if (Play) {//csvからの読み込み終わってから更新処理
		AllObjectControl[1]->Update(CameraControl::GetInstance()->GetCamera());
		AllObjectControl[0]->Update(CameraControl::GetInstance()->GetCamera());
		for (int i = 2; i < AllObjectControl.size(); i++) {
			if (AllObjectControl[i] != nullptr) {
				AllObjectControl[i]->Update(CameraControl::GetInstance()->GetCamera());
			}
		}
		grassfield->Update(CameraControl::GetInstance()->GetCamera());
		UI::GetInstance()->HUDUpdate(hudload, CameraControl::GetInstance()->GetCamera());
	}
	//後で別ん所移す
	dc->Update();

	dc->SetTarget({ PlayerControl::GetInstance()->GetPlayer()->GetPosition() });
	dc->SetEye({ PlayerControl::GetInstance()->GetPlayer()->GetPosition().x,
		 PlayerControl::GetInstance()->GetPlayer()->GetPosition().y + 300.0f,
		PlayerControl::GetInstance()->GetPlayer()->GetPosition().z - 1 });
	Field::GetInstance()->SetCamera(dc);

	Field::GetInstance()->Update(CameraControl::GetInstance()->GetCamera());

	
}

void Tutorial::Update()
{
	lightGroup->Update();

	SistemConfig::GetInstance()->Update();
	//各オブジェクトの更新処理
	objUpdate(CameraControl::GetInstance()->GetCamera());//オブジェクトの更新処理
	//csv読み込み部分(Cameraの更新後にするのでobjUpdate()挟んでから)
	LoadParam(CameraControl::GetInstance()->GetCamera());

	//一定数進んだらシーンチェンジ
	bool ArrivalJudg = PlayerControl::GetInstance()->GetPlayer()->GetPosition().z > -470.0f;
	if (ArrivalJudg) {
		scenechange = true;
	}

	if (scenechange) {
		Feed::GetInstance()->Update_White(Feed::FEEDIN);//白くなります
	}
	if (SistemConfig::GetInstance()->GetConfigJudgMent()) {
		c_postEffect = Blur;
	} else {
		c_postEffect = Default;
	}
	//postEffect->SetSize({ 10,10 });
	XMFLOAT3 ppos = PlayerControl::GetInstance()->GetPlayer()->GetPosition();
	if (EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0] != nullptr) {
		lightGroup->SetCircleShadowDir(0, XMVECTOR({ circleShadowDir[0], circleShadowDir[1], circleShadowDir[2], 0 }));
		//for (int i = 0; i < 3; i++) {
		lightGroup->SetCircleShadowCasterPos(0, { EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0]->GetPosition() });
		//}
		//lightGroup->SetCircleShadowCasterPos(1, XMFLOAT3(PlayerControl::GetInstance()->GetPlayer()->GetPosition()));
		lightGroup->SetCircleShadowAtten(0, XMFLOAT3(circleShadowAtten));
		lightGroup->SetCircleShadowFactorAngle(0, XMFLOAT2(circleShadowFactorAngle2));
	}
	lightGroup->SetCircleShadowDir(1, XMVECTOR({ circleShadowDir[0], circleShadowDir[1], circleShadowDir[2], 0 }));
	//for (int i = 0; i < 3; i++) {
	lightGroup->SetCircleShadowCasterPos(1, { ppos });
	//}
	//lightGroup->SetCircleShadowCasterPos(1, XMFLOAT3(PlayerControl::GetInstance()->GetPlayer()->GetPosition()));
	lightGroup->SetCircleShadowAtten(1, XMFLOAT3(circleShadowAtten));
	lightGroup->SetCircleShadowFactorAngle(1, XMFLOAT2(circleShadowFactorAngle));

	postEffect->SetCenterpos(HUD::GetInstance()->GetMinimapSprite()->GetPosition());
if (scenechange&& Feed::GetInstance()->GetAlpha() >= 1.0f) {//画面真っ白なったら
		BaseScene* scene = new PlayScene(sceneManager_);//次のシーンのインスタンス生成
		Play = false;
		SceneManager::GetInstance()->SetScene(SceneManager::PLAY);
		sceneManager_->SetnextScene(scene);//シーンのセット
	}
	DamageManager::GetIns()->Upda();

}

/*------------------------*/
/*--------描画処理---------*/
/*----------obj----------*/
void Tutorial::MyGameDraw()
{
	if (Play) {
		for (int i = 0; i < AllObjectControl.size(); i++) {
			if (AllObjectControl[i] != nullptr) {
				AllObjectControl[i]->Draw();
			}
		}
		grassfield->Draw();
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
	case Blur://ぼかし　描画準違うだけ
		postEffect->PreDrawScene();
		MyGameDraw();
		postEffect->PostDrawScene();

		DirectXCommon::GetInstance()->BeginDraw();
		//postEffect->Draw();
		SistemConfig::GetInstance()->SwordPedestalDraw();
		
		SistemConfig::GetInstance()->Draw();
		DirectXCommon::GetInstance()->EndDraw();

		break;

	case Default://普通のやつ特に何もかかっていない
		
		postEffect->PreDrawScene();
		Field::GetInstance()->MiniFieldDraw();
		postEffect->PostDrawScene();

		DirectXCommon::GetInstance()->BeginDraw();
		Field::GetInstance()->Draw();
		MyGameDraw();
		postEffect->Draw();
		DamageManager::GetIns()->Draw();
		PlayerControl::GetInstance()->DamageTexDraw();
		UI::GetInstance()->HUDDraw();
		Feed::GetInstance()->Draw();
		SistemConfig::GetInstance()->Draw();
		ImGui::Begin("WW");
		ImGui::InputFloat3("circleShadowDir", circleShadowDir);
		//ImGui::InputFloat3("circleShadowPos", circleShadowPos);
		ImGui::InputFloat3("circleShadowAtten", circleShadowAtten, 8);
		ImGui::InputFloat2("circleShadowFactorAngle", circleShadowFactorAngle);
		ImGui::InputFloat3("fighterPos", fighterPos);
		ImGui::End();
		DirectXCommon::GetInstance()->EndDraw();

		break;
	}
}

/*------------------------*/
/*--------読込処理--------*/
/*-----------------------*/
bool Tutorial::LoadParam(DebugCamera* camera)
{
	if (Load) {
		for (int i = 0; i < AllObjectControl.size(); i++) {
			AllObjectControl[i]->Load(CameraControl::GetInstance()->GetCamera());
		}
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
	for (int i = 0; i < AllObjectControl.size(); i++) {//初期化
		AllObjectControl[i]->Finalize();
	}
	AllObjectControl.clear();
	Field::GetInstance()->Finalize();
	Destroy(postEffect);
	Destroy(dc);
}
