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
	:BaseScene(sceneManager)
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

	Sprite::LoadTexture(3, L"Resources/2d/title/titlesp.png");
	titlesprite = Sprite::Create(3, { 0.0f,0.0f });

	Sprite::LoadTexture(4, L"Resources/title2.png");
	titlesprite2 = Sprite::Create(4, {  0,0.0f });

	Sprite::LoadTexture(5, L"Resources/2d/title/gameplay.png");
	Sprite::LoadTexture(6, L"Resources/2d/title/edit.png");
	
	Sprite*navGameSprite= Sprite::Create(5, { 0,0.0f });
	Sprite*navEditSprite= Sprite::Create(6, { 0,0.0f });
	
	TitleMenu[0].reset(navGameSprite);

	TitleMenu[1].reset(navEditSprite);
	for (int i = 0; i < 2; i++) {
		menuAlpha[i] = 1.0f;
		MenuScale[i] = { 1900,1000 };
		TitleMenu[i]->SetPosition({ 950,500 });
		TitleMenu[i]->SetSize({ 1900,1000 });
		TitleMenu[i]->SetAnchorPoint({ 0.5,0.5 });
	}
	Feed::GetInstance()->initialize();

	Sprite::LoadTexture(0, L"Resources/2d/LevelUp/debugfont.png");
	camera = new DebugCamera(WinApp::window_width, WinApp::window_height);

	field = std::make_unique<Object3d>();
	field->SetModel(ModelManager::GetIns()->GetModel(ModelManager::FIELD));
	field->Initialize(camera);
	celestal = std::make_unique<Object3d>();
	celestal->SetModel(ModelManager::GetIns()->GetModel(ModelManager::CELESTIALSPHERE));
	celestal->Initialize(camera);
	DebugTextSprite::GetInstance()->Initialize(0);
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
	field->SetRotation({ 0,0,0 });
	field->SetScale({ 0.15f,0.15f,0.15f });
	field->SetFogCenter(FogPos);
	field->setFog(TRUE);
	celestal->SetRotation({ 0,0,0 });
	celestal->SetScale({ 30.f,30.1f,30.1f });
	if (Input::GetInstance()->TriggerButton(Input::B)) {//押されたら
		menujudg_Play=true;
		BaseScene* scene = new Tutorial(sceneManager_);//次のシーンのインスタンス生成
		SceneManager::GetInstance()->SetScene(SceneManager::TUTORIAL);
		sceneManager_->SetnextScene(scene);//シーンのセット
		//押されたら
	feedf = true;
	}
if (feedf) {
	Cangle += 0.5;
	float cameratocenter_x = sqrtf((CameraPos.x - 16) * (CameraPos.x - 16));
	if (cameratocenter_x < 1.0f&&CameraPos.y<-30) {
		BackCam = true;
	}
	
}
else {
	Cangle += 0.1;
}
if (CameraPos.y <= -40) {
	Feed::GetInstance()->Update_White(Feed::FEEDIN);
}
	

	if (Feed::GetInstance()->GetAlpha() >= 1.0f) {
	BaseScene* scene = new Tutorial(sceneManager_);//次のシーンのインスタンス生成
		SceneManager::GetInstance()->SetScene(SceneManager::TUTORIAL);
		sceneManager_->SetnextScene(scene);//シーンのセット
	}
	//titlesprite2->SetRotation(180);165
	if (Cangle >= 360) {
		Cangle = 0;
	}
	
	if (!BackCam) {
		CameraPos.x = sinf(Cangle * (PI / 180)) * 40.0f;
		CameraPos.y = cosf(Cangle * (PI / 180)) * 40.0f;
	}
	else {
		CameraPos.y--;
	}

	TitleMenu[0]->SetSize(MenuScale[0]);
	TitleMenu[1]->SetSize(MenuScale[1]);

	TitleMenu[0]->setcolor({ 1,1,1,menuAlpha[0] });
	TitleMenu[1]->setcolor({ 1,1,1,menuAlpha[1] });


	if (menujudg_Play) {
		MenuScale[0].x += 20;
		MenuScale[0].y += 20;
		menuAlpha[0] -= 0.02f;
		menuAlpha[1] = 0.0f;
	}
	if (menujudg_Edit) {
		MenuScale[1].x += 20;
		MenuScale[1].y += 20;
		menuAlpha[1] -= 0.02f;
		menuAlpha[0] = 0.0f;
	}
	camera->SetEye({ CameraPos.x,2,CameraPos.y });
	camera->SetTarget({ 0,0,0 });
	camera->Update();
	titlesprite->SetSize({ WinApp::window_width/2,WinApp::window_height });
	titlesprite2->SetSize({ WinApp::window_width  ,WinApp::window_height });
	field->Update({ 0.6,0.6,0.6,1 }, camera);
	celestal->Update({ 0.6,0.6,0.6,1 }, camera);
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

	titlesprite->setcolor({ 1,1,1,0.5 });
	titlesprite2->setcolor({ 1,1,1,0.5 });
	Sprite::PreDraw();
	titlesprite2->Draw();
titlesprite->Draw();
//for (int i = 0; i < 2; i++) {
	TitleMenu[0]->Draw();
//}
	DebugTextSprite::GetInstance()->DrawAll();
	Sprite::PostDraw();
	Feed::GetInstance()->Draw();
}

void TitleScene::Draw()
{
	//ポストエフェクトの描画
	DirectXCommon::GetInstance()->BeginDraw();//描画コマンドの上らへんに
	SpriteDraw();
	DirectXCommon::GetInstance()->EndDraw();
}


/*------------------------*///17,125
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