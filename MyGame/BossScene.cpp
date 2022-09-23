#include "BossScene.h"
#include"Input.h"
#include"TitleScene.h"
#include"SceneManager.h"
#include"Field.h"
#include"PlayerAttackState.h"
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
#include"AttackCollision.h"
#include"SelectSword.h"
#include"KnockAttack.h"
#include"CircleAttack.h"
#include"HalfAttack.h"
#include"AltAttack.h"
BossScene::BossScene(SceneManager* sceneManager)
	:BaseScene(sceneManager)
{

}

void BossScene::Initialize()
{
	//各オブジェクトの初期化
	if (AllObjectControl.size() == 0) {//各オブジェクトインスタンスぶちこむ
		AllObjectControl.push_back(CameraControl::GetInstance());
		AllObjectControl.push_back(PlayerControl::GetInstance());
		AllObjectControl.push_back(EnemyControl::GetInstance());
	}
	for (int i = 0; i < AllObjectControl.size(); i++) {
		AllObjectControl[i]->Initialize(CameraControl::GetInstance()->GetCamera());
	}

	Field::GetInstance()->Initialize(CameraControl::GetInstance()->GetCamera());
	
	//ボス攻撃用->できれば移す
	KnockAttack::GetInstance()->Initialize();
	CircleAttack::GetInstance()->Initialize();
	HalfAttack::GetInstance()->Initialize();
	Nail::GetInstance()->ModelSet();
	AltAttack::GetInstance()->Initialize();
	// 3Dオブジェクトにカメラをセット
	Object3d::SetCamera(CameraControl::GetInstance()->GetCamera());
	//カメラをセット
	f_Object3d::SetCamera(CameraControl::GetInstance()->GetCamera());
	//グラフィックパイプライン生成
	f_Object3d::CreateGraphicsPipeline();
	
	postEffect = new PostEffect();
	postEffect->Initialize();

	//カメラ挙動をボスカットシーン
	CameraControl::GetInstance()->SetCameraState(CameraControl::BOSSCUTSCENE);

}

void BossScene::Update()
{
	//読み込み
	if (!LoadEnemy&&!Play) {
		LoadEnemy = true;
	}

	SistemConfig::GetInstance()->Update();

	if (Play) {//csvからの読み込み終わってから更新処理
		//1->Player  0->Camera カメラの注視点Playerに合わすのでPlayerが先
		AllObjectControl[1]->Update(CameraControl::GetInstance()->GetCamera());
		AllObjectControl[0]->Update(CameraControl::GetInstance()->GetCamera());
		for (int i = 2; i < AllObjectControl.size(); i++) {
			AllObjectControl[i]->Update(CameraControl::GetInstance()->GetCamera());
		}
		Nail::GetInstance()->Update();
		UI::GetInstance()->HUDUpdate(hudload, CameraControl::GetInstance()->GetCamera());
		}
	
	Field::GetInstance()->Update(CameraControl::GetInstance()->GetCamera());
	//各オブジェクトの更新処理
	//csv読み込み部分(Cameraの更新後にするのでobjUpdate()挟んでから)
	LoadParam(CameraControl::GetInstance()->GetCamera());

	if (scenechange) {
		Feed::GetInstance()->Update_White(Feed::FEEDIN);//白くなります
	}
	AltAttack::GetInstance()->ActionJudg();
	if (SistemConfig::GetInstance()->GetConfigJudgMent()) {
		c_postEffect = Blur;
	} else {
		c_postEffect = Default;
	}
}

void BossScene::MyGameDraw()
{
	Field::GetInstance()->Draw();
	if (Play) {
		for (int i = 0; i < AllObjectControl.size(); i++) {
			AllObjectControl[i]->Draw();
		}
	}
	AltAttack::GetInstance()->Draw();
}
void BossScene::Draw()
{
	//ポストエフェクトの場合わけ(Bでぼかし Dがデフォルト)
	switch (c_postEffect)
	{
	case Blur://ぼかし　描画準違うだけ
		postEffect->PreDrawScene();
		MyGameDraw();
		postEffect->PostDrawScene();

		DirectXCommon::GetInstance()->BeginDraw();
		postEffect->Draw();
		//UI
		if (HUD::GetInstance()->GetLayOutMode()) {
			UI::GetInstance()->HUDDraw();
		}
		//使用剣選択画面
		SelectSword::GetInstance()->Draw();
		//設定画面
		SistemConfig::GetInstance()->Draw();
		if (DirectXCommon::GetInstance()->GetFullScreen() == false) {
			ImGuiDraw();
		}
		DirectXCommon::GetInstance()->EndDraw();
		break;

	case Default://普通のやつ特に何もかかっていない
		postEffect->PreDrawScene();
		postEffect->Draw();
		postEffect->PostDrawScene();

		DirectXCommon::GetInstance()->BeginDraw();
		MyGameDraw();
		//ボスの攻撃に使うテクスチャなどの描画->できれば他に移す
		CircleAttack::GetInstance()->Draw();
		HalfAttack::GetInstance()->Draw();
		KnockAttack::GetInstance()->Draw();
		//UI
		if (CameraControl::GetInstance()->GetCameraState() != CameraControl::BOSSCUTSCENE) {
			UI::GetInstance()->HUDDraw();
		}
		Feed::GetInstance()->Draw();
		Field::GetInstance()->WarningDraw();
		SistemConfig::GetInstance()->Draw();

		if (DirectXCommon::GetInstance()->GetFullScreen() == false) {
			PlayerControl::GetInstance()->GetPlayer()->ImguiDraw();
			ImGuiDraw();
		}
		DirectXCommon::GetInstance()->EndDraw();
		break;
	}
}

bool BossScene::LoadParam(DebugCamera* camera)
{
	if (LoadEnemy) {
		for (int i = 0; i < AllObjectControl.size(); i++) {
			AllObjectControl[i]->Load(CameraControl::GetInstance()->GetCamera());
		}
		hudload = true;
		Play = true;
		LoadEnemy = false;
	}
	return true;
}

void BossScene::ImGuiDraw()
{

}

void BossScene::Finalize()
{
	UI::GetInstance()->Finalize();
	SistemConfig::GetInstance()->Finalize();
	AttackCollision::GetInstance()->Finalize();
	AllObjectControl.clear();
	Field::GetInstance()->Finalize();
}