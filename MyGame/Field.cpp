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
#include "mHelper.h"
#define PI 3.14f
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

void Field::Setplay()
{
}

void Field::Initialize()
{
	DebugCamera* camera = CameraControl::GetInstance()->GetCamera();

	PedestalDownF = false;
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

		FieldObject = TouchableObject::Create(ModelManager::GetIns()->GetModel(ModelManager::SANDFIELD), camera);

	}

	if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY || SceneManager::GetInstance()->GetScene() ==
		SceneManager::TUTORIAL)
	{
		FieldObject = TouchableObject::Create(ModelManager::GetIns()->GetModel(ModelManager::FIELD), camera);
		//ミニマップ(通常ふぃ－るど)
		//天球
		SetFieldModel(CELESTIALSPHERE, ModelManager::GetIns()->GetModel(ModelManager::CELESTIALSPHERE), camera);
	}

	

	if (SceneManager::GetInstance()->GetScene() == SceneManager::MAPCREATE)
	{
		FieldObject = TouchableObject::Create(ModelManager::GetIns()->GetModel(ModelManager::FIELD), camera);
	}
	if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY)
	{
		pedestalpos = { -300.0f, -32, 270 };
		Texture::LoadTexture(12, L"Resources/2d/enemy/GuardArea.png");
		Texture* l_tex[8];
		for (int i = 0; i < 8; i++) {
			l_tex[i] = Texture::Create(12);
			GuardArea[i].reset(l_tex[i]);
			GuardArea[i]->CreateTexture();
			GuardArea[i]->SetAnchorPoint({ 0.5f,0.5f });

			GuardAreaAngle[i] = static_cast<float>(i) * 45.f;
			//位置の初期化
			GuardareaPos[i].x = pedestalpos.x + sinf(GuardAreaAngle[i] * (PI / 180.0f)) * 80.0f;
			GuardareaPos[i].z = pedestalpos.z + cosf(GuardAreaAngle[i] * (PI / 180.0f)) * 80.0f;

		}
		SetFieldModel(PEDESTAL, ModelManager::GetIns()->GetModel(ModelManager::ICECRYSTAL), camera);
		SetFieldModel(BOSSBACK, ModelManager::GetIns()->GetModel(ModelManager::BOSSFIELD), camera);
	}
	//ボスのネームプレート
	BossName = Sprite::Create(40, {WinApp::window_width / 2, WinApp::window_height / 2});
	BossName->SetAnchorPoint({0.5f, 0.5f});
	BossName->SetPosition({WinApp::window_width / 2, WinApp::window_height / 2});
	BossName->SetSize({800, 800});
}

void Field::PedestalMoving()
{
	if (Collision::GetLength(PlayerControl::GetInstance()->GetPlayer()->GetPosition(),
	                         pedestalpos) < 10)
	{
		PedestalDownF = true;
	}
	if (PedestalDownF)
	{
		if (pedestalpos.y > -200)
		{
			pedestalpos.y -= 0.2f;
		}
	}
}

void Field::Update_Tutorial()
{
	DebugCamera* camera = CameraControl::GetInstance()->GetCamera();

	if (FieldObject == nullptr)
	{
		return;
	}
	FieldObject->SetPosition({0.0f, -25.0f, 0.0f});
	FieldObject->SetFogCenter({125.0f, -25.0f, -680.0f});
	FieldObject->setFog(TRUE);
	SetFieldUpdate(CELESTIALSPHERE, camera, {0.0f, 30.0f, 0.0f}, {40.0f, 40.0f, 40.0f}, FALSE, TRUE);
	FieldObject->SetColor({0.2f, 0.2f, 0.2f, 1.0f});
	FieldObject->Update({0.2f, 0.2f, 0.2f, 1.0f}, camera);
}

void Field::Update_Play()
{
	DebugCamera* camera = CameraControl::GetInstance()->GetCamera();
	XMFLOAT3 ppos = PlayerControl::GetInstance()->GetPlayer()->GetPosition();

	if (CameraControl::GetInstance()->GetCameraState_Spline() != CameraControl::PLAYCUTEND)
	{
		if (camera != nullptr)
		{
			FogCenterPos = camera->GetEye();
		}
	}
	else
	{
		FogCenterPos = PlayerControl::GetInstance()->GetPlayer()->GetPosition();
	}

	if (FieldObject == nullptr)
	{
		return;
	}
	CelestalRot += 0.1f;
	m_object[CELESTIALSPHERE]->SetRotation({90.0f, CelestalRot, 180.0f});
	SetFieldUpdate(CELESTIALSPHERE, camera, {0.0f, 2290.0f, 0.0f}, {30.0f, 30.0f, 30.0f}, FALSE, TRUE);
	SetFieldUpdate(PEDESTAL, camera, pedestalpos, {2, 2, 2});

	FieldObject->SetPosition({0.0f, -25.0f, 0.0f});
	FieldObject->SetColor({0.2f, 0.2f, 0.2f, 1.0f});
	FieldObject->SetFogCenter(FogCenterPos);
	FieldObject->setFog(true);
	FieldObject->Update({0.2f, 0.2f, 0.2f, 1.0f}, camera);

	m_object[BOSSBACK]->SetRotation({0.f, 180.f, 0.f });
	SetFieldUpdate(BOSSBACK, camera, {22.f, -70.f, 1010.f }, {1.0f, 1.0f, 1.0f}, FALSE, TRUE);

	GuardAreaTexUpda();
	PedestalMoving();
}

void Field::GuardAreaTexUpda()
{
	if (EnemyControl::GetInstance()->GetGuardianEnemy() == nullptr)return;
	DebugCamera* camera = CameraControl::GetInstance()->GetCamera();
	XMFLOAT3 ppos = PlayerControl::GetInstance()->GetPlayer()->GetPosition();

	XMVECTOR positionA = {
	pedestalpos.x,
	pedestalpos.y,
	pedestalpos.z
	};
	XMVECTOR positionB[8];
	//プレイヤーと敵のベクトルの長さ(差)を求める
	XMVECTOR SubVector[8];
	//調整用
	float RotY[8];
	//カメラからプレイヤーにかけての線分
	camera_to_player.start = { camera->GetEye().x,camera->GetEye().z };
	camera_to_player.end = { ppos.x,ppos.z };

	//角度の取得 プレイヤーが敵の索敵位置に入ったら向きをプレイヤーの方に
	for (int i = 0; i < 8; i++)
	{
		positionB[i] = {GuardareaPos[i].x,GuardareaPos[i] .y,GuardareaPos[i] .z};
		SubVector[i] = XMVectorSubtract(positionB[i], positionA); // positionA - positionB;
		RotY[i] = atan2f(SubVector[i].m128_f32[0], SubVector[i].m128_f32[2]);

		//テクスチャ回転用アングル
		GuardAreaAngle[i]++;
		//移動制限テクスチャの座標
		GuardareaPos[i].y = -30.0f;
		GuardareaPos[i].x = pedestalpos.x + sinf(GuardAreaAngle[i] * (PI / 180.0f)) * 80.0f;
		GuardareaPos[i].z = pedestalpos.z + cosf(GuardAreaAngle[i] * (PI / 180.0f)) * 80.0f;
		//移動制限テクスチャの回転値
		GuardAreaRot[i].y = RotY[i]*60;
		GuardAreaRot[i].x = 180.f;

		//衝突判定用ーー点
		GuardAreaPoint[i].x = GuardareaPos[i].x;
		GuardAreaPoint[i].y = GuardareaPos[i].z;
	

		//ガーディアン死んだら壁のアルファ値消していく
		if(EnemyControl::GetInstance()->GetGuardianEnemy()->GetisAlive()==false)
		{
			GuardAreaAlphaEtime[i] -= 0.05f;
		}
		else//それ以外
		{
			//カメラ座標からプレイヤーの方へ線分設定して
			if (Collision::IsCollidingLineAndCircle(camera_to_player,GuardAreaPoint[i]))
			{
				//当たったらアルファ値下げる
				GuardAreaAlphaEtime[i] -= 0.05f;
			}
			else
			{
				//当たってないなら上げる
				if (EnemyControl::GetInstance()->GetGuardianEnemy()->GetisAlive()) {
					GuardAreaAlphaEtime[i] += 0.05f;
				}
			}
			for (int i = 0; i < 8; i++)
			{
				//プレイヤーの移動制限設定
				if (Collision::GetLength(ppos, GuardareaPos[i]) <= 20.f)
				{
					PlayerControl::GetInstance()->GetPlayer()->isOldPos();
				
				}

			}
		}
		
		//アルファ値の上下
		GuardAreaAlpha[i] = Easing::EaseOut(GuardAreaAlphaEtime[i], 0.0f, 0.8f);

		//各パラメータの更新処理
		GuardArea[i]->SetColor({ 1.f,1.f,1.f,GuardAreaAlpha[i] });
		GuardArea[i]->SetPosition(GuardareaPos[i]);
		GuardArea[i]->SetRotation(GuardAreaRot[i]);
		GuardArea[i]->SetScale({ 6.f,6.f,4.f });
		GuardArea[i]->SetBillboard(FALSE);
		GuardArea[i]->Update(camera);

		//アルファ値の上限と下限
		GuardAreaAlphaEtime[i] = min(GuardAreaAlphaEtime[i], 1.0f);
		GuardAreaAlphaEtime[i] = max(GuardAreaAlphaEtime[i], 0.0f);
	}
}

void Field::Update_Edit()
{
	DebugCamera* camera = CameraControl::GetInstance()->GetCamera();

	FieldObject->SetPosition({0.0f, -25.0f, 0.0f});
	FieldObject->SetFogCenter({0.0f, -20.0f, 0.0f});
	FieldObject->SetColor({0.2f, 0.2f, 0.2f, 1.0f});
	FieldObject->Update({0.2f, 0.2f, 0.2f, 1.0f}, camera);
}

void Field::Update_Boss()
{
	DebugCamera* camera = CameraControl::GetInstance()->GetCamera();

	if (FieldObject == nullptr)
	{
		return;
	}
	SpriteFeed(TexAlpha_BossName, feed_BossName, feedSpeed_BossName, 1.5f);
	if (CameraControl::GetInstance()->GetCameraState() == CameraControl::PLAYER)
	{
		SpriteFeed(t, feed, feedSpeed_Explanation, 2.5f);
	}

	//天球オブジェクト更新
	SetFieldUpdate(CELESTIALSPHERE, camera, {0.0f, 30.0f, 0.0f}, {40.0f, 40.0f, 40.0f}, FALSE, TRUE);
	//外周ダメージエリア更新
	SetFieldUpdate(DAMAGEAREA, camera, {0.0f, -19.2f, 0.0f}, {1.0f, 1.0f, 1.0f}, TRUE, FALSE);
	//背景のコロシアム更新
	SetFieldUpdate(BOSSBACK, camera, {0, -19, 0}, {1.0f, 1.0f, 1.0f}, FALSE, TRUE);

	FieldObject->SetPosition({0.0f, -19.0f, 0.0f});
	FieldObject->SetFogCenter(camera->GetEye());
	FieldObject->setFog(TRUE);
	FieldObject->SetColor({0.8f, 0.8f, 0.8f, 1.0f});
	FieldObject->Update({0.8f, 0.8f, 0.8f, 1.0f}, camera);

	//フィールド外周とプレイヤーの当たり判定(現時点では矩形と点)
	//FieldDamageAreaCol();
}

void Field::Update()
{
	DebugCamera* camera = CameraControl::GetInstance()->GetCamera();

	if (SceneManager::GetInstance()->GetScene() == SceneManager::TUTORIAL)
	{
		Update_Tutorial();
	}
	if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY)
	{
		Update_Play();
	}
	if (SceneManager::GetInstance()->GetScene() == SceneManager::BOSS)
	{
		Update_Boss();
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
	if (FieldObject == nullptr)
	{
		return;
	}
	/*ImGui::Begin("field");
	ImGui::SliderFloat("PosX", &BossFieldPos.x, -700, 700);
	ImGui::SliderFloat("Posy", &BossFieldPos.y, -70, 70);

	ImGui::SliderFloat("Posz", &BossFieldPos.z, 0, 1700);
	ImGui::End();
	*/ //	if (FieldObject == nullptr)return;
	Object3d::PreDraw();
	if (SceneManager::GetInstance()->GetScene() != SceneManager::MAPCREATE)
	{
		m_object[CELESTIALSPHERE]->Draw();
	}


	FieldObject->Draw();
	if (SceneManager::GetInstance()->GetScene() == SceneManager::BOSS)
	{
		ModelDraw_nullCheck(BOSSBACK);
		ModelDraw_nullCheck(DAMAGEAREA);
	}
	if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY)
	{
		ModelDraw_nullCheck(PEDESTAL);
		ModelDraw_nullCheck(BOSSBACK);
		
	}
	Object3d::PostDraw();
	if (SceneManager::GetInstance()->GetScene() == SceneManager::BOSS)
	{
		BossName->setcolor({1.0f, 1.0f, 1.0f, TexAlpha_BossName});
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
	}
	else
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
	m_object[type]->Update({1.0f, 1.0f, 1.0f, 1.0f}, camera);
}

void Field::ModelDraw_nullCheck(ObjType type)
{
	if (m_object[type] == nullptr)
	{
		return;
	}
	m_object[type]->Draw();
}

void Field::GuardAreaDraw()
{
	
		Texture::PreDraw();
		for (int i = 0; i < 8; i++)
		{
			if (GuardArea[i] == nullptr)continue;
			GuardArea[i]->Draw();
		}
		Texture::PostDraw();
	
}
