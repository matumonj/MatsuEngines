#include "Field.h"
#include <algorithm>
#include "BossMap.h"
#include "CameraControl.h"
#include"TouchableObject.h"
#include"CollisionManager.h"
#include"SceneManager.h"
#include"ModelManager.h"
#include"Destroy.h"
#include"CollisionPrimitive.h"
#include "EnemyControl.h"
#include"UltAttack.h"
#include "Feed.h"
#include "mHelper.h"
#include"FootSwitch.h"
#include "PlayerControl.h"
#define PI 3.14f

Field::~Field()
{
	Destroy_unique(m_object[BOSSBACK]);
	for (int i = 0; i < SkyBack.size(); i++)
	{
		Destroy_unique(SkyBack[i]);
	}
	for (int i = 0; i < SkyMap.size(); i++)
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
	//ミニマップ(通常ふぃ−るど)
	//天球
	SetFieldModel(CELESTIALSPHERE, ModelManager::GetIns()->GetModel(ModelManager::SKY), camera);
}

void Field::Init_Play()
{
	DebugCamera* camera = CameraControl::GetIns()->GetCamera();

	FieldObject = TouchableObject::Create(ModelManager::GetIns()->GetModel(ModelManager::FIELD), camera);
	//ミニマップ(通常ふぃ−るど)
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
		GuardArea[i]->SetAnchorPoint({0.5f, 0.5f});

		GuardAreaAngle[i] = static_cast<float>(i) * (360.f / static_cast<float>(GuardAreaSize));
		//位置の初期化
		GuardareaPos[i].x = pedestalpos.x + sinf(GuardAreaAngle[i] * (PI / 180.0f)) * 80.0f;
		GuardareaPos[i].z = pedestalpos.z + cosf(GuardAreaAngle[i] * (PI / 180.0f)) * 80.0f;
	}
	//ガーディアン出現用の塔
	pedestalpos = {-242.0f, -45.f, 333.f};

	SetFieldModel(PEDESTAL, ModelManager::GetIns()->GetModel(ModelManager::ICECRYSTAL), camera);

	//ボスの背景obj
	SetFieldModel(BOSSBACK, ModelManager::GetIns()->GetModel(ModelManager::BOSSFIELD), camera);

	FootSwitch::GetIns()->Init();
}

void Field::Init_Boss()
{
	DebugCamera* camera = CameraControl::GetIns()->GetCamera();
	if (camera == nullptr)
	{
		return;
	}
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
	cleartex->SetAnchorPoint({0.5f, 0.5f});

	//銅像
	for (int i = 0; i < SkyMap.size(); i++)
	{
		SkyMap[i].reset(new Object3d());
		SkyMap[i]->Initialize(camera);
		SkyMap[i]->SetModel(ModelManager::GetIns()->GetModel(ModelManager::SKYMAP));
		SkyMap[i]->SetScale({1, 1, 1});
		SkyMap[i]->SetRotation({0.f, static_cast<float>(i) * -90.f, 0.f});
	}
	SkyMap[0]->SetPosition({0.0f, 0.0f, -200.0f});
	SkyMap[1]->SetPosition({200.0f, 0.0f, 0.0f});
	SkyMap[2]->SetPosition({0.0f, 0.0f, 200.0f});
	SkyMap[3]->SetPosition({-200.0f, 0.0f, 0.0f});

	//塔
	for (int i = 0; i < SkyBack.size(); i++)
	{
		SkyBack[i].reset(new Object3d());
		SkyBack[i]->Initialize(camera);
		SkyBack[i]->SetModel(ModelManager::GetIns()->GetModel(ModelManager::TOWER));
		SkyBack[i]->SetScale({8, 10, 8});
	}
	SkyBackPos[0] = {-150.f, -100.f, 140.f};
	SkyBackPos[1] = {87.f, -100.f, 160.f};
	SkyBackPos[2] = {160.f, -100.f, -150.f};
	SkyBackPos[3] = {-150.f, -100.f, -150.f};
	//下地
	FieldObject = TouchableObject::Create(ModelManager::GetIns()->GetModel(ModelManager::SANDFIELD), camera);
	Sprite::LoadTexture(40, L"Resources/2d/icon/BossName.png");
	//ボスのネームプレート
	BossName = Sprite::Create(40, {WinApp::window_width / 2, WinApp::window_height / 2});
	BossName->SetAnchorPoint({0.5f, 0.5f});
	BossName->SetPosition({WinApp::window_width / 2, WinApp::window_height / 2});
	BossName->SetSize({800, 800});

	//松明用
	for (int i = 0; i < TorchObj.size(); i++)
	{
		TorchObj[i].reset(new Object3d());
		TorchObj[i]->Initialize(camera);
		TorchObj[i]->SetModel(ModelManager::GetIns()->GetModel(ModelManager::TORCH));
		TorchObj[i]->SetScale({4.f, 3.f, 4.f});

		FireEffect[i].reset(new Particle());
		FireEffect[i]->Init(Particle::ParticleTexName::TOURCHFIRE);
		FireEffect[i]->SetParScl({0.3f, 0.3f});
		FireEffect[i]->SetParColor({1.f, 1.f, 1.f, 1.f});
	}
}


void Field::Initialize()
{
	DebugCamera* camera = CameraControl::GetIns()->GetCamera();

	PedestalDownF = false;
	if (SceneManager::GetIns()->GetScene() == SceneManager::BOSS)
	{
		Init_Boss();
	}
	if (SceneManager::GetIns()->GetScene() == SceneManager::TUTORIAL)
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
	constexpr float YPos_Min = -200.f;
	constexpr float MoveDownSpeed = 0.02f;

	constexpr float PlayerColPedestalDis = 10.f;
	bool DownJudg = PedestalDownF && pedestalpos.y > YPos_Min;

	if (Collision::GetLength(PlayerControl::GetIns()->GetPlayer()->GetPosition(),
	                         pedestalpos) < PlayerColPedestalDis)
	{
		PedestalDownF = true;
	}
	if (DownJudg)
	{
		pedestalpos.y -= MoveDownSpeed;
	}


	if (PlayerControl::GetIns()->GetPlayer()->GetHP() <= 0)
	{
		PedestalDownF = false;
		pedestalpos = {-300.0f, -32, 270};
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
	FieldObject->Update(camera);
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
	SetFieldUpdate(CELESTIALSPHERE, camera, {0.0f, 2290.0f, 0.0f}, {30.0f, 30.0f, 30.0f}, FALSE, false);
	m_object[CELESTIALSPHERE]->SetColor({ 3.f,3.f,3.f,0.2f });
	SetFieldUpdate(PEDESTAL, camera, pedestalpos, {2, 2, 2});

	FieldObject->Setf(true);
	FieldObject->SetPosition({0.0f, -25.0f, 0.0f});
	FieldObject->SetColor({0.9f, 0.9f, 0.9f, 1.0f});
	FieldObject->SetFogCenter(FogCenterPos);
	FieldObject->setFog(true);
	FieldObject->Update(camera);

	m_object[BOSSBACK]->SetRotation({0.f, 180.f, 0.f});
	BossFieldPos = {60.f, -56.f, 500.f};
	SetFieldUpdate(BOSSBACK, camera, BossFieldPos, {0.6f, 0.6f, 0.6f}, FALSE, true);

	FootSwitch::GetIns()->Upda();

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
			if (Collision::IsCollidingLineAndCircle(camera_to_player, GuardAreaPoint[i], 30.f))
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
						if (Collision::IsCollidingLineAndCircle(camera_to_player, GuardAreaPoint[i], 30.f))
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

	FieldPosition = {0.0f, -25.0f, 0.0f};

	FieldObject->SetPosition({FieldPosition});
	FieldObject->SetFogCenter({0.0f, 0.0f, 0.0f});
	FieldObject->SetColor({0.8f, 0.8f, 0.8f, 1.0f});
	FieldObject->Update(camera);
}

void Field::Update_Boss()
{
	DebugCamera* camera = CameraControl::GetIns()->GetCamera();
	if (camera == nullptr)
	{
		return;
	}
	if (FieldObject == nullptr)
	{
		return;
	}

	CelestalRot += 0.1f;
	m_object[CELESTIALSPHERE]->SetRotation({0.0f, CelestalRot, 0.0f});
	SetFieldUpdate(CELESTIALSPHERE, camera, {0.0f, 30.0f, 0.0f}, {celestalscl, celestalscl, celestalscl}, FALSE, TRUE);
	SpriteFeed(TexAlpha_BossName, feed_BossName, feedSpeed_BossName, 1.5f);
	TexAlpha_BossName = min(TexAlpha_BossName, 3.0f);
	TexAlpha_BossName = max(TexAlpha_BossName, 0.0f);
	//背景のコロシアム更新
	if (KoloiamAlpha <= 0.0f)
	{
		SkyMapDestF = true;
		destf = true;
	}
	if (UltAttack::GetIns()->GetFieldDestG())
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
	if (!nextStageF)
	{
		//外周ダメージエリア更新

		m_object[BOSSBACK]->SetDestFlag(true);
		m_object[BOSSBACK]->SetDestTime(destt);
		m_object[BOSSBACK]->SetCenter(DestCenter);
		m_object[BOSSBACK]->Setf(true);
		m_object[BOSSBACK]->SetColor({0.4f, 0.4f, 0.4f, 1.0f});
		SetFieldUpdate(BOSSBACK, camera, {0, -19, 0}, {1.0f, 1.0f, 1.0f}, false, false);
	}
	FieldPosition = {0.0f, -19.0f, 0.0f};

	FieldObject->SetPosition(FieldPosition);
	FieldObject->SetColor({0.8f, 0.8f, 0.8f, KoloiamAlpha});
	FieldObject->Update(camera);


	constexpr float l_DestSpeed = 4.f;

	if (SkyMapDestF)
	{
		SkyMapDestT -= l_DestSpeed;
	}
	SkyMapDestT = max(SkyMapDestT, 0.f);

	for (int i = 0; i < SkyMap.size(); i++)
	{
		SkyMap[i]->SetDestFlag(true);
		SkyMap[i]->SetDestTime(SkyMapDestT);
		SkyMap[i]->setFog(true);
		SkyMap[i]->SetFogCenter(camera->GetEye());
		SkyMap[i]->Update(camera);
	}
	for (int i = 0; i < SkyBack.size(); i++)
	{
		SkyBack[i]->SetDestFlag(true);
		SkyBack[i]->SetDestTime(SkyMapDestT);
		SkyBack[i]->setFog(true);
		SkyBack[i]->SetFogCenter(camera->GetEye());
		SkyBack[i]->SetPosition(SkyBackPos[i]);
		SkyBack[i]->SetRotation({0.f, objroty[i], 0.f});
		SkyBack[i]->Update(camera);
	}

	//松明
	for (int i = 0; i < TorchObj.size(); i++)
	{
		TorchPos[i] = {
			sinf(static_cast<float>(i) * (360.f / static_cast<float>(TorchSize)) * (PI / 180.0f)) * TorchPosAngle,
			posY,
			cosf(static_cast<float>(i) * (360.f / static_cast<float>(TorchSize)) * (PI / 180.0f)) * TorchPosAngle
		};
		TorchObj[i]->SetPosition(TorchPos[i]);
		TorchObj[i]->Update(camera);


		FireEffect[i]->BCreateParticle(true, TorchPos[i]);
		FireEffect[i]->Bleath();
	}
	//クリアシーンへのワープテクスチャの基礎パラメータ
	constexpr XMFLOAT3 l_ClearAreaTexPos = {0.f, 17.f, 75.f};
	constexpr XMFLOAT3 l_ClearAreaTexRot = {90.f, 0.f, 0.f};
	constexpr XMFLOAT3 l_ClearAreaTexScl = {7.f, 7.f, 3.f};

	cleartex->SetPosition(l_ClearAreaTexPos);
	cleartex->SetRotation(l_ClearAreaTexRot);
	cleartex->SetScale(l_ClearAreaTexScl);
	cleartex->SetBillboard(false);
	cleartex->SetColor({1.f, 1.f, 1.f, 1.f});
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

	if (SceneManager::GetIns()->GetScene() == SceneManager::BOSS)
	{
		if (nextStageF)
		{
			for (int i = 0; i < SkyMap.size(); i++)
			{
				if (SkyMap[i] == nullptr)
				{
					continue;
				}
				SkyMap[i]->Draw();
			}
			for (int i = 0; i < SkyBack.size(); i++)
			{
				if (SkyBack[i] == nullptr)
				{
					continue;
				}
				SkyBack[i]->Draw();
			}
			m_object[CELESTIALSPHERE]->Draw();
		}
		if (!nextStageF)
		{
			ModelDraw_nullCheck(BOSSBACK);
			Object3d::PreDraw();
			for (int i = 0; i < TorchObj.size(); i++)
			{
				if (TorchObj[i] == nullptr)
				{
					continue;
				}
				TorchObj[i]->Draw();
			}
			Object3d::PostDraw();
			for (int i = 0; i < TorchObj.size() - 1; i++)
			{
				if (FireEffect[i] == nullptr)
				{
					continue;
				}
				FireEffect[i]->Draw();
			}
		}
	}

	else
	{
		FieldObject->Draw();
	}
	if (SceneManager::GetIns()->GetScene() == SceneManager::PLAY)
	{
		ModelDraw_nullCheck(PEDESTAL);
		ModelDraw_nullCheck(BOSSBACK);

		FootSwitch::GetIns()->Draw();
	}
	Object3d::PostDraw();

	BossMap::GetIns()->Draw();

	ImGui::Begin("cel"); /*
	ImGui::SliderFloat("posx", &bacff.x, -60, 60);
	ImGui::SliderFloat("posy", &bacff.y, -90, 29);
	ImGui::SliderFloat("posz", &bacff.z, 480, 580);*/
	ImGui::End();
}

void Field::WarningDraw()
{
	if (SceneManager::GetIns()->GetScene() == SceneManager::BOSS)
	{
		Texture::PreDraw();
		if (cleartex != nullptr)
		{
			if (EnemyControl::GetIns()->GetEnemy(EnemyControl::BOSS)[0] == nullptr)
			{
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

void Field::NameDraw()
{
	BossName->setcolor({1.0f, 1.0f, 1.0f, TexAlpha_BossName});
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
	m_object[type]->Update(camera);
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
