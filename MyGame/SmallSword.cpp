#include "SmallSword.h"

#include "CameraControl.h"
#include"PlayerControl.h"
#include"TargetMarker.h"
#include"EnemyControl.h"
#include"SceneManager.h"

SmallSword::~SmallSword()
{
	m_Object.reset();
}

void SmallSword::Initialize()
{
	DebugCamera* camera = CameraControl::GetIns()->GetCamera();

	m_Object = std::make_unique<Object3d>();

	//m_Object->CreateGraphicsPipeline(L"Resources/Shader/Object3dVS.hlsl", L"Resources/Shader/Object3dPS.hlsl", L"Resources/Shader/BasicGS.hlsl");
	//ƒ‚ƒfƒ‹Š„‚è“–‚Ä

	Scale = {1.f, 1.f, 1.f};

	Rotation = {16.f, 0.f, 230.f};
	SwordObbScl = {3.f, 3.f, 4.f};
}

void SmallSword::Update()
{
	DebugCamera* camera = CameraControl::GetIns()->GetCamera();

	if (!LoadF)
	{
		LoadCsv("SD", "SK", "SS", "SM");
		LoadF = true;
	}
	if (m_Object != nullptr)
	{
		m_Object->SetRotation(Rotation);
		m_Object->Update(PlayerControl::GetIns()->GetPlayer()->GetHanMat(), {1.0f, 1.0f, 1.0f, 1.0f}, camera);
	}
}

void SmallSword::Draw()
{
	if (m_Object == nullptr)
	{
		return;
	}
	Draw_Obj();
}

void SmallSword::MagicAttack()
{
	//int index = TargetMarker::GetIns()->GetNearIndex();

	//if (SceneManager::GetIns()->GetScene() == SceneManager::TUTORIAL)
	//{
	//	XMFLOAT3 ppos = PlayerControl::GetIns()->GetPlayer()->GetPosition();
	//	bool len = Collision::GetLength(EnemyControl::GetIns()->GetEnemy(EnemyControl::TUTORIAL)[0]->GetPosition(),
	//	                                ppos) < 50;
	//	if (len)
	//	{
	//		if (CustomButton::GetIns()->Get2AttackAction() && PlayerAttackState::GetIns()->GetCoolTime() == 0)
	//		{
	//			Damage = Damage_Value_S;
	//			CoolTime = CoolTime_Value_S;
	//			attackMotion = FIRESPHERE;
	//		}
	//		if (CustomButton::GetIns()->GetAttackAction() && PlayerAttackState::GetIns()->GetCoolTime() == 0)
	//		{
	//			attackMotion = BLIZZARD;
	//			Damage = Damage_Value;
	//			CoolTime = CoolTime_Value;
	//		}
	//	}
	//}

	//else if (SceneManager::GetIns()->GetScene() == SceneManager::PLAY)
	//{
	//	XMFLOAT3 ppos = PlayerControl::GetIns()->GetPlayer()->GetPosition();
	//	bool len = Collision::GetLength(
	//		EnemyControl::GetIns()->GetEnemy(EnemyControl::PLAYSCENE)[index]->GetPosition(), ppos) < 50;
	//	if (len)
	//	{
	//		if (CustomButton::GetIns()->Get2AttackAction() && PlayerAttackState::GetIns()->GetCoolTime() == 0)
	//		{
	//			Damage = Damage_Value_S;
	//			CoolTime = CoolTime_Value_S;
	//			attackMotion = FIRESPHERE;
	//		}
	//		if (CustomButton::GetIns()->GetAttackAction() && PlayerAttackState::GetIns()->GetCoolTime() == 0)
	//		{
	//			attackMotion = BLIZZARD;
	//			Damage = Damage_Value;
	//			CoolTime = CoolTime_Value;
	//		}
	//	}
	//}
	//else if (SceneManager::GetIns()->GetScene() == SceneManager::BOSS)
	//{
	//	if (CustomButton::GetIns()->Get2AttackAction() && PlayerAttackState::GetIns()->GetCoolTime() == 0)
	//	{
	//		Damage = Damage_Value_S;
	//		CoolTime = CoolTime_Value_S;
	//		attackMotion = FIRESPHERE;
	//	}
	//	if (CustomButton::GetIns()->GetAttackAction() && PlayerAttackState::GetIns()->GetCoolTime() == 0)
	//	{
	//		attackMotion = BLIZZARD;
	//		Damage = Damage_Value;
	//		CoolTime = CoolTime_Value;
	//	}
	//}
	//if (Bliz->Getphase() == Bliz->DEST || SlashArea->Getphase() == SlashArea->DEST)
	//{
	//	PlayerControl::GetIns()->GetPlayer()->SetStopFlag(false);
	//	// 	ReturnMoveCameraTarget = true;
	//}
	//DirectX::XMFLOAT3 pPos = PlayerControl::GetIns()->GetPlayer()->GetPosition();

	//if (attackMotion == FIRESPHERE || attackMotion == BLIZZARD)
	//{
	//	if (SceneManager::GetIns()->GetScene() == SceneManager::TUTORIAL)
	//	{
	//		if (attackMotion == BLIZZARD && EnemyControl::GetIns()->GetEnemy(EnemyControl::TUTORIAL)[0] != nullptr)
	//		{
	//			Bliz->SetActFlag(TRUE);
	//		}
	//		if (attackMotion == FIRESPHERE && EnemyControl::GetIns()->GetEnemy(EnemyControl::TUTORIAL)[0] !=
	//			nullptr)
	//		{
	//			SlashArea->SetActFlag(TRUE);
	//		}
	//	}
	//	else if (SceneManager::GetIns()->GetScene() == SceneManager::PLAY)
	//	{
	//		if (attackMotion == BLIZZARD && EnemyControl::GetIns()->GetEnemy(EnemyControl::PLAYSCENE)[index] !=
	//			nullptr)
	//		{
	//			Bliz->SetActFlag(TRUE);
	//		}
	//		if (attackMotion == FIRESPHERE && EnemyControl::GetIns()->GetEnemy(EnemyControl::PLAYSCENE)[index] !=
	//			nullptr)
	//		{
	//			SlashArea->SetActFlag(TRUE);
	//		}
	//	}
	//	else if (SceneManager::GetIns()->GetScene() == SceneManager::BOSS)
	//	{
	//		if (attackMotion == BLIZZARD && EnemyControl::GetIns()->GetEnemy(EnemyControl::BOSS)[0] != nullptr)
	//		{
	//			Bliz->SetActFlag(TRUE);
	//		}
	//		if (attackMotion == FIRESPHERE && EnemyControl::GetIns()->GetEnemy(EnemyControl::BOSS)[0] != nullptr)
	//		{
	//			SlashArea->SetActFlag(TRUE);
	//		}
	//	}
	//	attackMotion = NON;
	//}
}

void SmallSword::Blizzard::Init(DebugCamera* camera)
{
	IceCrystalObj = std::make_unique<Object3d>();
	IceCrystalObj->Initialize(camera);
	Texture::LoadTexture(69, L"Resources/iceMagic.png");
	IceCrystalObj->SetModel(ModelManager::GetIns()->GetModel(ModelManager::ICECRYSTAL));

	Texture* l_tex = {Texture::Create(69, {0.0f, 0.0f, 0.0f}, {100.0f, 100.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f})};
	IceMagicTex.reset(l_tex);
	IceMagicTex->CreateTexture();
	IceMagicTex->SetAnchorPoint({0.5f, 0.5f});

	DestAlpha = 1.0f;
	DestTime = 0.0f;
}

void SmallSword::Slash::Init(DebugCamera* camera)
{
	Texture* l_tex[TexNum];
	Texture::LoadTexture(24, L"Resources/crystal.jpg");
	for (int i = 0; i < TexNum; i++)
	{
		l_tex[i] = {Texture::Create(24, {0.0f, 0.0f, 0.0f}, {100.0f, 100.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f})};
		InpactTex[i].reset(l_tex[i]);
		InpactTex[i]->CreateTexture();
		InpactTex[i]->SetAnchorPoint({0.5f, 0.5f});
	}
}


/*-------------*/
/*    XV     */
/*-------------*/
void SmallSword::Blizzard::Updata(DebugCamera* camera)
{
	//const float texSclupSpeed = 0.2f;
	//if (phase == NON)
	//{
	//	if (SceneManager::GetIns()->GetScene() == SceneManager::TUTORIAL)
	//	{
	//		if (EnemyControl::GetIns()->GetEnemy(EnemyControl::TUTORIAL)[0] != nullptr)
	//		{
	//			EnemyControl::GetIns()->GetEnemy(EnemyControl::TUTORIAL)[0]->SetMoveStop(false);
	//			if (ActFlag)
	//			{
	//				IcePos.x = EnemyControl::GetIns()->GetEnemy(EnemyControl::TUTORIAL)[0]->GetPosition().x;
	//				IcePos.z = EnemyControl::GetIns()->GetEnemy(EnemyControl::TUTORIAL)[0]->GetPosition().z - 5;

	//				EnemyControl::GetIns()->GetEnemy(EnemyControl::TUTORIAL)[0]->RecvDamage(4);
	//				phase = ACTIVE;
	//			}
	//		}
	//	}
	//	else if (SceneManager::GetIns()->GetScene() == SceneManager::PLAY)
	//	{
	//		int index = TargetMarker::GetIns()->GetNearIndex();
	//		EnemyControl::GetIns()->GetEnemy(EnemyControl::PLAYSCENE)[index]->SetMoveStop(false);
	//		if (ActFlag)
	//		{
	//			IcePos.x = EnemyControl::GetIns()->GetEnemy(EnemyControl::PLAYSCENE)[index]->GetPosition().x;
	//			IcePos.z = EnemyControl::GetIns()->GetEnemy(EnemyControl::PLAYSCENE)[index]->GetPosition().z - 5;

	//			EnemyControl::GetIns()->GetEnemy(EnemyControl::PLAYSCENE)[index]->RecvDamage(6);
	//			phase = ACTIVE;
	//		}
	//	}
	//	else if (EnemyControl::GetIns()->GetEnemy(EnemyControl::BOSS)[0] != nullptr)
	//	{
	//		EnemyControl::GetIns()->GetEnemy(EnemyControl::BOSS)[0]->SetMoveStop(false);
	//		if (ActFlag)
	//		{
	//			IcePos.x = EnemyControl::GetIns()->GetEnemy(EnemyControl::BOSS)[0]->GetPosition().x;
	//			IcePos.z = EnemyControl::GetIns()->GetEnemy(EnemyControl::BOSS)[0]->GetPosition().z - 5;

	//			EnemyControl::GetIns()->GetEnemy(EnemyControl::BOSS)[0]->RecvDamage(4);
	//			phase = ACTIVE;
	//		}
	//	}
	//	EaseTime = 0.0f;
	//	IceExistence = 0;
	//	DestAlpha = 1.0f;
	//	DestTime = 0.0f;

	//	IceCrystalObj->SetDestFlag(FALSE);
	//	IceScl = {5.0f, 5.0f, 5.0f};
	//	TexScl = {0.0f, 0.0f, 1.0f};
	//}
	//else if (phase == ACTIVE)
	//{
	//	PlayerControl::GetIns()->GetPlayer()->SetStopFlag(true);
	//	if (SceneManager::GetIns()->GetScene() == SceneManager::TUTORIAL)
	//	{
	//		EnemyControl::GetIns()->GetEnemy(EnemyControl::TUTORIAL)[0]->SetMoveStop(true);
	//		IcePos.y = Easing::EaseOut(
	//			EaseTime, EnemyControl::GetIns()->GetEnemy(EnemyControl::TUTORIAL)[0]->GetPosition().y - 20.0f,
	//			EnemyControl::GetIns()->GetEnemy(EnemyControl::TUTORIAL)[0]->GetPosition().y - 5.0f);
	//	}
	//	else if (SceneManager::GetIns()->GetScene() == SceneManager::PLAY)
	//	{
	//		int index = TargetMarker::GetIns()->GetNearIndex();

	//		EnemyControl::GetIns()->GetEnemy(EnemyControl::PLAYSCENE)[index]->SetMoveStop(true);
	//		IcePos.y = Easing::EaseOut(
	//			EaseTime,
	//			EnemyControl::GetIns()->GetEnemy(EnemyControl::PLAYSCENE)[index]->GetPosition().y - 20.0f,
	//			EnemyControl::GetIns()->GetEnemy(EnemyControl::PLAYSCENE)[index]->GetPosition().y - 5.0f);
	//	}
	//	else if (SceneManager::GetIns()->GetScene() == SceneManager::BOSS)
	//	{
	//		EnemyControl::GetIns()->GetEnemy(EnemyControl::BOSS)[0]->SetMoveStop(true);
	//		IcePos.y = Easing::EaseOut(
	//			EaseTime, EnemyControl::GetIns()->GetEnemy(EnemyControl::BOSS)[0]->GetPosition().y - 20.0f,
	//			EnemyControl::GetIns()->GetEnemy(EnemyControl::BOSS)[0]->GetPosition().y - 5.0f);
	//	}
	//	IceExistence++;
	//	TexScl.x += texSclupSpeed;
	//	TexScl.y += texSclupSpeed;
	//	TexAlpha -= 0.04f;
	//	if (EaseTime < 1.0f)
	//	{
	//		EaseTime += 1.0f / 25.0f;
	//	}
	//	bool nextPhase = IceExistence > 90;
	//	if (nextPhase)
	//	{
	//		phase = DEST;
	//	}
	//	///IceScl.y = min(IceScl.y, 5.0f);
	//}
	//else if (phase == DEST)
	//{
	//	if (SceneManager::GetIns()->GetScene() == SceneManager::TUTORIAL)
	//	{
	//		EnemyControl::GetIns()->GetEnemy(EnemyControl::TUTORIAL)[0]->SetMoveStop(true);
	//	}
	//	else if (SceneManager::GetIns()->GetScene() == SceneManager::PLAY)
	//	{
	//		int index = TargetMarker::GetIns()->GetNearIndex();

	//		EnemyControl::GetIns()->GetEnemy(EnemyControl::PLAYSCENE)[index]->SetMoveStop(true);
	//	}
	//	else if (SceneManager::GetIns()->GetScene() == SceneManager::BOSS)
	//	{
	//		EnemyControl::GetIns()->GetEnemy(EnemyControl::BOSS)[0]->SetMoveStop(true);
	//	}
	//	TexScl.x += texSclupSpeed;
	//	TexScl.y += texSclupSpeed;
	//	DestTime += 0.2f;
	//	DestAlpha -= 0.02f;

	//	IceCrystalObj->SetDestFlag(TRUE);
	//	IceCrystalObj->SetDestTime(DestTime);

	//	bool EndDest = DestAlpha < 0.0f;
	//	if (EndDest)
	//	{
	//		//IceCrystalObj->SetDestFlag(FALSE);
	//		ActFlag = false;
	//		phase = NON;
	//	}
	//}
	//IceMagicTex->SetDisplayRadius(100);
	//IceMagicTex->SetBillboard(false);

	//IceCrystalObj->SetScale(IceScl);
	//IceCrystalObj->SetPosition(IcePos);
	//IceCrystalObj->SetColor({1.0f, 1.0f, 1.0f, DestAlpha});
	//IceCrystalObj->Update({1.0f, 1.0f, 1.0f, DestAlpha}, camera);

	//IceMagicTex->Update(camera);
	//IceMagicTex->SetPosition({IcePos.x, IcePos.y, IcePos.z});
	//IceMagicTex->SetRotation({90.0f, 0.0f, 0.0f});
	//IceMagicTex->SetScale(TexScl);
	//IceMagicTex->SetColor({1, 1, 1, TexAlpha});
}

void SmallSword::Slash::Updata(DebugCamera* camera)
{
	//int index = TargetMarker::GetIns()->GetNearIndex();
	//if (phase == NON)
	//{
	//	if (ActFlag)
	//	{
	//		for (int i = 0; i < TexNum; i++)
	//		{
	//			int randAlpha = rand() % 2;
	//			Alpha[i] = static_cast<float>(randAlpha) / 1.50f;
	//			//Alpha[i] = 1.0f;
	//			TexScale[i] = {1.0f, 2.0f, 2.0f};
	//			slashCount[i] = 0;
	//			next = false;
	//		}
	//		if (SceneManager::GetIns()->GetScene() == SceneManager::TUTORIAL)
	//		{
	//			EnemyControl::GetIns()->GetEnemy(EnemyControl::TUTORIAL)[0]->RecvDamage(40);
	//		}
	//		else if (SceneManager::GetIns()->GetScene() == SceneManager::PLAY)
	//		{
	//			EnemyControl::GetIns()->GetEnemy(EnemyControl::PLAYSCENE)[index]->RecvDamage(40);
	//		}
	//		else if (SceneManager::GetIns()->GetScene() == SceneManager::BOSS)
	//		{
	//			EnemyControl::GetIns()->GetEnemy(EnemyControl::BOSS)[0]->RecvDamage(40);
	//		}
	//		phase = ACTIVE;
	//	}
	//}

	//else if (phase == ACTIVE)
	//{
	//	if (SceneManager::GetIns()->GetScene() == SceneManager::TUTORIAL)
	//	{
	//		EnemyControl::GetIns()->GetEnemy(EnemyControl::TUTORIAL)[0]->SetMoveStop(true);
	//	}
	//	else if (SceneManager::GetIns()->GetScene() == SceneManager::PLAY)
	//	{
	//		EnemyControl::GetIns()->GetEnemy(EnemyControl::PLAYSCENE)[index]->SetMoveStop(true);
	//	}
	//	else if (SceneManager::GetIns()->GetScene() == SceneManager::BOSS)
	//	{
	//		EnemyControl::GetIns()->GetEnemy(EnemyControl::BOSS)[0]->SetMoveStop(true);
	//	}
	//	for (int i = 1; i < TexNum; i++)
	//	{
	//		if (slashCount[0] <= 3 || Alpha[0] > 0.0f)
	//		{
	//			continue;
	//		}
	//		if (slashCount[0] == slashCount[i] && Alpha[0] == Alpha[i])
	//		{
	//			next = true;
	//		}
	//	}
	//	XMFLOAT3 ePos = {0.0f, 0.0f, 0.0f};
	//	if (SceneManager::GetIns()->GetScene() == SceneManager::TUTORIAL)
	//	{
	//		ePos = EnemyControl::GetIns()->GetEnemy(EnemyControl::TUTORIAL)[0]->GetPosition();
	//	}
	//	else if (SceneManager::GetIns()->GetScene() == SceneManager::PLAY)
	//	{
	//		ePos = EnemyControl::GetIns()->GetEnemy(EnemyControl::PLAYSCENE)[index]->GetPosition();
	//	}
	//	else if (SceneManager::GetIns()->GetScene() == SceneManager::BOSS)
	//	{
	//		ePos = EnemyControl::GetIns()->GetEnemy(EnemyControl::BOSS)[0]->GetPosition();
	//	}
	//	for (int i = 0; i < TexNum; i++)
	//	{
	//		Alpha[i] -= 0.01f;
	//		TexScale[i].x -= 0.02f;
	//		TexScale[i].y += 0.2f;
	//		if (TexScale[i].y >= 6.0f && slashCount[i] <= 3)
	//		{
	//			rotCorrection_Value[i] = rand() % 60 - 31;
	//			posCorrection_Valuex[i] = rand() % 20 - 10;
	//			posCorrection_Valuey[i] = rand() % 20 - 10;
	//			posCorrection_Valuez[i] = rand() % 5 - 10;
	//			InpactTex[i]->SetPosition({
	//				ePos.x + static_cast<float>(posCorrection_Valuex[i]),
	//				ePos.y + static_cast<float>(posCorrection_Valuey[i]),
	//				ePos.z + static_cast<float>(posCorrection_Valuez[i])
	//			});
	//			Alpha[i] = 1.0f;
	//			slashCount[i]++;
	//			TexScale[i] = {1.0f, 2.0f, 2.0f};
	//		}
	//	}
	//	if (next)
	//	{
	//		phase = DEST;
	//	}
	//}
	//else if (phase == DEST)
	//{
	//	next = false;
	//	if (SceneManager::GetIns()->GetScene() == SceneManager::TUTORIAL)
	//	{
	//		EnemyControl::GetIns()->GetEnemy(EnemyControl::TUTORIAL)[index]->SetMoveStop(false);
	//	}
	//	else if (SceneManager::GetIns()->GetScene() == SceneManager::PLAY)
	//	{
	//		EnemyControl::GetIns()->GetEnemy(EnemyControl::PLAYSCENE)[index]->SetMoveStop(false);
	//	}
	//	phase = NON;
	//	ActFlag = false;
	//}


	//for (int i = 0; i < TexNum; i++)
	//{
	//	InpactTex[i]->SetDisplayRadius(100);


	//	InpactTex[i]->SetColor({1.0f, 1.0f, 1.0f, Alpha[i]});
	//	InpactTex[i]->SetScale(TexScale[i]);
	//	InpactTex[i]->Update(camera);
	//	InpactTex[i]->SetRotation({0.0f, 0.0f, 0.0f + rotCorrection_Value[i]});


	//	Alpha[i] = min(Alpha[i], 1.0f);
	//	Alpha[i] = max(Alpha[i], 0.0f);
	//}
}

/*-------------*/
/*     •`‰æ    */
/*-------------*/
void SmallSword::Blizzard::Draw()
{
	if (!ActFlag)
	{
		return;
	}
	Object3d::PreDraw();

	//IceCrystalObj->Draw();
	Object3d::PostDraw();

	Texture::PreDraw();
	//IceMagicTex->Draw();
	Texture::PostDraw();
}

void SmallSword::Slash::Draw()
{
	Texture::PreDraw();
	for (int i = 0; i < TexNum; i++)
	{
		//	InpactTex[i]->Draw();
	}
	Texture::PostDraw();
}
