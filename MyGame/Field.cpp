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
	Destroy_unique(CelestialSphereObject);
	Destroy(FieldObject);
	Destroy_unique(BackObject);
	for (int i = 0; i < objNum; i++) {
		Destroy_unique(m_object[i]);
	}
}
Field* Field::GetInstance()
{
	static Field instance;
	return &instance;
}
void Field::Initialize(DebugCamera* camera)
{

	if (SceneManager::GetInstance()->GetScene() == SceneManager::BOSS) {
			TouchableObject* bossField = TouchableObject::Create(ModelManager::GetIns()->GetModel(ModelManager::BOSSFIELD), camera);
			FieldObject = bossField;

			SetFieldModel(MINI, ModelManager::GetIns()->GetModel(ModelManager::BOSSFIELD), camera);

			SetFieldModel(DAMAGEAREA, ModelManager::GetIns()->GetModel(ModelManager::DAMAGEAREA), camera);

			SetFieldModel(CELESTIALSPHERE, ModelManager::GetIns()->GetModel(ModelManager::CELESTIALSPHERE), camera);

			SetFieldModel(BOSSBACK, ModelManager::GetIns()->GetModel(ModelManager::BACKGROUND), camera);

		//Bossの名前表示用とフィールド外周のダメージエリア通知
		Sprite::LoadTexture(40, L"Resources/BossName.png");
		Sprite::LoadTexture(41, L"Resources/warning1.png");
	}
	else {
		TouchableObject* normalField = TouchableObject::Create(ModelManager::GetIns()->GetModel(ModelManager::FIELD), camera);
		FieldObject = normalField;

		SetFieldModel(MINI, ModelManager::GetIns()->GetModel(ModelManager::FIELD), camera);

		SetFieldModel(CELESTIALSPHERE, ModelManager::GetIns()->GetModel(ModelManager::CELESTIALSPHERE), camera);

	}
	m_object[ObjType::MINI]->SetCamera(dc);
	
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
}

	
void Field::Update(DebugCamera* camera)
{
	m_object[ObjType::MINI]->SetCamera(dc);

SetFieldUpdate(MINI, camera, { 0.0f,-25.0f,0.0f }, { 1,0,1 });
	
	if (SceneManager::GetInstance()->GetScene() == SceneManager::TUTORIAL) {
		FieldObject->SetPosition({ 0.0f,-25.0f,0.0f });
		FieldObject->SetFogCenter({ 125.0f, -25.0f, -680.0f });
		FieldObject->setFog(TRUE);

		SetFieldUpdate(CELESTIALSPHERE, camera, { 0.0f,30.0f,0.0f }, { 40.0f,40.0f,40.0f }, FALSE, TRUE);
}

	else if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY|| SceneManager::GetInstance()->GetScene() == SceneManager::MAPCREATE) {
		if (CameraControl::GetInstance()->GetMoveBosAreaCam()==CameraControl::TARGETPLAYER) {
			ypos+=0.4f;
			PlayerControl::GetInstance()->GetPlayer()->SetStopFlag(true);
		}
		FieldObject->SetPosition({ssp });
		FieldObject->SetFogCenter({ 125.0f, -25.0f, -680.0f });
		FieldObject->setFog(FALSE);

	}

	else if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY) {
		if (SceneManager::GetInstance()->GetScene() != SceneManager::MAPCREATE) {
			XMFLOAT3 Ppos = PlayerControl::GetInstance()->GetPlayer()->GetPosition();
			FieldObject->SetPosition({ Ppos.x, ypos,Ppos.z });
			FieldObject->setFog(FALSE);
			FieldObject->Update({ 0.2f,0.2f,0.2f,1.0f }, camera);
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

		SetFieldUpdate(CELESTIALSPHERE, camera, { 0.0f,30.0f,0.0f }, { 40.0f,40.0f,40.0f }, FALSE, TRUE);
		SetFieldUpdate(DAMAGEAREA, camera, { 0.0f,0.0f,0.0f }, { 1.0f,1.0f,1.0f },TRUE,FALSE);
		SetFieldUpdate(BOSSBACK, camera, { 0.0f,-19.2f,0.0f }, { 1,1,1 },TRUE,TRUE);

		//フィールド外周とプレイヤーの当たり判定(現時点では矩形と点)
		FieldDamageAreaCol();
	}

	FieldObject->SetColor({ 0.2f,0.2f,0.2f,1.0f });
	FieldObject->Update({ 0.2f,0.2f,0.2f,1.0f }, camera);

	SetFieldUpdate(MINI,dc, { 0.0f,-25.0f,0.0f }, { 1.0f,0.0f,1.0f });

	 t= min(t, 2.5f);
	t = max(t, 0.0f);
	TexAlpha_BossName = min(TexAlpha_BossName, 3.0f);
	TexAlpha_BossName = max(TexAlpha_BossName, 0.0f);
}
void Field::MiniFieldDraw()
{
	Object3d::PreDraw();
	m_object[ObjType::MINI]->Draw();
	Object3d::PostDraw();
}
#include"imgui.h"
void Field::Draw()
{
	Object3d::PreDraw();
	m_object[ObjType::CELESTIALSPHERE]->Draw();
	FieldObject->Draw();
	if (SceneManager::GetInstance()->GetScene() == SceneManager::BOSS) {
		ModelDraw_nullCheck(BOSSBACK);
		ModelDraw_nullCheck(DAMAGEAREA);
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

void Field::SetFieldModel(ObjType type, Model* model, DebugCamera* camera)
{
	m_object[type] = std::make_unique<Object3d>();
	m_object[type]->SetModel(model);
	m_object[type]->Initialize(camera);
	
}

void Field::SetFieldUpdate(ObjType type, DebugCamera* camera, XMFLOAT3 Pos, XMFLOAT3 Scl, bool uvscroll, bool fog)
{
	if (m_object[type] == nullptr)return;

	m_object[type]->SetPosition(Pos);
	m_object[type]->SetScale(Scl);
	m_object[type]->SetUVf(uvscroll);
	m_object[type]->setFog(fog);
	m_object[type]->Update({ 1.0f,1.0f,1.0f,1.0f }, camera);
}

void Field::ModelDraw_nullCheck(ObjType type)
{
	if (m_object[type] == nullptr)return;
	m_object[type]->Draw();
}