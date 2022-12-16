#include "SmallSword.h"
#include"PlayerControl.h"
#include"TargetMarker.h"


#include"EnemyControl.h"
#include"PlayerControl.h"
#include"mHelper.h"
#include"CustomButton.h"
#include"SceneManager.h"

SmallSword::~SmallSword()
{
	//delete  m_Model;
}

void SmallSword::Initialize(DebugCamera* camera)
{
	m_Object = std::make_unique<Object3d>();

	m_Model = Model::CreateFromOBJ("Wand");
	//m_Object->CreateGraphicsPipeline(L"Resources/Shader/Object3dVS.hlsl", L"Resources/Shader/Object3dPS.hlsl", L"Resources/Shader/BasicGS.hlsl");
	//ƒ‚ƒfƒ‹Š„‚è“–‚Ä
	m_Object->Initialize(camera);
	m_Object->SetModel(m_Model);

	Scale = {1, 1, 1};

	Rotation = {0, 0 + 30, 0 + 100};
	animetiontime = 0.03f;
	KnockPower = 15.0f;

	Damage = 10;
	Bliz = std::make_unique<Blizzard>();
	Bliz->Init(camera);

	SlashArea = std::make_unique<Slash>();
	SlashArea->Init(camera);
	TargetMarker::GetInstance()->Initialize();
}

void SmallSword::Update(DebugCamera* camera)
{
	if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY)
	{
		TargetMarker::GetInstance()->Update_PlayScene(camera);
	}

	m_Object->SetRotation(Rotation);
	m_Object->Update(PlayerControl::GetInstance()->GetPlayer()->GetHanMat(), {1.0f, 1.0f, 1.0f, 1.0f}, camera);

	if (PlayerControl::GetInstance()->GetPlayer()->GetnoAttack() == false)
	{
		MagicAttack();
		Bliz->Updata(camera);
		SlashArea->Updata(camera);
	}
}

void SmallSword::Draw()
{
	Draw_Obj();
	Bliz->Draw();
	SlashArea->Draw();
	TargetMarker::GetInstance()->Draw();
}

#include"PlayerAttackState.h"

void SmallSword::MagicAttack()
{
	//int index = TargetMarker::GetInstance()->GetNearIndex();

	//if (SceneManager::GetInstance()->GetScene() == SceneManager::TUTORIAL)
	//{
	//	XMFLOAT3 ppos = PlayerControl::GetInstance()->GetPlayer()->GetPosition();
	//	bool len = Collision::GetLength(EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0]->GetPosition(),
	//	                                ppos) < 50;
	//	if (len)
	//	{
	//		if (CustomButton::GetInstance()->Get2AttackAction() && PlayerAttackState::GetInstance()->GetCoolTime() == 0)
	//		{
	//			Damage = Damage_Value_S;
	//			CoolTime = CoolTime_Value_S;
	//			attackMotion = FIRESPHERE;
	//		}
	//		if (CustomButton::GetInstance()->GetAttackAction() && PlayerAttackState::GetInstance()->GetCoolTime() == 0)
	//		{
	//			attackMotion = BLIZZARD;
	//			Damage = Damage_Value;
	//			CoolTime = CoolTime_Value;
	//		}
	//	}
	//}

	//else if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY)
	//{
	//	XMFLOAT3 ppos = PlayerControl::GetInstance()->GetPlayer()->GetPosition();
	//	bool len = Collision::GetLength(
	//		EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE)[index]->GetPosition(), ppos) < 50;
	//	if (len)
	//	{
	//		if (CustomButton::GetInstance()->Get2AttackAction() && PlayerAttackState::GetInstance()->GetCoolTime() == 0)
	//		{
	//			Damage = Damage_Value_S;
	//			CoolTime = CoolTime_Value_S;
	//			attackMotion = FIRESPHERE;
	//		}
	//		if (CustomButton::GetInstance()->GetAttackAction() && PlayerAttackState::GetInstance()->GetCoolTime() == 0)
	//		{
	//			attackMotion = BLIZZARD;
	//			Damage = Damage_Value;
	//			CoolTime = CoolTime_Value;
	//		}
	//	}
	//}
	//else if (SceneManager::GetInstance()->GetScene() == SceneManager::BOSS)
	//{
	//	if (CustomButton::GetInstance()->Get2AttackAction() && PlayerAttackState::GetInstance()->GetCoolTime() == 0)
	//	{
	//		Damage = Damage_Value_S;
	//		CoolTime = CoolTime_Value_S;
	//		attackMotion = FIRESPHERE;
	//	}
	//	if (CustomButton::GetInstance()->GetAttackAction() && PlayerAttackState::GetInstance()->GetCoolTime() == 0)
	//	{
	//		attackMotion = BLIZZARD;
	//		Damage = Damage_Value;
	//		CoolTime = CoolTime_Value;
	//	}
	//}
	//if (Bliz->Getphase() == Bliz->DEST || SlashArea->Getphase() == SlashArea->DEST)
	//{
	//	PlayerControl::GetInstance()->GetPlayer()->SetStopFlag(false);
	//	// 	ReturnMoveCameraTarget = true;
	//}
	//DirectX::XMFLOAT3 pPos = PlayerControl::GetInstance()->GetPlayer()->GetPosition();

	//if (attackMotion == FIRESPHERE || attackMotion == BLIZZARD)
	//{
	//	if (SceneManager::GetInstance()->GetScene() == SceneManager::TUTORIAL)
	//	{
	//		if (attackMotion == BLIZZARD && EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0] != nullptr)
	//		{
	//			Bliz->SetActFlag(TRUE);
	//		}
	//		if (attackMotion == FIRESPHERE && EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0] !=
	//			nullptr)
	//		{
	//			SlashArea->SetActFlag(TRUE);
	//		}
	//	}
	//	else if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY)
	//	{
	//		if (attackMotion == BLIZZARD && EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE)[index] !=
	//			nullptr)
	//		{
	//			Bliz->SetActFlag(TRUE);
	//		}
	//		if (attackMotion == FIRESPHERE && EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE)[index] !=
	//			nullptr)
	//		{
	//			SlashArea->SetActFlag(TRUE);
	//		}
	//	}
	//	else if (SceneManager::GetInstance()->GetScene() == SceneManager::BOSS)
	//	{
	//		if (attackMotion == BLIZZARD && EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0] != nullptr)
	//		{
	//			Bliz->SetActFlag(TRUE);
	//		}
	//		if (attackMotion == FIRESPHERE && EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0] != nullptr)
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
	//	if (SceneManager::GetInstance()->GetScene() == SceneManager::TUTORIAL)
	//	{
	//		if (EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0] != nullptr)
	//		{
	//			EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0]->SetMoveStop(false);
	//			if (ActFlag)
	//			{
	//				IcePos.x = EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0]->GetPosition().x;
	//				IcePos.z = EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0]->GetPosition().z - 5;

	//				EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0]->RecvDamage(4);
	//				phase = ACTIVE;
	//			}
	//		}
	//	}
	//	else if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY)
	//	{
	//		int index = TargetMarker::GetInstance()->GetNearIndex();
	//		EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE)[index]->SetMoveStop(false);
	//		if (ActFlag)
	//		{
	//			IcePos.x = EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE)[index]->GetPosition().x;
	//			IcePos.z = EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE)[index]->GetPosition().z - 5;

	//			EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE)[index]->RecvDamage(6);
	//			phase = ACTIVE;
	//		}
	//	}
	//	else if (EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0] != nullptr)
	//	{
	//		EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->SetMoveStop(false);
	//		if (ActFlag)
	//		{
	//			IcePos.x = EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->GetPosition().x;
	//			IcePos.z = EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->GetPosition().z - 5;

	//			EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->RecvDamage(4);
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
	//	PlayerControl::GetInstance()->GetPlayer()->SetStopFlag(true);
	//	if (SceneManager::GetInstance()->GetScene() == SceneManager::TUTORIAL)
	//	{
	//		EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0]->SetMoveStop(true);
	//		IcePos.y = Easing::EaseOut(
	//			EaseTime, EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0]->GetPosition().y - 20.0f,
	//			EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0]->GetPosition().y - 5.0f);
	//	}
	//	else if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY)
	//	{
	//		int index = TargetMarker::GetInstance()->GetNearIndex();

	//		EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE)[index]->SetMoveStop(true);
	//		IcePos.y = Easing::EaseOut(
	//			EaseTime,
	//			EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE)[index]->GetPosition().y - 20.0f,
	//			EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE)[index]->GetPosition().y - 5.0f);
	//	}
	//	else if (SceneManager::GetInstance()->GetScene() == SceneManager::BOSS)
	//	{
	//		EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->SetMoveStop(true);
	//		IcePos.y = Easing::EaseOut(
	//			EaseTime, EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->GetPosition().y - 20.0f,
	//			EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->GetPosition().y - 5.0f);
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
	//	if (SceneManager::GetInstance()->GetScene() == SceneManager::TUTORIAL)
	//	{
	//		EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0]->SetMoveStop(true);
	//	}
	//	else if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY)
	//	{
	//		int index = TargetMarker::GetInstance()->GetNearIndex();

	//		EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE)[index]->SetMoveStop(true);
	//	}
	//	else if (SceneManager::GetInstance()->GetScene() == SceneManager::BOSS)
	//	{
	//		EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->SetMoveStop(true);
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
	//int index = TargetMarker::GetInstance()->GetNearIndex();
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
	//		if (SceneManager::GetInstance()->GetScene() == SceneManager::TUTORIAL)
	//		{
	//			EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0]->RecvDamage(40);
	//		}
	//		else if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY)
	//		{
	//			EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE)[index]->RecvDamage(40);
	//		}
	//		else if (SceneManager::GetInstance()->GetScene() == SceneManager::BOSS)
	//		{
	//			EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->RecvDamage(40);
	//		}
	//		phase = ACTIVE;
	//	}
	//}

	//else if (phase == ACTIVE)
	//{
	//	if (SceneManager::GetInstance()->GetScene() == SceneManager::TUTORIAL)
	//	{
	//		EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0]->SetMoveStop(true);
	//	}
	//	else if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY)
	//	{
	//		EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE)[index]->SetMoveStop(true);
	//	}
	//	else if (SceneManager::GetInstance()->GetScene() == SceneManager::BOSS)
	//	{
	//		EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->SetMoveStop(true);
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
	//	if (SceneManager::GetInstance()->GetScene() == SceneManager::TUTORIAL)
	//	{
	//		ePos = EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0]->GetPosition();
	//	}
	//	else if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY)
	//	{
	//		ePos = EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE)[index]->GetPosition();
	//	}
	//	else if (SceneManager::GetInstance()->GetScene() == SceneManager::BOSS)
	//	{
	//		ePos = EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->GetPosition();
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
	//	if (SceneManager::GetInstance()->GetScene() == SceneManager::TUTORIAL)
	//	{
	//		EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[index]->SetMoveStop(false);
	//	}
	//	else if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY)
	//	{
	//		EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE)[index]->SetMoveStop(false);
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
