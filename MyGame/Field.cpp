#include "Field.h"
#include"CameraControl.h"
#include"TouchableObject.h"
#include"CollisionManager.h"
#include"SceneManager.h"
#include"ModelManager.h"
#include"Destroy.h"
#include"CollisionPrimitive.h"
#include"PlayerControl.h"
#include"ChestControl.h"
Field::~Field()
{
	
}

void Field::Finalize()
{
	Destroy(CelestialSphereModel);
	Destroy_unique(CelestialSphereObject);
	Destroy(FieldObject);
	Destroy(FieldModel);
	Destroy(BackM);
	Destroy_unique(BackObject);
	Destroy(BossFieldObject);
}
Field* Field::GetInstance()
{
	static Field instance;
	return &instance;
}
bool Field::Initialize(DebugCamera* camera)
{
	//playscene用のスカイドーム
	CelestialSphereObject = std::make_unique<Object3d>();
	CelestialSphereModel = Model::CreateFromOBJ("skydome");
	miniObj = std::make_unique<Object3d>();

	//Bossscene用の背景オブジェ
	BackObject= std::make_unique<Object3d>();
	//BackM = Model::CreateFromOBJ("BackGround");
	//Bossscene用のフィールドの外周ダメージエリア
	DamageAreaObj = std::make_unique<Object3d>();

	if (SceneManager::GetInstance()->GetScene() == SceneManager::BOSS) {

		FieldObject = TouchableObject::Create(ModelManager::GetIns()->GetModel(ModelManager::BOSSFIELD), camera);

		BackObject->Initialize(camera);
		BackObject->SetModel(ModelManager::GetIns()->GetModel(ModelManager::BACKGROUND));

		DamageAreaObj->Initialize(camera);
		DamageAreaObj->SetModel(ModelManager::GetIns()->GetModel(ModelManager::DAMAGEAREA));
		miniObj->SetModel(ModelManager::GetIns()->GetModel(ModelManager::BOSSFIELD));

		//Bossの名前表示用とフィールド外周のダメージエリア通知
		Sprite::LoadTexture(40, L"Resources/BossName.png");
		Sprite::LoadTexture(41, L"Resources/warning1.png");
	}
	else {
		miniObj->SetModel(ModelManager::GetIns()->GetModel(ModelManager::FIELD));

		BossFieldObject = TouchableObject::Create(ModelManager::GetIns()->GetModel(ModelManager::BOSSFIELD), camera);

		FieldObject = TouchableObject::Create(ModelManager::GetIns()->GetModel(ModelManager::FIELD), camera);
	}
	miniObj->Initialize(dc);
	
	CelestialSphereObject->Initialize(camera);
	CelestialSphereObject->SetModel(CelestialSphereModel);

	//フィールド外周がダメージエリアになる警告スプライト
	Explanation = Sprite::Create(41, { WinApp::window_width / 2,WinApp::window_height / 2 });
	Explanation->SetAnchorPoint({ 0.5f,0.5f });
	Explanation->SetPosition({ WinApp::window_width / 2,WinApp::window_height / 2  });
	Explanation->SetSize({ 800,800 });

	//ボスのネームプレート
	BossName = Sprite::Create(40, { WinApp::window_width / 2,WinApp::window_height / 2 });
	BossName->SetAnchorPoint({ 0.5f,0.5f });
	BossName->SetPosition({ WinApp::window_width / 2,WinApp::window_height / 2 });
	BossName->SetSize({ 800,800 });

	ypos = -40.0f;
	return true;
}

	
void Field::Update(DebugCamera* camera)
{
	CelestialSphereObject->SetPosition({ 0.0f,30.0f,0.0f });
	CelestialSphereObject->SetScale({ 40.0f,40.0f,40.0f });

	BackObject->SetPosition({ 0.0f,30.0f,0.0f });
	BackObject->SetScale({ 0.5f,0.5f,0.5f });
	miniObj->SetScale({1,0,1});

	miniObj->SetCamera(dc);
	miniObj->SetPosition({ 0.0f,-25.0f,0.0f });
	miniObj->SetRotation({ 0,0,0 });
	miniObj->setFog(FALSE);

	if (SceneManager::GetInstance()->GetScene() == SceneManager::TUTORIAL) {
		FieldObject->SetScale(1.0f);
		FieldObject->SetPosition({ 0.0f,-25.0f,0.0f });
		FieldObject->SetFogCenter({ 125.0f, -25.0f, -680.0f });
		FieldObject->setFog(TRUE);
		CelestialSphereObject->setFog(TRUE);


	}
	else if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY|| SceneManager::GetInstance()->GetScene() == SceneManager::MAPCREATE) {
		if (CameraControl::GetInstance()->GetMoveBosAreaCam()==CameraControl::TARGETPLAYER) {
			ypos+=0.4f;
			PlayerControl::GetInstance()->GetPlayer()->SetStopFlag(true);
		}
		FieldObject->SetScale(1.0f);
		FieldObject->SetPosition({ssp });
		FieldObject->SetFogCenter({ 125.0f, -25.0f, -680.0f });
		FieldObject->setFog(FALSE);
		CelestialSphereObject->setFog(FALSE);


	}
	else if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY) {
		if (SceneManager::GetInstance()->GetScene() != SceneManager::MAPCREATE) {
			XMFLOAT3 Ppos = PlayerControl::GetInstance()->GetPlayer()->GetPosition();
			BossFieldObject->SetScale(1.0f);
			BossFieldObject->SetPosition({ Ppos.x, ypos,Ppos.z });
			BossFieldObject->setFog(FALSE);
			BossFieldObject->setFog(FALSE);

	BossFieldObject->SetColor({ 0.2f,0.2f,0.2f,1.0f });
	BossFieldObject->Update({ 0.2f,0.2f,0.2f,1.0f }, camera);
		}
	}
		else if (SceneManager::GetInstance()->GetScene() == SceneManager::BOSS) {
		SpriteFeed(TexAlpha_BossName, feed_BossName, feedSpeed_BossName, 1.5f);
		if (CameraControl::GetInstance()->GetCameraState() == CameraControl::PLAYER) {
			SpriteFeed(t, feed, feedSpeed_Explanation, 2.5f);
		}
		FieldObject->SetScale(1.0f);
		FieldObject->SetPosition({ 0.0f,-20.0f,0.0f });

		FieldObject->SetFogCenter({ 0.0f, -20.0f, 0.0f });

		CelestialSphereObject->setFog(TRUE);
	
		DamageAreaObj->setFog(FALSE);

		BackObject->setFog(TRUE);
		BackObject->Update({ 0.6f,0.6f,0.6f,1.0f }, camera);

		DamageAreaObj->SetUVf(true);
		DamageAreaObj->SetPosition({ 0.0f,-19.2f,0.0f });
		DamageAreaObj->Update({ 0.6f,0.6f,0.6f,1.0f }, camera);

		//フィールド外周とプレイヤーの当たり判定(現時点では矩形と点)
		FieldDamageAreaCol();
	
	}
	FieldObject->SetColor({ 0.2f,0.2f,0.2f,1.0f });
	FieldObject->Update({ 0.2f,0.2f,0.2f,1.0f }, camera);
	miniObj->SetColor({ 0.2f,0.2f,0.2f,1.0f });
	miniObj->Update({ 0.2f,0.2f,0.2f,1.0f },dc);

	CelestialSphereObject->Update({ 1.0f,1.0f,1.0f,1.0f }, camera);

	 t= min(t, 2.5f);
	t = max(t, 0.0f);
	TexAlpha_BossName = min(TexAlpha_BossName, 3.0f);
	TexAlpha_BossName = max(TexAlpha_BossName, 0.0f);
}
void Field::MiniFieldDraw()
{
	Object3d::PreDraw();
	miniObj->Draw();
	Object3d::PostDraw();
}
#include"imgui.h"
void Field::Draw()
{
	Object3d::PreDraw();
	CelestialSphereObject->Draw();
	
	FieldObject->Draw();
	//FieldObject->PostDraw();

	if (SceneManager::GetInstance()->GetScene() == SceneManager::BOSS) {
		BackObject->Draw();
		DamageAreaObj->Draw();
	}
	
	if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY) {
		if (ypos > -15) {
			BossFieldObject->Draw();
		}
	}
	Object3d::PostDraw();
	Explanation->setcolor({ 1.0f,1.0f,1.0f,t });
	BossName->setcolor({ 1.0f,1.0f,1.0f,TexAlpha_BossName });

	ImGui::Begin("pos");
	ImGui::SliderFloat("x", &ssp.x, -600, 600);
	ImGui::SliderFloat("y", &ssp.y, -100, 100);
	ImGui::SliderFloat("z", &ssp.z, -600, 600);
	ImGui::End();
}

void Field::WarningDraw()
{
	Sprite::PreDraw();
	BossName->Draw();
	Explanation->Draw();
	Sprite::PostDraw();
}

void Field::FieldDamageAreaCol()
{
	if (PlayerControl::GetInstance()->GetPlayer() != nullptr) {
		Box damagearea;
		Point player;
		Ppos = PlayerControl::GetInstance()->GetPlayer()->GetPosition();

		damagearea.position = { -60.0f,-50.0f };
		damagearea.scale = { 120.0f,110.0f };

		player.x = Ppos.x;
		player.y = Ppos.z*-1;

		if (Collision::CheckPoint2Rect(player, damagearea) == false) {
		//	PlayerControl::GetInstance()->GetPlayer()->RecvDamage(10);
		}
	}
}

void Field::SpriteFeed(float& alpha, bool& feed, const float feedSpeed, const float MaxAlphaValue)
{
	if (feed) {
		alpha -= 0.02f;
	} else {
		alpha+= feedSpeed;
	}
	if (alpha >= MaxAlphaValue) {
		feed = true;
	}
}