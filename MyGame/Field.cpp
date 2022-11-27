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
	for (int i = 0; i < objNum; i++)
	{
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
	if (SceneManager::GetInstance()->GetScene() == SceneManager::BOSS)
	{
		//FieldObject->CreateGraphicsPipeline(L"Resources/Shader/Object3dVS.hlsl", L"Resources/Shader/Object3dPS.hlsl", L"Resources/Shader/BasicGS.hlsl");
		//ミニマップ(ボスフィールド)
		//ダメージエリア
		SetFieldModel(DAMAGEAREA, ModelManager::GetIns()->GetModel(ModelManager::DAMAGEAREA), camera);
		//天球
		SetFieldModel(CELESTIALSPHERE, ModelManager::GetIns()->GetModel(ModelManager::CELESTIALSPHERE), camera);
		//ボスの背景obj
		SetFieldModel(BOSSBACK, ModelManager::GetIns()->GetModel(ModelManager::BOSSFIELD), camera);

		//Bossの名前表示用とフィールド外周のダメージエリア通知
		Sprite::LoadTexture(40, L"Resources/BossName.png");
		Sprite::LoadTexture(41, L"Resources/warning1.png");
	}

	if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY || SceneManager::GetInstance()->GetScene() ==
		SceneManager::TUTORIAL)
	{
		FieldObject = TouchableObject::Create(ModelManager::GetIns()->GetModel(ModelManager::FIELD), camera);
		//ミニマップ(通常ふぃ－るど)
		//天球
		SetFieldModel(CELESTIALSPHERE, ModelManager::GetIns()->GetModel(ModelManager::CELESTIALSPHERE), camera);
		
		}
	
	if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY)
	{
		//ボスの背景obj
		SetFieldModel(BOSSBACK, ModelManager::GetIns()->GetModel(ModelManager::BOSSFIELD), camera);
	}

	if (SceneManager::GetInstance()->GetScene() == SceneManager::MAPCREATE)
	{
		FieldObject = TouchableObject::Create(ModelManager::GetIns()->GetModel(ModelManager::FIELD), camera);
	}

	//ボスのネームプレート
	BossName = Sprite::Create(40, { WinApp::window_width / 2, WinApp::window_height / 2 });
	BossName->SetAnchorPoint({ 0.5f, 0.5f });
	BossName->SetPosition({ WinApp::window_width / 2, WinApp::window_height / 2 });
	BossName->SetSize({ 800, 800 });
	
}

void Field::Update_Tutorial(DebugCamera* camera)
{
	if (FieldObject == nullptr)
	{
		return;
	}
	FieldObject->SetPosition({ 0.0f, -25.0f, 0.0f });
	FieldObject->SetFogCenter({ 125.0f, -25.0f, -680.0f });
	FieldObject->setFog(TRUE);
	SetFieldUpdate(CELESTIALSPHERE, camera, { 0.0f, 30.0f, 0.0f }, { 40.0f, 40.0f, 40.0f }, FALSE, TRUE);
	FieldObject->SetColor({ 0.2f, 0.2f, 0.2f, 1.0f });
	FieldObject->Update({ 0.2f, 0.2f, 0.2f, 1.0f }, camera);
	
	
}

void Field::Update_Play(DebugCamera* camera)
{
	if (CameraControl::GetInstance()->GetCameraState_Spline() != CameraControl::PLAYCUTEND)
	{
		if (camera != nullptr)
		{
			FogCenterPos = camera->GetEye();
		}
	} else
	{
		FogCenterPos = PlayerControl::GetInstance()->GetPlayer()->GetPosition();
	}

	if (FieldObject == nullptr)
	{
		return;
	}
	CelestalRot += 0.1f;
	m_object[CELESTIALSPHERE]->SetRotation({ 90.0f, CelestalRot, 180.0f });
	SetFieldUpdate(CELESTIALSPHERE, camera, { 0.0f, 2290.0f, 0.0f }, { 30.0f, 30.0f, 30.0f }, FALSE, TRUE);
	FieldObject->SetPosition({ 0.0f, -25.0f, 0.0f });
	FieldObject->SetFogCenter(FogCenterPos);
	FieldObject->SetColor({ 0.2f, 0.2f, 0.2f, 1.0f });
	FieldObject->setFog(true);
	FieldObject->Update({ 0.2f, 0.2f, 0.2f, 1.0f }, camera);

	m_object[BOSSBACK]->SetRotation({ 0,180,0 });
	SetFieldUpdate(BOSSBACK, camera, { 22,-70,1010 }, { 1.0f, 1.0f, 1.0f }, FALSE, TRUE);

}

void Field::Update_Edit(DebugCamera* camera)
{
	FieldObject->SetPosition({ 0.0f, -25.0f, 0.0f });
	FieldObject->SetFogCenter({ 0.0f, -20.0f, 0.0f });
	FieldObject->SetColor({ 0.2f, 0.2f, 0.2f, 1.0f });
	FieldObject->Update({ 0.2f, 0.2f, 0.2f, 1.0f }, camera);
}

void Field::Update_Boss(DebugCamera* camera)
{
	SpriteFeed(TexAlpha_BossName, feed_BossName, feedSpeed_BossName, 1.5f);
	if (CameraControl::GetInstance()->GetCameraState() == CameraControl::PLAYER)
	{
		SpriteFeed(t, feed, feedSpeed_Explanation, 2.5f);
	}

	SetFieldUpdate(CELESTIALSPHERE, camera, { 0.0f, 30.0f, 0.0f }, { 40.0f, 40.0f, 40.0f }, FALSE, TRUE);
	SetFieldUpdate(DAMAGEAREA, camera, { 0.0f, -19.2f, 0.0f }, { 1.0f, 1.0f, 1.0f }, TRUE, FALSE);
	SetFieldUpdate(BOSSBACK, camera, {0,-19,0}, { 1.0f, 1.0f, 1.0f }, FALSE, TRUE);

	//フィールド外周とプレイヤーの当たり判定(現時点では矩形と点)
	//FieldDamageAreaCol();

	//playerpoint->SetPosition({ PlayerControl::GetInstance()->GetPlayer()->GetPosition().x,PlayerControl::GetInstance()->GetPlayer()->GetPosition().y + 10,PlayerControl::GetInstance()->GetPlayer()->GetPosition().z });
	//playerpoint->Update(dc);
	//playerpoint->SetScale({ 4,4,4 });
	//playerpoint->SetBillboard(true);
	//playerpoint->SetRotation({ 0.0f,0.0f,0.0f });
	//playerpoint->SetColor({ 1.0f,1.0f,1.0f,1 });
}

void Field::Update(DebugCamera* camera)
{
	if (SceneManager::GetInstance()->GetScene() == SceneManager::TUTORIAL)
	{
		Update_Tutorial(camera);
	}
	if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY)
	{
		Update_Play(camera);
	}
	if (SceneManager::GetInstance()->GetScene() == SceneManager::BOSS)
	{
		Update_Boss(camera);
	}

	t = min(t, 2.5f);
	t = max(t, 0.0f);
	TexAlpha_BossName = min(TexAlpha_BossName, 3.0f);
	TexAlpha_BossName = max(TexAlpha_BossName, 0.0f);
}

void Field::MiniFieldDraw()
{
	Object3d::PreDraw();
	//m_object[MINI]->Draw();
	Object3d::PostDraw();


}

#include"imgui.h"

void Field::Draw()
{
	/*ImGui::Begin("field");
	ImGui::SliderFloat("PosX", &BossFieldPos.x, -700, 700);
	ImGui::SliderFloat("Posy", &BossFieldPos.y, -70, 70);

	ImGui::SliderFloat("Posz", &BossFieldPos.z, 0, 1700);
	ImGui::End();
	*///	if (FieldObject == nullptr)return;
	Object3d::PreDraw();
	if (SceneManager::GetInstance()->GetScene() != SceneManager::MAPCREATE)
	{
		m_object[CELESTIALSPHERE]->Draw();
	}

	if (SceneManager::GetInstance()->GetScene() != SceneManager::BOSS)
	{
		FieldObject->Draw();
	} else
	{
		ModelDraw_nullCheck(BOSSBACK);
		ModelDraw_nullCheck(DAMAGEAREA);
	}
	if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY)
	{
		ModelDraw_nullCheck(BOSSBACK);
	}
	Object3d::PostDraw();
	if (SceneManager::GetInstance()->GetScene() == SceneManager::BOSS)
	{
		BossName->setcolor({ 1.0f, 1.0f, 1.0f, TexAlpha_BossName });
	}
}
void Field::WarningDraw()
{
	Sprite::PreDraw();
	BossName->Draw();
	//Explanation->Draw();
	Sprite::PostDraw();
}

void Field::FieldDamageAreaCol()
{


}

void Field::SpriteFeed(float& alpha, bool& feed, const float feedSpeed, const float MaxAlphaValue)
{
	if (feed)
	{
		alpha -= 0.02f;
	} else
	{
		alpha += feedSpeed;
	}
	if (alpha >= MaxAlphaValue)
	{
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
	if (m_object[type] == nullptr)
	{
		return;
	}

	m_object[type]->SetPosition(Pos);
	m_object[type]->SetScale(Scl);
	m_object[type]->SetUVf(uvscroll);
	m_object[type]->setFog(fog);
	m_object[type]->Update({ 1.0f, 1.0f, 1.0f, 1.0f }, camera);
}

void Field::ModelDraw_nullCheck(ObjType type)
{
	if (m_object[type] == nullptr)
	{
		return;
	}
	m_object[type]->Draw();
}
