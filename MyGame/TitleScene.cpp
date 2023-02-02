#include "TitleScene.h"

#include <tchar.h>

#include"Input.h"
#include"PlayScene.h"
#include"MapCreateScene.h"
#include"SceneManager.h"
#include"Feed.h"
#include "ImageManager.h"

TitleScene::TitleScene(SceneManager* sceneManager)
	: BaseScene(sceneManager)
{
}

/*------------------------*/
/*-------初期化処理--------*/
/*-----------------------*/
void TitleScene::Initialize()
{
	// ライト生成
	lightGroup = LightGroup::Create();
	// 3Dオブエクトにライトをセット
	Object3d::SetLightGroup(lightGroup);

	Feed::GetInstance()->initialize();

	//フィールド
	TitleFieldInit();
	//タイトルスプライト
	TitleTexInit();
	
	}

/*------------------------*/
/*-------更新処理--------*/
/*-----------------------*/
#include"mHelper.h"

void TitleScene::Update()
{
	if (Input::GetInstance()->TriggerButton(Input::B))
	{
		//SceneManager::GetInstance()->SetScene(SceneManager::TUTORIAL, sceneManager_);

		//押されたら
		menujudg_Play = true;
		//押されたら
		feedf = true;
	}
	if (Input::GetInstance()->TriggerButton(Input::A))
	{
		SceneManager::GetInstance()->SetScene(SceneManager::MAPCREATE, sceneManager_);
	}


	//画面真っ白になったらシーン切り替え
	//
	if (ChangeScene() == true)
	{
		SceneManager::GetInstance()->SetScene(SceneManager::TUTORIAL,sceneManager_);
	}

	//360言ったら０にリセット
	if (Cangle >= 360.0f)
	{
		Cangle = 0.0f;
	}

	//カメラがフィールド中心に回るように
	if (!CameraBackF)
	{
		CameraPos.x = sinf(Cangle * (PI / 180.0f)) * 40.0f;
		CameraPos.y = cosf(Cangle * (PI / 180.0f)) * 40.0f;
	}
	else
	{
		//カメラのｚ座標を引く
		CameraPos.y--;
	}

	//スプライト
	TitleTexUpda();
	//フィールド
	TitleFieldUpda();

	//カメラ更新(後で移す)
	camera->SetEye({CameraPos.x, 2.0f, CameraPos.y});
	camera->SetTarget({0.0f, 0.0f, 0.0f});
	camera->Update();
	//SceneManager::GetInstance()->SetScene(SceneManager::TUTORIAL, sceneManager_);

}



void TitleScene::MyGameDraw()
{
	//モデル描画
	Object3d::PreDraw();
	celestal->Draw();
	field->Draw();
	Object3d::PostDraw();

}
bool TitleScene::ChangeScene()
{
	CameraBackF = true;
	if (feedf)
	{
		Cangle += 0.5f;
		float cameratocenter_x = sqrtf((CameraPos.x -26.0f) * (CameraPos.x -26.0f));
		if (cameratocenter_x < 1.0f && CameraPos.y < -30.0f)
		{
			CameraBackF = true;
		}
	}
	else
	{
		Cangle += 0.1f;
	}
	if (CameraPos.y <= -40.0f)
	{
		Feed::GetInstance()->Update_White(Feed::FEEDIN);
	}

	//画面真っ白になったらシーン切り替え
	if (Feed::GetInstance()->GetAlpha() >= 1.0f)
	{
		return true;
	}
	return false;
}

/*------------------------*/
/*-------描画処理--------*/
/*-----------------------*/
void TitleScene::SpriteDraw()
{
	
	//スプライト描画
	titlesprite->setcolor({1.0f, 1.0f, 1.0f, 0.5f});

	Sprite::PreDraw();
	titlesprite->Draw();
	TitleMenu[0]->Draw();
	DebugTextSprite::GetInstance()->DrawAll();
	Sprite::PostDraw();

	Feed::GetInstance()->Draw();
}

void TitleScene::Draw()
{
	//ポストエフェクトの描画
	DirectXCommon::GetInstance()->BeginDraw(); //描画コマンドの上らへんに
	MyGameDraw();
	SpriteDraw();
	DirectXCommon::GetInstance()->EndDraw();
}


/*------------------------*/ //17,125
/*-------解放処理--------*/
/*-----------------------*/
void TitleScene::Finalize()
{
	field.release();
	celestal.release();
	//delete postEffect;
	delete lightGroup;
	Destroy_unique(camera);
	Destroy_unique(titlesprite);
	//Destroy_unique(titlesprite);
}

void TitleScene::TitleTexInit()
{
	titlesprite.reset(Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::TITLE1), {0.0f, 0.0f}));

	Sprite* navGameSprite = Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::GAMEPLAY), {0, 0.0f});
	Sprite* navEditSprite = Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::GAMEPLAY), {0, 0.0f});

	TitleMenu[0].reset(navGameSprite);
	TitleMenu[1].reset(navEditSprite);

	for (int i = 0; i < 2; i++)
	{
		menuAlpha[i] = 1.0f;
		MenuScale[i] = {1900, 1000};
		TitleMenu[i]->SetPosition({950, 500});
		TitleMenu[i]->SetSize({1900, 1000});
		TitleMenu[i]->SetAnchorPoint({0.5, 0.5});
	}
}

void TitleScene::TitleTexUpda()
{
	TitleMenu[0]->SetSize(MenuScale[0]);
	TitleMenu[1]->SetSize(MenuScale[1]);

	TitleMenu[0]->setcolor({1.0f, 1.0f, 1.0f, menuAlpha[0]});
	TitleMenu[1]->setcolor({1.0f, 1.0f, 1.0f, menuAlpha[1]});


	if (menujudg_Play)
	{
		MenuScale[0].x += 20.0f;
		MenuScale[0].y += 20.0f;
		menuAlpha[0] -= 0.02f;
		menuAlpha[1] = 0.0f;
	}
	if (menujudg_Edit)
	{
		MenuScale[1].x += 20.0f;
		MenuScale[1].y += 20.0f;
		menuAlpha[1] -= 0.02f;
		menuAlpha[0] = 0.0f;
	}
	titlesprite->SetSize({WinApp::window_width / 2, WinApp::window_height});
}

void TitleScene::TitleTexDraw()
{
}

void TitleScene::TitleFieldInit()
{
	camera = std::make_unique<DebugCamera>(WinApp::window_width, WinApp::window_height);

	//地形
	field = std::make_unique<Object3d>();
	field->SetModel(ModelManager::GetIns()->GetModel(ModelManager::FIELD));
	field->Initialize(camera.get());
	//天球
	celestal = std::make_unique<Object3d>();
	celestal->SetModel(ModelManager::GetIns()->GetModel(ModelManager::SKY));
	celestal->Initialize(camera.get());
}

void TitleScene::TitleFieldUpda()
{
	//フィールドくるくる回す
	FieldRotY += 0.1f;

	//パラメータをセット(地形)
	field->SetRotation({0.0f,FieldRotY, 0.0f});
	field->SetScale({0.15f, 0.15f, 0.15f});
	
	//パラメータをセット(天球)
	celestal->SetRotation({0.0f, 0.0f, 0.0f});
	celestal->SetScale({30.f, 30.1f, 30.1f});

	//更新処理
	field->setFog(true);
	field->Update({0.6f, 0.6f, 0.6f, 1.0f}, camera.get());
	celestal->Update({0.6f, 0.6f, 0.6f, 1.0f}, camera.get());
}

void TitleScene::LightUpdate()
{

}