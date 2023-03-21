#include "TitleScene.h"
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

	Feed::GetIns()->initialize();
	
	LoadWords.emplace("リ", Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::LOADWORD_1),{ 0, 0.0f }));
	LoadWords.emplace("ソ", Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::LOADWORD_2), { 0, 0.0f }));
	LoadWords.emplace("ー", Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::LOADWORD_3), { 0, 0.0f }));
	LoadWords.emplace("ス", Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::LOADWORD_4), { 0, 0.0f }));
	LoadWords.emplace("読", Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::LOADWORD_5), { 0, 0.0f }));
	LoadWords.emplace("み", Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::LOADWORD_6), { 0, 0.0f }));
	LoadWords.emplace("込", Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::LOADWORD_7), { 0, 0.0f }));
	LoadWords.emplace("中", Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::LOADWORD_8), { 0, 0.0f }));
	LoadWords.emplace("完", Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::LOADWORD_9), { 0, 0.0f }));
	LoadWords.emplace("了", Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::LOADWORD_10), { 0, 0.0f }));


	postEffect = new PostEffect();
	postEffect->Initialize();
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
	if (Input::GetIns()->TriggerButton(Input::B))
	{
		SceneManager::GetIns()->SetLoad(true);
		CameraBackF = true;
		menujudg_Play = true;
		//feedf = true;
	}
	if (Input::GetIns()->TriggerButton(Input::A))
	{
		SceneManager::GetIns()->SetScene(SceneManager::MAPCREATE, sceneManager_);
	}

	
	//画面真っ白になったらシーン切り替え
	//
	if (CameraBackF)
	{
		if (SceneManager::GetIns()->GetLoad()) {
			DebugTextSprite::GetIns()->Print("NowLoading", 1000, 600, 3);
		}
		else
		{
			DebugTextSprite::GetIns()->Print("Clear", 1000, 600, 3);
		}
		timef++;
		SceneManager::GetIns()->LoadScene();
	}
	if (ChangeScene() == true)
	{
		SceneManager::GetIns()->SetScene(SceneManager::TUTORIAL, sceneManager_);
	}
	if(!FadeFlag)
	{
		if (feedf)
		{
			postEffect->SetSepiaF(true);
			Feed::GetIns()->Update_Black(Feed::FEEDOUT);
		} else
		{
			if (menujudg_Play) {
				Feed::GetIns()->Update_Black(Feed::FEEDIN);
				if (Feed::GetIns()->GetAlpha() >= 1.0f)
				{
					feedf = true;
				}
			}
		}
	}

	//360言ったら０にリセット
	if (Cangle >= PI_360)
	{
		Cangle = 0.0f;
	}

	//カメラがフィールド中心に回るように
	if (!CameraBackF)
	{
		CameraPos.x = sinf(Cangle * (PI / PI_180)) * 40.0f;
		CameraPos.y = cosf(Cangle * (PI / PI_180)) * 40.0f;
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
	
	FadeFlag = timef&&!SceneManager::GetIns()->GetLoad();
	float CameraCenterPosX = CameraPos.x - 26.0f;
	if (FadeFlag)
	{
		CamAngleSpeed = 0.5f;

		float cameratocenter_x = sqrtf(CameraCenterPosX * CameraCenterPosX);

		if (cameratocenter_x < 1.0f && CameraPos.y < -30.0f)
		{
			CameraBackF = true;
		}
	}
	else
	{
		CamAngleSpeed = 0.1f;
	}

	//回転用アングルの値を足していく
	Cangle += CamAngleSpeed;

	//シーン遷移時
	if (FadeFlag)
	{
		Feed::GetIns()->Update_White(Feed::FEEDIN);

		//画面真っ白になったらシーン切り替え
		if (Feed::GetIns()->GetAlpha() >= 1.0f)
		{
			return true;
		}
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
	TitleMenu->Draw();

	//1文字ずつ読み込む
	for (auto i = LoadWords.begin(); i != LoadWords.end(); i++) {
		LoadWords[i->first].get()->Draw();
	}

	if (feedf)
	LoadMenuSprite->Draw();
	DebugTextSprite::GetIns()->DrawAll();

	Sprite::PostDraw();

	Feed::GetIns()->Draw();
}

void TitleScene::Draw()
{
	
	//ポストエフェクトの描画
	DirectXCommon::GetIns()->BeginDraw(); //描画コマンドの上らへんに
	//postEffect->Draw();
	MyGameDraw();
	SpriteDraw();
	DirectXCommon::GetIns()->EndDraw();
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
	Sprite* loadSprite= Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::LOAD), { 0, 0.0f });
	TitleMenu.reset(navGameSprite);
	LoadMenuSprite.reset(loadSprite);
	menuAlpha = 1.0f;

	MenuScale = {1900.f, 1000.f};
	MenuPos = {950.f, 500.f};

	TitleMenu->SetPosition(MenuPos);
	TitleMenu->SetAnchorPoint({0.5f, 0.5f});
}

void TitleScene::TitleTexUpda()
{
	TitleMenu->SetSize(MenuScale);
	TitleMenu->setcolor({1.0f, 1.0f, 1.0f, menuAlpha});

	constexpr float l_MenuSclingSpeed = 20.f;
	constexpr float l_MenuSubAlphaSpeed = 0.02f;

	if (menujudg_Play)
	{
		MenuScale.x += l_MenuSclingSpeed;
		MenuScale.y += l_MenuSclingSpeed;
		menuAlpha -= l_MenuSubAlphaSpeed;
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
	constexpr XMFLOAT3 FieldScl = {0.15f, 0.15f, 0.15f};
	constexpr XMFLOAT3 CelestalScl = {30.f, 30.1f, 30.1f};

	constexpr float addRotYSpeed = 0.1f;

	//フィールドくるくる回す
	FieldRotY += addRotYSpeed;

	//パラメータをセット(地形)
	field->SetRotation({0.0f, FieldRotY, 0.0f});
	//パラメータをセット(天球)
	celestal->SetRotation({0.0f, 0.0f, 0.0f});
	celestal->SetScale(CelestalScl);

	//更新処理
	field->setFog(true);
	celestal->Setf(true);
	celestal->setFog(true);
	celestal->SetFogCenter(camera->GetEye());

	field->Update(camera.get());
	celestal->Update(camera.get());
}

void TitleScene::LightUpdate()
{
}
