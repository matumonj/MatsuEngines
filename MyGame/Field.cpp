#include "Field.h"
#include"TouchableObject.h"
#include"CollisionManager.h"
#include"SceneManager.h"
#include"ModelManager.h"
#include"Destroy.h"
#include"CollisionPrimitive.h"
#include"PlayerControl.h"
Field::~Field()
{
	
}

void Field::Finalize()
{delete CelestialSphereModel;
	Destroy_unique(CelestialSphereObject);
	delete FieldObject, FieldModel;
	delete BackM;
	Destroy_unique(BackObject);

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
	//Bossscene用の背景オブジェ
	BackObject= std::make_unique<Object3d>();
	BackM = Model::CreateFromOBJ("BackGround");
	//Bossscene用のフィールドの外周ダメージエリア
	DamageAreaObj = std::make_unique<Object3d>();
	
	if (SceneManager::GetInstance()->GetScene() == SceneManager::BOSS) {
		FieldModel = ModelManager::GetIns()->GetModel(ModelManager::BOSSFIELD);
		DamageAreaModel = Model::CreateFromOBJ("BossFieldDamageArea");

		BackObject->Initialize(camera);
		BackObject->SetModel(BackM);

		DamageAreaObj->Initialize(camera);
		DamageAreaObj->SetModel(DamageAreaModel);

		//Bossの名前表示用とフィールド外周のダメージエリア通知
		Sprite::LoadTexture(40, L"Resources/BossName.png");
		Sprite::LoadTexture(41, L"Resources/bossName.png");
	}
	else {
		FieldModel = ModelManager::GetIns()->GetModel(ModelManager::FIELD);
	}

	//フィールドにモデル割り当て
	FieldObject = TouchableObject::Create(FieldModel,camera);

	CelestialSphereObject->Initialize(camera);
	CelestialSphereObject->SetModel(CelestialSphereModel);

	Explanation = Sprite::Create(40, { WinApp::window_width / 2,WinApp::window_height / 2 });
	Explanation->SetAnchorPoint({ 0.5f,0.5f });
	Explanation->SetPosition({ WinApp::window_width / 2,WinApp::window_height / 2  });
	Explanation->SetSize({ 800,800 });
	return true;
}

void Field::Update(DebugCamera* camera)
{
	CelestialSphereObject->SetPosition({ 0.0f,30.0f,0.0f });
	CelestialSphereObject->SetScale({ 40.0f,40.0f,40.0f });

	BackObject->SetPosition({ 0.0f,30.0f,0.0f });
	BackObject->SetScale({ 0.5f,0.5f,0.5f });

	FieldObject->SetScale(1.0f);
	FieldObject->SetPosition({ 0.0f,-20.0f,0.0f });

	if (SceneManager::GetInstance()->GetScene() == SceneManager::TUTORIAL) {
		FieldObject->SetFogCenter({ 125.0f, -25.0f, -680.0f });
		FieldObject->setFog(TRUE);
		CelestialSphereObject->setFog(TRUE);
	}
	else if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY) {
		FieldObject->SetFogCenter({ 125.0f, -25.0f, -680.0f });
		FieldObject->setFog(FALSE);
		CelestialSphereObject->setFog(FALSE);
	}
	else if (SceneManager::GetInstance()->GetScene() == SceneManager::BOSS) {
		if (feed) {
			TexAlpha -= 0.02f;
		}
		else {
			TexAlpha += 0.005f;
		}
		if (TexAlpha >= 1.5f) {
			feed = true;
		}
		FieldObject->SetFogCenter({ 0.0f, -20.0f, 0.0f });

		CelestialSphereObject->setFog(TRUE);
	
		DamageAreaObj->setFog(FALSE);

		BackObject->setFog(TRUE);
		BackObject->Update({ 0.6f,0.6f,0.6f,1.0f }, camera);

		DamageAreaObj->SetUVf(true);
		DamageAreaObj->SetPosition({ 0.0f,-19.2f,0.0f });
		DamageAreaObj->Update({ 0.6f,0.6f,0.6f,1.0f }, camera);

		if (PlayerControl::GetInstance()->GetPlayer() != nullptr && TexAlpha <= 0.2f) {
			Box damagearea;
			Box player;
			 Ppos = PlayerControl::GetInstance()->GetPlayer()->GetPosition();
			damagearea.LBposition = { -60.0f,-60.0f };
			damagearea.LUposition = { -60.0f,60.0f };
			damagearea.RUposition = { 60.0f,60.0f };
			damagearea.RBposition = { 60.0f,-60.0f };

			player.LBposition = { Ppos.x - 1.0f,Ppos.z - 1.0f };
			player.LUposition = { Ppos.x - 1.0f,Ppos.z + 1.0f };
			player.RBposition = { Ppos.x + 1.0f,Ppos.z + 1.0f };
			player.RUposition = { Ppos.x + 1.0f,Ppos.z - 1.0f };

			if (Collision::CheckBox2Box( player,damagearea) == false) {
				PlayerControl::GetInstance()->GetPlayer()->RecvDamage(10);
			}
		}
	}

	FieldObject->SetColor({ 0.6f,0.6f,0.6f,1.0f });
	FieldObject->Update({ 0.6f,0.6f,0.6f,1.0f }, camera);
	
	CelestialSphereObject->Update({ 1.0f,1.0f,1.0f,1.0f }, camera); 

	
	TexAlpha = min(TexAlpha, 1.5f);
	TexAlpha = max(TexAlpha, 0.0f);
}

void Field::Draw()
{
	Object3d::PreDraw();
	CelestialSphereObject->Draw();
	
	FieldObject->Draw();
	
	if (SceneManager::GetInstance()->GetScene() == SceneManager::BOSS) {
		BackObject->Draw();
		
		DamageAreaObj->Draw();
		}
	Object3d::PostDraw();
	Explanation->setcolor({ 1.0f,1.0f,1.0f,TexAlpha });
}

void Field::WarningDraw()
{
	Sprite::PreDraw();
	Explanation->Draw();
	Sprite::PostDraw();
}