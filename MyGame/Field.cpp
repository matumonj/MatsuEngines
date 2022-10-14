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

void Field::Setplay(DebugCamera* camera)
{
	
}

void Field::Initialize(DebugCamera* camera)
{

	if (SceneManager::GetInstance()->GetScene() == SceneManager::BOSS) {
		FieldObject = TouchableObject::Create(ModelManager::GetIns()->GetModel(ModelManager::BOSSFIELD), camera);

		//ミニマップ(ボスフィールド)
		SetFieldModel(MINI ,ModelManager::GetIns()->GetModel(ModelManager::BOSSFIELD), camera);
		//ダメージエリア
		SetFieldModel(DAMAGEAREA, ModelManager::GetIns()->GetModel(ModelManager::DAMAGEAREA), camera);
		//天球
		SetFieldModel(CELESTIALSPHERE, ModelManager::GetIns()->GetModel(ModelManager::CELESTIALSPHERE), camera);
		//ボスの背景obj
		SetFieldModel(BOSSBACK, ModelManager::GetIns()->GetModel(ModelManager::BACKGROUND), camera);
		
		//Bossの名前表示用とフィールド外周のダメージエリア通知
		Sprite::LoadTexture(40, L"Resources/BossName.png");
		Sprite::LoadTexture(41, L"Resources/warning1.png");
	}

	if(SceneManager::GetInstance()->GetScene()==SceneManager::PLAY|| SceneManager::GetInstance()->GetScene() == SceneManager::TUTORIAL){
		FieldObject = TouchableObject::Create(ModelManager::GetIns()->GetModel(ModelManager::FIELD), camera);
		//ミニマップ(通常ふぃ－るど)
		SetFieldModel(MINI, ModelManager::GetIns()->GetModel(ModelManager::FIELD), camera);
		//天球
		SetFieldModel(CELESTIALSPHERE, ModelManager::GetIns()->GetModel(ModelManager::CELESTIALSPHERE), camera);
	}

	//フィールド外周がダメージエリアになる警告スプライト
	Explanation = Sprite::Create(41, { WinApp::window_width / 2,WinApp::window_height / 2 });
	Explanation->SetAnchorPoint({ 0.5f,0.5f });
	Explanation->SetPosition({ WinApp::window_width / 2,WinApp::window_height / 2 });
	Explanation->SetSize({ 800.0f,800.0f });

	//ボスのネームプレート
	BossName = Sprite::Create(40, { WinApp::window_width / 2,WinApp::window_height / 2 });
	BossName->SetAnchorPoint({ 0.5f,0.5f });
	BossName->SetPosition({ WinApp::window_width / 2,WinApp::window_height / 2 });
	BossName->SetSize({ 800,800 });

	Texture::LoadTexture(299, L"Resources/AOE.png");

	playerpoint = Texture::Create(299, { 0.0f ,0.0f ,0.0f }, { 100.0f ,100.0f ,1.0f }, { 1.0f ,1.0f ,1.0f ,1.0f });
	playerpoint->CreateTexture();
	playerpoint->SetAnchorPoint({ 0.5f,0.5f });
}

void Field::Update_Tutorial(DebugCamera* camera)
{
	FieldObject->SetPosition({ 0.0f,-25.0f,0.0f });
	FieldObject->SetFogCenter({ 125.0f, -25.0f, -680.0f });
	FieldObject->setFog(TRUE);

	SetFieldUpdate(CELESTIALSPHERE, camera, { 0.0f,30.0f,0.0f }, { 40.0f,40.0f,40.0f }, FALSE, TRUE);
	FieldObject->SetColor({ 0.2f,0.2f,0.2f,1.0f });
	FieldObject->Update({ 0.2f,0.2f,0.2f,1.0f }, camera);

}

void Field::Update_Play(DebugCamera* camera)
{
	FieldObject->SetPosition({ 0.0f,-25.0f,0.0f });
	FieldObject->SetFogCenter({ 0.0f, -20.0f, 0.0f });
	FieldObject->SetColor({ 0.2f,0.2f,0.2f,1.0f });
	FieldObject->Update({ 0.2f,0.2f,0.2f,1.0f }, camera);

}

void Field::Update_Boss(DebugCamera* camera)
{
	SpriteFeed(TexAlpha_BossName, feed_BossName, feedSpeed_BossName, 1.5f);
	if (CameraControl::GetInstance()->GetCameraState() == CameraControl::PLAYER) {
		SpriteFeed(t, feed, feedSpeed_Explanation, 2.5f);
	}

	SetFieldUpdate(CELESTIALSPHERE, camera, { 0.0f,30.0f,0.0f }, { 40.0f,40.0f,40.0f }, FALSE, TRUE);
	SetFieldUpdate(DAMAGEAREA, camera, { 0.0f,-19.2f,0.0f }, { 1.0f,1.0f,1.0f }, TRUE, FALSE);
	SetFieldUpdate(BOSSBACK, camera, { 0.0f,-19.2f,0.0f }, { 0.5f,0.5f,0.5f }, FALSE, TRUE);

	//フィールド外周とプレイヤーの当たり判定(現時点では矩形と点)
	FieldDamageAreaCol();
	//FieldObject->setFog(TRUE);
	FieldObject->SetPosition({ 0.0f,-20.0f,0.0f });
	FieldObject->SetColor({ 0.2f,0.2f,0.2f,1.0f });
	FieldObject->Update({ 0.2f,0.2f,0.2f,1.0f }, camera);

	playerpoint->SetPosition({ PlayerControl::GetInstance()->GetPlayer()->GetPosition().x,PlayerControl::GetInstance()->GetPlayer()->GetPosition().y + 10,PlayerControl::GetInstance()->GetPlayer()->GetPosition().z });
	playerpoint->Update(dc);
	playerpoint->SetScale({ 4,4,4 });
	playerpoint->SetBillboard(true);
	//playerpoint->SetRotation({ 0.0f,0.0f,0.0f });
	playerpoint->SetColor({ 1.0f,1.0f,1.0f,1 });

}

void Field::Update(DebugCamera* camera)
{
	if (SceneManager::GetInstance()->GetScene() == SceneManager::TUTORIAL) {
		Update_Tutorial(camera);
	}
	if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY) {
		Update_Play(camera);
	}
	if (SceneManager::GetInstance()->GetScene() == SceneManager::BOSS) {
		Update_Boss(camera);
	}
	SetFieldUpdate(MINI, dc, { 0.0f,-25.0f,0.0f }, { 1.0f,0.0f,1.0f });
	t = min(t, 2.5f);
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
		player.y = Ppos.z * -1;

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
		alpha += feedSpeed;
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