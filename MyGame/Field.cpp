#include "Field.h"
#include"CameraControl.h"
#include"TouchableObject.h"
#include"CollisionManager.h"
#include"SceneManager.h"
#include"ModelManager.h"
#include"Destroy.h"
#include"CollisionPrimitive.h"
#include"PlayerControl.h"
#include"EnemyControl.h"
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
	Enemyicon.clear();
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
		//FieldObject->CreateGraphicsPipeline(L"Resources/Shader/Object3dVS.hlsl", L"Resources/Shader/Object3dPS.hlsl", L"Resources/Shader/BasicGS.hlsl");
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
		//ミニマップ(通常ふぃ−るど)
		SetFieldModel(MINI, ModelManager::GetIns()->GetModel(ModelManager::MINI), camera);
		//天球
		SetFieldModel(CELESTIALSPHERE, ModelManager::GetIns()->GetModel(ModelManager::CELESTIALSPHERE), camera);
		Texture::LoadTexture(36, L"Resources/life.png");

		playerpoint = Texture::Create(36, { 0.0f ,0.0f ,0.0f }, { 100.0f ,100.0f ,1.0f }, { 1.0f ,1.0f ,1.0f ,1.0f });
		playerpoint->CreateTexture();
		playerpoint->SetAnchorPoint({ 0.5f,0.5f });
	}
	Texture::LoadTexture(37, L"Resources/2d/icon/enemyicon.png");
	if (SceneManager::GetInstance()->GetScene() == SceneManager::TUTORIAL) {
		EnemyIconSize = (int)EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL).size();

	}
	if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY) {
		EnemyIconSize = (int)EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE).size();
	}
		Enemyicon.resize(EnemyIconSize);
		std::vector<Texture*>l_tex;
		l_tex.resize(EnemyIconSize);
		for (int i = 0; i < EnemyIconSize; i++) {
			l_tex[i] = Texture::Create(37, {0.0f ,0.0f ,0.0f}, {100.0f ,100.0f ,1.0f}, {1.0f ,1.0f ,1.0f ,1.0f});
			Enemyicon[i].reset(l_tex[i]);
			Enemyicon[i]->CreateTexture();
			Enemyicon[i]->SetAnchorPoint({ 0.5f,0.5f });
		}
	
	if (SceneManager::GetInstance()->GetScene() == SceneManager::MAPCREATE) {
		FieldObject = TouchableObject::Create(ModelManager::GetIns()->GetModel(ModelManager::FIELD), camera);
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

	//Texture::LoadTexture(299, L"Resources/AOE.png");

	//playerpoint = Texture::Create(299, { 0.0f ,0.0f ,0.0f }, { 100.0f ,100.0f ,1.0f }, { 1.0f ,1.0f ,1.0f ,1.0f });
	//playerpoint->CreateTexture();
	//playerpoint->SetAnchorPoint({ 0.5f,0.5f });
}

void Field::Update_Tutorial(DebugCamera* camera)
{
	if (FieldObject == nullptr)return;
	FieldObject->SetPosition({ 0.0f,-25.0f,0.0f });
	FieldObject->SetFogCenter({ 125.0f, -25.0f, -680.0f });
	FieldObject->setFog(TRUE);
	SetFieldUpdate(CELESTIALSPHERE, camera, { 0.0f,30.0f,0.0f }, { 40.0f,40.0f,40.0f }, FALSE, TRUE);
	FieldObject->SetColor({ 0.2f,0.2f,0.2f,1.0f });
	FieldObject->Update({ 0.2f,0.2f,0.2f,1.0f }, camera);
	playerpoint->SetPosition({ PlayerControl::GetInstance()->GetPlayer()->GetPosition().x,PlayerControl::GetInstance()->GetPlayer()->GetPosition().y + 10,PlayerControl::GetInstance()->GetPlayer()->GetPosition().z });
	playerpoint->Update(dc);
	playerpoint->SetScale({ 4.0f,4.0f,4.0f });
	playerpoint->SetBillboard(true);
	playerpoint->SetColor({ 1.0f,1.0f,1.0f,1 });

	for (int i = 0; i < EnemyIconSize; i++) {
		//if (Enemyicon[i] == nullptr||Collision::GetLength(PlayerControl::GetInstance()->GetPlayer()->GetPosition(), EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[i]->GetPosition())>100) {
			continue;
		//}
		Enemyicon[i]->SetPosition({ EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[i]->GetPosition().x,
			EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[i]->GetPosition().y+10.0f,
			EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[i]->GetPosition().z});
		Enemyicon[i]->Update(dc);
		Enemyicon[i]->SetScale({ 4.0f,4.0f,4.0f });
		Enemyicon[i]->SetBillboard(true);
		Enemyicon[i]->SetColor({ 1.0f,1.0f,1.0f,1 });

		if (EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[i]->GetHP() <= 0) {
			Destroy_unique(Enemyicon[i]);
		}
	}
	
}

void Field::Update_Play(DebugCamera* camera)
{
	if (CameraControl::GetInstance()->GetCameraState_Spline() != CameraControl::PLAYCUTEND) {
		if (camera!= nullptr) {
			FogCenterPos = camera->GetEye();
		}
	}
	else {
		FogCenterPos = PlayerControl::GetInstance()->GetPlayer()->GetPosition();
	}
	
	if (FieldObject == nullptr)return;
	CelestalRot += 0.1f;
	m_object[CELESTIALSPHERE]->SetRotation({ 90.0f,CelestalRot,180.0f });
	SetFieldUpdate(CELESTIALSPHERE, camera, { 0.0f,2290.0f,0.0f }, { 40.0f,40.0f,40.0f }, FALSE, FALSE);
	FieldObject->SetPosition({ 0.0f,-25.0f,0.0f });
	FieldObject->SetFogCenter(FogCenterPos);
	FieldObject->SetColor({ 0.2f,0.2f,0.2f,1.0f });
	FieldObject->setFog(true);
	FieldObject->Update({ 0.2f,0.2f,0.2f,1.0f }, camera);

	playerpoint->SetPosition({ PlayerControl::GetInstance()->GetPlayer()->GetPosition().x,150.0f,PlayerControl::GetInstance()->GetPlayer()->GetPosition().z });
	playerpoint->Update(dc);
	playerpoint->SetScale({ 4.0f,4.0f,4.0f });
	playerpoint->SetBillboard(true);
	playerpoint->SetColor({ 1.0f,1.0f,1.0f,1.0f });

	
}

void Field::Update_Edit(DebugCamera* camera)
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

	//playerpoint->SetPosition({ PlayerControl::GetInstance()->GetPlayer()->GetPosition().x,PlayerControl::GetInstance()->GetPlayer()->GetPosition().y + 10,PlayerControl::GetInstance()->GetPlayer()->GetPosition().z });
	//playerpoint->Update(dc);
	//playerpoint->SetScale({ 4,4,4 });
	//playerpoint->SetBillboard(true);
	//playerpoint->SetRotation({ 0.0f,0.0f,0.0f });
	//playerpoint->SetColor({ 1.0f,1.0f,1.0f,1 });

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
	if (m_object[MINI] != nullptr) {
		m_object[MINI]->SetShadowF(FALSE);
	}
	SetFieldUpdate(MINI, dc, { 0.0f,-25.0f,0.0f }, { 1.0f,1.0f,1.0f });
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

	Texture::PreDraw();
	playerpoint->Draw();

	for (int i = 0; i < EnemyIconSize; i++) {
		if (SceneManager::GetInstance()->GetScene() == SceneManager::TUTORIAL) {
			if (Enemyicon[i] == nullptr || Collision::GetLength(PlayerControl::GetInstance()->GetPlayer()->GetPosition(), EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[i]->GetPosition()) > 100) {

				continue;
			}
			Enemyicon[i]->Draw();
		}
	}
	Texture::PostDraw();
}
#include"imgui.h"
void Field::Draw()
{
	if (FieldObject == nullptr)return;
	Object3d::PreDraw();
	if (SceneManager::GetInstance()->GetScene() != SceneManager::MAPCREATE) {
		m_object[ObjType::CELESTIALSPHERE]->Draw();
	}
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
			PlayerControl::GetInstance()->GetPlayer()->RecvDamage(10);
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