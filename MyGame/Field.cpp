#include "Field.h"
#include"TouchableObject.h"
#include"CollisionManager.h"
#include"SceneManager.h"
Field::~Field()
{
	delete CelestialSphereModel, CelestialSphereObject;
	delete FieldObject, FieldModel;
	delete BackM, BackObject;
}
Field* Field::GetInstance()
{
	static Field instance;
	return &instance;
}
bool Field::Initialize(DebugCamera* camera)
{
	CelestialSphereObject = std::make_unique<Object3d>();
	CelestialSphereModel = Model::CreateFromOBJ("skydome");
	BackObject= std::make_unique<Object3d>();
	DamageAreaObj = std::make_unique<Object3d>();
	BackM = Model::CreateFromOBJ("BackGround");
	if (SceneManager::GetInstance()->GetScene() == SceneManager::BOSS) {
		FieldModel = Model::CreateFromOBJ("BossField");
		DamageAreaModel = Model::CreateFromOBJ("BossFieldDamageArea");

		BackObject->Initialize(camera);
		BackObject->SetModel(BackM);

		DamageAreaObj->Initialize(camera);
		DamageAreaObj->SetModel(DamageAreaModel);

		Sprite::LoadTexture(40, L"Resources/warning1.png");
	
	}
	else {
		FieldModel = Model::CreateFromOBJ("LowPoly_Landscape");
	}

	//フィールドにモデル割り当て
	FieldObject = TouchableObject::Create(FieldModel,camera);

	CelestialSphereObject->Initialize(camera);
	CelestialSphereObject->SetModel(CelestialSphereModel);

	Explanation = Sprite::Create(40, { WinApp::window_width / 2,WinApp::window_height / 2 });
	Explanation->SetAnchorPoint({ 0.5f,0.5f });
	Explanation->SetPosition({ WinApp::window_width / 2,WinApp::window_height / 2 + 200 });
	Explanation->SetSize({ 800,800 });
	return true;
}

void Field::Update(DebugCamera* camera)
{
	CelestialSphereObject->SetPosition({ 0,30,0 });
	CelestialSphereObject->SetScale({ 40,40,40 });

	BackObject->SetPosition({ 0,30,0 });
	BackObject->SetScale({ 0.5,0.5,0.5 });

	FieldObject->SetScale(1.0f);
	FieldObject->SetPosition({ 0,-20,0 });

	if (SceneManager::GetInstance()->GetScene() == SceneManager::TUTORIAL) {

		FieldObject->SetFogCenter({ 125, -25, -680 });
		FieldObject->setFog(TRUE);
		CelestialSphereObject->setFog(TRUE);
	}
	else if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY) {
		
		FieldObject->SetFogCenter({ 125, -25, -680 });
		FieldObject->setFog(FALSE);
		CelestialSphereObject->setFog(FALSE);
	}
	else if (SceneManager::GetInstance()->GetScene() == SceneManager::BOSS) {
		if (feed) {
			TexAlpha -= 0.02f;
		}
		else {
			TexAlpha += 0.002f;
		}
		if (TexAlpha >= 1.0f) {
			feed = true;
		}
		FieldObject->SetFogCenter({ 0, -20, 0 });
		CelestialSphereObject->setFog(TRUE);
		BackObject->setFog(TRUE);
		DamageAreaObj->setFog(FALSE);
		
		DamageAreaObj->SetUVf(true);
		DamageAreaObj->SetPosition({ 0,-19.2,0 });
		DamageAreaObj->Update({ 0.6,0.6,0.6,1 }, camera);
	}
	FieldObject->SetColor({ 0.6,0.6,0.6,1 });
	FieldObject->Update({ 0.6,0.6,0.6,1 }, camera);
	CelestialSphereObject->Update({ 1,1,1,1 }, camera); 
	if (SceneManager::GetInstance()->GetScene() == SceneManager::BOSS) {
		BackObject->Update({ 0.6,0.6,0.6,1 }, camera);
	}
	TexAlpha = min(TexAlpha, 1);
	TexAlpha = max(TexAlpha, 0);
}

void Field::Draw()
{
	CelestialSphereObject->PreDraw();
	CelestialSphereObject->Draw();
	CelestialSphereObject->PostDraw();

	FieldObject->PreDraw();
	FieldObject->Draw();
	FieldObject->PostDraw();
	if (SceneManager::GetInstance()->GetScene() == SceneManager::BOSS) {
		BackObject->PreDraw();
		BackObject->Draw();
		BackObject->PostDraw();

		DamageAreaObj->PreDraw();
		DamageAreaObj->Draw();
		DamageAreaObj->PostDraw();
	}

	Explanation->setcolor({ 1,1,1,TexAlpha });
}

void Field::WarningDraw()
{
	Sprite::PreDraw();
	Explanation->Draw();
	Sprite::PostDraw();
}