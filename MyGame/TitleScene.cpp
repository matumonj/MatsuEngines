#include "TitleScene.h"
#include"Input.h"
#include"PlayScene.h"
#include"MapCreateScene.h"
#include"SceneManager.h"
#include"Tutorial.h"
#include"Feed.h"
#include"BossScene.h"
#include"UI.h"

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

	Sprite::LoadTexture(30, L"Resources/2d/title/titlesp.png");
	titlesprite = Sprite::Create(30, {0.0f, 0.0f});

	Sprite::LoadTexture(31, L"Resources/title2.png");
	titlesprite2 = Sprite::Create(31, {0, 0.0f});

	Sprite::LoadTexture(32, L"Resources/2d/title/gameplay.png");
	Sprite::LoadTexture(33, L"Resources/2d/title/edit.png");

	Sprite* navGameSprite = Sprite::Create(32, {0, 0.0f});
	Sprite* navEditSprite = Sprite::Create(33, {0, 0.0f});

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
	Feed::GetInstance()->initialize();

	camera = new DebugCamera(WinApp::window_width, WinApp::window_height);

	field = std::make_unique<Object3d>();
	field->SetModel(ModelManager::GetIns()->GetModel(ModelManager::FIELD));
	field->Initialize(camera);
	celestal = std::make_unique<Object3d>();
	celestal->SetModel(ModelManager::GetIns()->GetModel(ModelManager::CELESTIALSPHERE));
	celestal->Initialize(camera);

	Feed::GetInstance()->initialize();
}

/*------------------------*/
/*-------更新処理--------*/
/*-----------------------*/
#include"mHelper.h"
#include"Feed.h"

void TitleScene::Update()
{
	FieldRotY += 0.1f;
	field->SetRotation({0.0f, 0.0f, 0.0f});
	field->SetScale({0.15f, 0.15f, 0.15f});
	field->SetFogCenter(FogPos);
	field->setFog(TRUE);
	celestal->SetRotation({0.0f, 0.0f, 0.0f});
	celestal->SetScale({30.f, 30.1f, 30.1f});
	if (Input::GetInstance()->TriggerButton(Input::B))
	{
		SceneManager::GetInstance()->SetScene(SceneManager::TUTORIAL, sceneManager_);

		//押されたら
		menujudg_Play = true;
		//押されたら
		feedf = true;
	}
	if (Input::GetInstance()->TriggerButton(Input::A))
	{
		//押されたら
		SceneManager::GetInstance()->SetScene(SceneManager::MAPCREATE, sceneManager_);
	}
	if (feedf)
	{
		Cangle += 0.5f;
		float cameratocenter_x = sqrtf((CameraPos.x - 16.0f) * (CameraPos.x - 16.0f));
		if (cameratocenter_x < 1.0f && CameraPos.y < -30.0f)
		{
			BackCam = true;
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


	if (Feed::GetInstance()->GetAlpha() >= 1.0f)
	{
		SceneManager::GetInstance()->SetScene(SceneManager::TUTORIAL, sceneManager_);
	}
	//titlesprite2->SetRotation(180);165
	if (Cangle >= 360.0f)
	{
		Cangle = 0.0f;
	}

	if (!BackCam)
	{
		CameraPos.x = sinf(Cangle * (PI / 180.0f)) * 40.0f;
		CameraPos.y = cosf(Cangle * (PI / 180.0f)) * 40.0f;
	}
	else
	{
		CameraPos.y--;
	}

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
	camera->SetEye({CameraPos.x, 2.0f, CameraPos.y});
	camera->SetTarget({0.0f, 0.0f, 0.0f});
	camera->Update();
	titlesprite->SetSize({WinApp::window_width / 2, WinApp::window_height});
	titlesprite2->SetSize({WinApp::window_width, WinApp::window_height});
	field->Update({0.6f, 0.6f, 0.6f, 1.0f}, camera);
	celestal->Update({0.6f, 0.6f, 0.6f, 1.0f}, camera);
	//	DebugTextSprite::GetInstance()->Print("aa", 0, 0, 2);
}

/*------------------------*/
/*-------描画処理--------*/
/*-----------------------*/
void TitleScene::SpriteDraw()
{
	Object3d::PreDraw();
	celestal->Draw();
	field->Draw();
	Object3d::PostDraw();

	titlesprite->setcolor({1.0f, 1.0f, 1.0f, 0.5f});
	titlesprite2->setcolor({1.0f, 1.0f, 1.0f, 0.5f});
	Sprite::PreDraw();
	titlesprite2->Draw();
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
	delete camera;
	delete titlesprite, titlesprite2;
}
