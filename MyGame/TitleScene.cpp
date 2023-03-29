#include "TitleScene.h"

#include <algorithm>

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
	LoadWords.emplace("み_1", Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::LOADWORD_6), { 0, 0.0f }));
	LoadWords.emplace("込", Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::LOADWORD_7), { 0, 0.0f }));
	LoadWords.emplace("み_2", Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::LOADWORD_6), { 0, 0.0f }));
	LoadWords.emplace("中", Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::LOADWORD_8), { 0, 0.0f }));
	LoadWords.emplace("完", Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::LOADWORD_9), { 0, 0.0f }));
	LoadWords.emplace("了", Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::LOADWORD_10), { 0, 0.0f }));


	WordsSize = { 220.f };

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
	}
	if (Input::GetIns()->TriggerButton(Input::A))
	{
		SceneManager::GetIns()->SetScene(SceneManager::MAPCREATE, sceneManager_);
	}

	
	//画面真っ白になったらシーン切り替え
	//
	if (CameraBackF)
	{
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
	LoadWordsSetParam();

	TitleTexUpda();
	//フィールド
	TitleFieldUpda();
	//カメラ更新(後で移す)
	LoadMenuSprite->SetSize({ 1900.f,1000.f });
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
		CamAngleSpeed = 0.1f;
	

	//回転用アングルの値を足していく
	Cangle += CamAngleSpeed;

	//シーン遷移時
	if (LoadWordsChange&&timef>=600.f)
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

	

	if (feedf) {
		LoadMenuSprite->Draw();
		//1文字ずつ読み込む
		for (auto i = LoadWords.begin(); i != LoadWords.end(); i++) {
			if (LoadWords[i->first] == nullptr)continue;
			LoadWords[i->first].get()->Draw();
		}
	//	DebugTextSprite::GetIns()->DrawAll();
	}
	
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
	LoadWords["読"].reset();
	LoadWords["み_1"].reset();
	LoadWords["込"].reset();
	LoadWords["み_2"].reset();
	LoadWords["完"].reset();
	LoadWords["了"].reset();

	delete postEffect;
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
	celestal->SetColor({ 1.f,1.f,1.f,1.f });
	//更新処理
	field->setFog(true);
	celestal->SetFogCenter(camera->GetEye());

	field->Update(camera.get());
	celestal->Update(camera.get());
}

void TitleScene::LightUpdate()
{
}


void TitleScene::LoadWordsSetParam()
{
	//上下運動の中心
	constexpr XMFLOAT2 CenterPos = {200.f,800.f};
	//文字の間隔
	constexpr float WordsInter = 100.f;

	constexpr float AddMovingVal = 2.2f;

	//最初の文字だけずっと動かす
	LoadWordsPosAngle[0] += 1.f;
	

	if(!LoadWordsChange){
		for (int i = 0; i < LoadWordsPosAngle.size(); i++)
		{
			if (i != 0 && LoadWordsPosAngle[i - 1] > 10.f)
			{
				LoadWordsPosAngle[i] += 1.f;
			}
			LoadWordsPos[i].x = CenterPos.x + static_cast<float>(i) * WordsInter;
			LoadWordsPos[i].y = CenterPos.y + sinf(PI * 2.f / 120.f * LoadWordsPosAngle[i]) * WordsInter / 2.f;
		}
	}
	else
	{
		for (int i = 0; i < LoadWordsPosAngle.size(); i++)
		{
			LoadWordsPos[i].x = CenterPos.x + static_cast<float>(i) * WordsInter;
			LoadWordsPos[i].y = CenterPos.y;
		}
	}
	//文字のサイズ設定
	for (auto i = LoadWords.begin(); i != LoadWords.end(); i++) {
		if (LoadWords[i->first] == nullptr)continue;
		LoadWords[i->first].get()->SetAnchorPoint({ 0.5f,0.5f });
		LoadWords[i->first].get()->SetSize({ WordsSize,WordsSize });
	}


	//わかりやすくするために個々で書いてる
	if (LoadWordsChange) {
		LoadWords["読"].get()->SetPosition(LoadWordsPos[0]);
		LoadWords["み_1"].get()->SetPosition(LoadWordsPos[1]);
		LoadWords["込"].get()->SetPosition(LoadWordsPos[2]);
		LoadWords["み_2"].get()->SetPosition(LoadWordsPos[3]);
		LoadWords["完"].get()->SetPosition(LoadWordsPos[4]);
		LoadWords["了"].get()->SetPosition(LoadWordsPos[5]);

		//要らなくなったテクスチャは破棄
		LoadWords["リ"].reset(nullptr);
		LoadWords["ソ"].reset(nullptr);
		LoadWords["ー"].reset(nullptr);
		LoadWords["ス"].reset(nullptr);
		LoadWords["中"].reset(nullptr);

		WordsSize += AddMovingVal*2.f;
	}
	else {
		LoadWords["リ"] .get()->SetPosition(LoadWordsPos[0]);
		LoadWords["ソ"].get()->SetPosition(LoadWordsPos[1]);
		LoadWords["ー"].get()->SetPosition(LoadWordsPos[2]);
		LoadWords["ス"].get()->SetPosition(LoadWordsPos[3]);
		LoadWords["読"].get()->SetPosition(LoadWordsPos[4]);
		LoadWords["み_1"].get()->SetPosition(LoadWordsPos[5]);
		LoadWords["込"].get()->SetPosition(LoadWordsPos[6]);
		LoadWords["み_2"].get()->SetPosition(LoadWordsPos[7]);
		LoadWords["中"].get()->SetPosition(LoadWordsPos[8]);

		//文字隠し
		LoadWords["完"].get()->SetSize({0.f,0.f});
		LoadWords["了"].get()->SetSize({0.f,0.f});

		if(FadeFlag)
		{
			WordsSize -= AddMovingVal;
		}

		LoadWordsChange = WordsSize <= 0.f;
	}


	WordsSize = std::clamp(WordsSize, 0.f, 200.f);
}