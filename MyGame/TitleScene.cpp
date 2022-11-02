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
	Feed::GetInstance()->initialize();

	Sprite::LoadTexture(0, L"Resources/2d/LevelUp/font.png");
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
	if (Input::GetInstance()->TriggerButton(Input::A)) {//押されたら
	BaseScene* scene = new MapCreateScene(sceneManager_);//次のシーンのインスタンス生成
	SceneManager::GetInstance()->SetScene(SceneManager::MAPCREATE);
	sceneManager_->SetnextScene(scene);//シーンのセット
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
	ImGui::Begin("Fog");
	ImGui::SliderFloat("FogX", &CameraPos.x, -100, 100);
	ImGui::SliderFloat("FogY", &FogPos.y, -100, 100);
	ImGui::SliderFloat("FogZ", &CameraPos.y, -500, 500);
	ImGui::End();
	ImGui::Begin("Angle");
	ImGui::SliderFloat("CamAngle", &Cangle, 0, 360);
	
	ImGui::End();
	Object3d::PreDraw();
	celestal->Draw();
	field->Draw();
	Object3d::PostDraw();

	titlesprite->setcolor({ 1,1,1,0.5 });
	titlesprite2->setcolor({ 1,1,1,0.5 });
	Sprite::PreDraw();
	titlesprite2->Draw();
titlesprite->Draw();

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
	//delete postEffect;
	delete titlesprite;
}