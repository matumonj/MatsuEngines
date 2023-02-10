#include "Field.h"
#include <algorithm>

#include "BossMap.h"
#include"CameraControl.h"
#include"TouchableObject.h"
#include"CollisionManager.h"
#include"SceneManager.h"
#include"ModelManager.h"
#include"Destroy.h"
#include"CollisionPrimitive.h"
#include"PlayerControl.h"
#include"EnemyControl.h"
#include "mHelper.h"
#include"UltAttack.h"
#include "Feed.h"
#define PI 3.14f

Field::~Field()
{
	Destroy_unique(m_object[BOSSBACK]);
	for(int i=0;i<SkyBack.size();i++)
	{
		Destroy_unique(SkyBack[i]);
	}
	for(int i=0;i<SkyMap.size();i++)
	{
		Destroy_unique(SkyMap[i]);
	}
	Destroy_unique(m_object[CELESTIALSPHERE]);
	Destroy_unique(m_object[PEDESTAL]);
	Destroy(FieldObject);
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

Field* Field::GetIns()
{
	static Field instance;
	return &instance;
}

void Field::Init_Tutorial()
{
	DebugCamera* camera = CameraControl::GetIns()->GetCamera();

	FieldObject = TouchableObject::Create(ModelManager::GetIns()->GetModel(ModelManager::FIELD), camera);
	//ミニマップ(通常ふぃ－るど)
	//天球
	SetFieldModel(CELESTIALSPHERE, ModelManager::GetIns()->GetModel(ModelManager::SKY), camera);

}

void Field::Init_Play()
{
	DebugCamera* camera = CameraControl::GetIns()->GetCamera();

	FieldObject = TouchableObject::Create(ModelManager::GetIns()->GetModel(ModelManager::FIELD), camera);
	//ミニマップ(通常ふぃ－るど)
	//天球
	SetFieldModel(CELESTIALSPHERE, ModelManager::GetIns()->GetModel(ModelManager::SKY), camera);

	//ガーディアン出現時の移動制限テクスチャ
	Texture::LoadTexture(29, L"Resources/2d/enemy/GuardArea.png");
	Texture* l_tex[GuardAreaSize];
	//初期化
	for (int i = 0; i < GuardAreaSize; i++)
	{
		l_tex[i] = Texture::Create(29);
		GuardArea[i].reset(l_tex[i]);
		GuardArea[i]->CreateTexture();
		GuardArea[i]->SetAnchorPoint({ 0.5f, 0.5f });

		GuardAreaAngle[i] = static_cast<float>(i) * (360.f / static_cast<float>(GuardAreaSize));
		//位置の初期化
		GuardareaPos[i].x = pedestalpos.x + sinf(GuardAreaAngle[i] * (PI / 180.0f)) * 80.0f;
		GuardareaPos[i].z = pedestalpos.z + cosf(GuardAreaAngle[i] * (PI / 180.0f)) * 80.0f;
	}
	//ガーディアン出現用の塔
	pedestalpos = { -300.0f, -32, 270 };

	SetFieldModel(PEDESTAL, ModelManager::GetIns()->GetModel(ModelManager::ICECRYSTAL), camera);

	//ボスの背景obj
	SetFieldModel(BOSSBACK, ModelManager::GetIns()->GetModel(ModelManager::BOSSFIELD), camera);

	
}

void Field::Init_Boss()
{
	DebugCamera* camera = CameraControl::GetIns()->GetCamera();
	if (camera == nullptr)return;
	//天球
	SetFieldModel(CELESTIALSPHERE, ModelManager::GetIns()->GetModel(ModelManager::SKY), camera);
	//ボスの背景obj
	SetFieldModel(BOSSBACK, ModelManager::GetIns()->GetModel(ModelManager::BOSSFIELD), camera);
	//ボス床のまっぷちっぷ
	BossMap::GetIns()->Init();

	//クリア時の移動誘導テクスチャ
	Texture::LoadTexture(67, L"Resources/2d/tutorialstep/targetpos.png");
	Texture* l_tex = Texture::Create(67);
	cleartex.reset(l_tex);
	cleartex->CreateTexture();
	cleartex->SetAnchorPoint({ 0.5f, 0.5f });

	//銅像
	for (int i = 0; i < SkyMap.size(); i++) {
		SkyMap[i].reset(new Object3d());
		SkyMap[i]->Initialize(camera);
		SkyMap[i]->SetModel(ModelManager::GetIns()->GetModel(ModelManager::SKYMAP));
		SkyMap[i]->SetScale({ 1,1,1 });
		SkyMap[i]->SetRotation({ 0.f,float(i) * -90.f,0.f });
	}
	SkyMap[0]->SetPosition({ 0.0f,0.0f,-200.0f });
	SkyMap[1]->SetPosition({ 200.0f,0.0f,0.0f });
	SkyMap[2]->SetPosition({ 0.0f,0.0f,200.0f });
	SkyMap[3]->SetPosition({ -200.0f,0.0f,0.0f });

	//塔
	for (int i = 0; i < SkyBack.size(); i++) {
		SkyBack[i].reset(new Object3d());
		SkyBack[i]->Initialize(camera);
		SkyBack[i]->SetModel(ModelManager::GetIns()->GetModel(ModelManager::TOWER));
		SkyBack[i]->SetScale({ 8,10,8 });
	}
	SkyBackPos[0] = { -150.f,-100.f,140.f };
	SkyBackPos[1] = { 87.f,-100.f,160.f };
	SkyBackPos[2] = { 160.f,-100.f,-150.f };
	SkyBackPos[3] = { -150.f,-100.f,-150.f };
	//下地
	FieldObject = TouchableObject::Create(ModelManager::GetIns()->GetModel(ModelManager::SANDFIELD), camera);
	Sprite::LoadTexture(40, L"Resources/2d/icon/BossName.png");
	//ボスのネームプレート
	BossName = Sprite::Create(40, { WinApp::window_width / 2, WinApp::window_height / 2 });
	BossName->SetAnchorPoint({ 0.5f, 0.5f });
	BossName->SetPosition({ WinApp::window_width / 2, WinApp::window_height / 2 });
	BossName->SetSize({ 800, 800 });

}



void Field::Initialize()
{
	DebugCamera* camera = CameraControl::GetIns()->GetCamera();

	PedestalDownF = false;
	if (SceneManager::GetIns()->GetScene() == SceneManager::BOSS)
	{
		Init_Boss();
	}
	if ( SceneManager::GetIns()->GetScene() ==SceneManager::TUTORIAL)
	{
		Init_Tutorial();
	}
	if (SceneManager::GetIns()->GetScene() == SceneManager::PLAY)
	{
		Init_Play();
	}

	if (SceneManager::GetIns()->GetScene() == SceneManager::MAPCREATE)
	{
		FieldObject = TouchableObject::Create(ModelManager::GetIns()->GetModel(ModelManager::FIELD), camera);
	//	FieldObject->SetScale(0.3f);
	}
	

}

void Field::PedestalMoving()
{
	//if (PlayerControl::GetIns()->GetPlayer()->GetHP() <= 0)return;
	if (Task::GetIns()->TaskThirdClear())
	{
		if (Collision::GetLength(PlayerControl::GetIns()->GetPlayer()->GetPosition(),
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

		if (PlayerControl::GetIns()->GetPlayer()->GetHP() <= 0) {
		PedestalDownF = false;
		pedestalpos = { -300.0f, -32, 270 };
	}
}

void Field::Update_Tutorial()
{
	DebugCamera* camera = CameraControl::GetIns()->GetCamera();

	if (FieldObject == nullptr)
	{
		return;
	}
	FieldObject->SetPosition({0.0f, -25.0f, 0.0f});
	FieldObject->SetFogCenter({camera->GetEye()});
	FieldObject->setFog(TRUE);
	SetFieldUpdate(CELESTIALSPHERE, camera, {0.0f, 30.0f, 0.0f}, {40.0f, 40.0f, 40.0f}, FALSE, TRUE);
	FieldObject->SetColor({0.2f, 0.2f, 0.2f, 1.0f});
	FieldObject->Update({0.2f, 0.2f, 0.2f, 1.0f}, camera);
}

void Field::Update_Play()
{
	DebugCamera* camera = CameraControl::GetIns()->GetCamera();
	XMFLOAT3 ppos = PlayerControl::GetIns()->GetPlayer()->GetPosition();

	if (CameraControl::GetIns()->GetCameraState_Spline() != CameraControl::PLAYCUTEND)
	{
		if (camera != nullptr)
		{
			FogCenterPos = camera->GetEye();
		}
	}
	else
	{
		FogCenterPos = PlayerControl::GetIns()->GetPlayer()->GetPosition();
	}

	if (FieldObject == nullptr)
	{
		return;
	}
	CelestalRot += 0.1f;
	m_object[CELESTIALSPHERE]->SetRotation({90.0f, CelestalRot, 180.0f});
	SetFieldUpdate(CELESTIALSPHERE, camera, {0.0f, 2290.0f, 0.0f}, {30.0f, 30.0f, 30.0f}, FALSE, TRUE);
	SetFieldUpdate(PEDESTAL, camera, pedestalpos, {2, 2, 2});

	FieldObject->Setf(true);
	FieldObject->SetPosition({0.0f, -25.0f, 0.0f});
	FieldObject->SetColor({0.2f, 0.2f, 0.2f, 1.0f});
	FieldObject->SetFogCenter(FogCenterPos);
	FieldObject->setFog(true);
	FieldObject->Update({0.2f, 0.2f, 0.2f, 1.0f}, camera);

	m_object[BOSSBACK]->SetRotation({0.f, 180.f, 0.f});
	SetFieldUpdate(BOSSBACK, camera, {22.f-50.f, -70.f, 1010.f-500.f}, {0.6f, 0.6f, 0.6f}, FALSE, true);

	GuardAreaTexUpda();
	PedestalMoving();
}

void Field::GuardAreaTexUpda()
{
	if (EnemyControl::GetIns()->GetGuardianEnemy() == nullptr)
	{
		return;
	}
	Input* input = Input::GetIns();
	DebugCamera* camera = CameraControl::GetIns()->GetCamera();
	XMFLOAT3 ppos = PlayerControl::GetIns()->GetPlayer()->GetPosition();

	XMVECTOR positionA = {
		pedestalpos.x,
		pedestalpos.y,
		pedestalpos.z
	};
	XMVECTOR positionB[GuardAreaSize];
	//プレイヤーと敵のベクトルの長さ(差)を求める
	XMVECTOR SubVector[GuardAreaSize];
	//調整用
	float RotY[GuardAreaSize];
	//カメラからプレイヤーにかけての線分
	camera_to_player.start = {camera->GetEye().x, camera->GetEye().z};
	camera_to_player.end = {ppos.x, ppos.z};

	//角度の取得 プレイヤーが敵の索敵位置に入ったら向きをプレイヤーの方に
	for (int i = 0; i < GuardAreaSize; i++)
	{
		if (GuardArea[i] == nullptr)
		{
			continue;
		}
		positionB[i] = {GuardareaPos[i].x, GuardareaPos[i].y, GuardareaPos[i].z};
		SubVector[i] = XMVectorSubtract(positionB[i], positionA); // positionA - positionB;
		RotY[i] = atan2f(SubVector[i].m128_f32[0], SubVector[i].m128_f32[2]);

		//テクスチャ回転用アングル
		GuardAreaAngle[i]++;
		//移動制限テクスチャの座標
		GuardareaPos[i].y = -30.0f;
		GuardareaPos[i].x = pedestalpos.x + sinf(GuardAreaAngle[i] * (PI / 180.0f)) * 80.0f;
		GuardareaPos[i].z = pedestalpos.z + cosf(GuardAreaAngle[i] * (PI / 180.0f)) * 80.0f;
		//移動制限テクスチャの回転値
		GuardAreaRot[i].y = RotY[i] * 60;
		GuardAreaRot[i].x = 180.f;

		//衝突判定用ーー点
		GuardAreaPoint[i].x = GuardareaPos[i].x;
		GuardAreaPoint[i].y = GuardareaPos[i].z;


		//ガーディアン死んだら壁のアルファ値消していく
		if (EnemyControl::GetIns()->GetGuardianEnemy()->GetisAlive() == false)
		{
			GuardAreaAlphaEtime[i] -= 0.05f;
		}
		else //それ以外
		{
			//カメラ座標からプレイヤーの方へ線分設定して
			if (Collision::IsCollidingLineAndCircle(camera_to_player, GuardAreaPoint[i],30.f))
			{
				//当たったらアルファ値下げる
				GuardAreaAlphaEtime[i] -= 0.05f;
			}
			else
			{
				//当たってないなら上げる
				if (EnemyControl::GetIns()->GetGuardianEnemy()->GetisAlive())
				{
					GuardAreaAlphaEtime[i] += 0.05f;
				}
			}
			for (int i = 0; i < GuardAreaSize; i++)
			{
				if (GuardArea[i] == nullptr)
				{
					continue;
				}
				//プレイヤーの移動制限設定
				if (Collision::GetLength(ppos, GuardareaPos[i]) <= 20.f)
				{
					if (input->TiltPushStick(Input::L_UP, 0.0f) ||
						input->TiltPushStick(Input::L_DOWN, 0.0f) ||
						input->TiltPushStick(Input::L_RIGHT, 0.0f) ||
						input->TiltPushStick(Input::L_LEFT, 0.0f))
					{
						if (Collision::IsCollidingLineAndCircle(camera_to_player, GuardAreaPoint[i],30.f))
						{
							GuardAreaAlphaEtime[i] = 1.f;
						}
					}
						PlayerControl::GetIns()->GetPlayer()->isOldPos();
					}
				}
		}

		//アルファ値の上下
		GuardAreaAlpha[i] = Easing::EaseOut(GuardAreaAlphaEtime[i], 0.0f, 0.8f);

		//各パラメータの更新処理
		GuardArea[i]->SetColor({1.f, 1.f, 1.f, GuardAreaAlpha[i]});
		GuardArea[i]->SetPosition(GuardareaPos[i]);
		GuardArea[i]->SetRotation(GuardAreaRot[i]);
		GuardArea[i]->SetScale({6.f, 6.f, 4.f});
		GuardArea[i]->SetBillboard(FALSE);
		GuardArea[i]->Update(camera);

		//アルファ値の上限と下限
		GuardAreaAlphaEtime[i] = min(GuardAreaAlphaEtime[i], 1.0f);
		GuardAreaAlphaEtime[i] = max(GuardAreaAlphaEtime[i], 0.0f);
	}
	if (EnemyControl::GetIns()->GetGuardianEnemy() == nullptr)
	{
		for (int i = 0; i < GuardAreaSize; i++)
		{
			if (GuardAreaAlpha[i] > 0.0f)
			{
				continue;
			}
			if (GuardArea[i] == nullptr)
			{
				continue;
			}
			Destroy_unique(GuardArea[i]);
		}
	}
}

void Field::Update_Edit()
{
	DebugCamera* camera = CameraControl::GetIns()->GetCamera();

	FieldObject->SetPosition({0.0f, -25.0f, 0.0f});
	FieldObject->SetFogCenter({0.0f, -20.0f, 0.0f});
	FieldObject->SetColor({0.8f, 0.8f, 0.8f, 1.0f});
	FieldObject->Update({0.2f, 0.2f, 0.2f, 1.0f}, camera);
}

void Field::Update_Boss()
{
	DebugCamera* camera = CameraControl::GetIns()->GetCamera();
	if (camera == nullptr)return;
	if (FieldObject == nullptr)
	{
		return;
	}

	CelestalRot += 0.1f;
	m_object[CELESTIALSPHERE]->SetRotation({ 0.0f, CelestalRot, 0.0f });
	SetFieldUpdate(CELESTIALSPHERE, camera, { 0.0f, 30.0f, 0.0f }, { celestalscl, celestalscl, celestalscl }, FALSE, TRUE);
	SpriteFeed(TexAlpha_BossName, feed_BossName, feedSpeed_BossName, 1.5f);
	TexAlpha_BossName = min(TexAlpha_BossName, 3.0f);
	TexAlpha_BossName = max(TexAlpha_BossName, 0.0f);
	//背景のコロシアム更新
	if (KoloiamAlpha <= 0.0f)
	{
		SkyMapDestF = true;
		destf = true;
	}
	if (Input::GetIns()->TriggerButton(Input::X))
	{
		destf = false;
	}

	if ( UltAttack::GetIns()->GetFieldDestG())
	{
		for (int i = 0; i < SkyBack.size(); i++)
		{
			SkyBackEaseT[i] += 0.02f;
			SkyBackEaseT[i] = std::clamp(SkyBackEaseT[i], 0.f, 1.f);

			SkyBackPos[i].y = Easing::EaseOut(SkyBackEaseT[i], -100.f, 0.f);
		}


		KoloiamAlpha -= 0.01f;
		destt += 5.1f;
		DestCenter.x += 1.1f;
	}

	KoloiamAlpha = std::clamp(KoloiamAlpha, 0.f, 1.f);
	if (!nextStageF) {
		//外周ダメージエリア更新
		
		m_object[BOSSBACK]->SetDestFlag(true);
		m_object[BOSSBACK]->SetDestTime(destt);
		m_object[BOSSBACK]->SetCenter(DestCenter);
		m_object[BOSSBACK]->Setf(true);
		m_object[BOSSBACK]->SetColor({ 0.4f,0.4f,0.4f,1.0f });
		SetFieldUpdate(BOSSBACK, camera, { 0, -19, 0 }, { 1.0f, 1.0f, 1.0f }, false, false);
	}
	FieldObject->SetPosition({ 0.0f, -19.0f, 0.0f });

	FieldObject->SetColor({ 0.8f, 0.8f, 0.8f, KoloiamAlpha });
	FieldObject->Update({ 0.8f, 0.8f, 0.8f, 1.0f }, camera);

	if (SkyMapDestF)
	{
		SkyMapDestT -= 4;
	}
	if (SkyMapDestT < 0)
	{
		SkyMapDestT = 0.f;
	}
	for (int i = 0; i < SkyMap.size(); i++) {
		SkyMap[i]->SetDestFlag(true);
		SkyMap[i]->SetDestTime(SkyMapDestT);
		SkyMap[i]->setFog(true);
		SkyMap[i]->SetFogCenter(camera->GetEye());
		SkyMap[i]->Update({ 1,1,1,1 }, camera);
	}
	for (int i = 0; i < SkyBack.size(); i++) {
		objroty[i] = float(i) * 90.f;
		SkyBack[i]->SetDestFlag(true);
		SkyBack[i]->SetDestTime(SkyMapDestT);
		SkyBack[i]->setFog(true);
		SkyBack[i]->SetFogCenter(camera->GetEye());
		SkyBack[i]->SetPosition(SkyBackPos[i]);
		SkyBack[i]->SetRotation({ 0.f,objroty[i],0.f });
		SkyBack[i]->Update({ 1,1,1,1 }, camera);
	}cleartex->SetPosition({ 0,17,75 });
	cleartex->SetRotation({ 90,0,0 });
	cleartex->SetScale({ 3,3,3 });
	cleartex->SetBillboard(false);
	cleartex->SetColor({ 1,1,1,1 });
	cleartex->Update(camera);
	
	BossMap::GetIns()->Upda();
}

void Field::Update()
{
	DebugCamera* camera = CameraControl::GetIns()->GetCamera();

	if (SceneManager::GetIns()->GetScene() == SceneManager::TUTORIAL)
	{
		Update_Tutorial();
	}
	if (SceneManager::GetIns()->GetScene() == SceneManager::PLAY)
	{
		Update_Play();
	}
	if (SceneManager::GetIns()->GetScene() == SceneManager::BOSS)
	{
		Update_Boss();
	}

	t = min(t, 2.5f);
	t = max(t, 0.0f);
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

	
	 //	if (FieldObject == nullptr)return;
	Object3d::PreDraw();
	if (SceneManager::GetIns()->GetScene() != SceneManager::MAPCREATE)
	{
		m_object[CELESTIALSPHERE]->Draw();
	}

	if (KoloiamAlpha > 0.f) {
		FieldObject->Draw();
	}
	if (SceneManager::GetIns()->GetScene() == SceneManager::BOSS)
	{
		if (nextStageF)
		{
			for (int i = 0; i < SkyMap.size(); i++) {
				if (SkyMap[i] == nullptr) continue;
					SkyMap[i]->Draw();
			}
			for (int i = 0; i < SkyBack.size(); i++) {
				if (SkyBack[i] == nullptr) continue;
				SkyBack[i]->Draw();
			}
			m_object[CELESTIALSPHERE]->Draw();
		}

	if (!nextStageF)
	{
		ModelDraw_nullCheck(BOSSBACK);
	}

	}
	if (SceneManager::GetIns()->GetScene() == SceneManager::PLAY)
	{
		ModelDraw_nullCheck(PEDESTAL);
		ModelDraw_nullCheck(BOSSBACK);
	}
	Object3d::PostDraw();

	BossMap::GetIns()->Draw();

	ImGui::Begin("cel");
	ImGui::SliderFloat("scl", &celestalscl, 10, 40);
	ImGui::End();
}

void Field::WarningDraw()
{
	if (SceneManager::GetIns()->GetScene() == SceneManager::BOSS)
	{

		Texture::PreDraw();
		if (cleartex != nullptr) {
			if (EnemyControl::GetIns()->GetEnemy(EnemyControl::BOSS)[0] == nullptr) {
				cleartex->Draw();
			}
		}
		Texture::PostDraw();

		
	}
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
void Field::NameDraw()
{BossName->setcolor({ 1.0f, 1.0f, 1.0f, TexAlpha_BossName });
	Sprite::PreDraw();
	BossName->Draw();
	Sprite::PostDraw();
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
	for (int i = 0; i < GuardAreaSize; i++)
	{
		if (GuardArea[i] == nullptr)
		{
			continue;
		}
		GuardArea[i]->Draw();
	}
	Texture::PostDraw();
}
