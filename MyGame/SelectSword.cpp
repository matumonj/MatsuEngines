#include "SelectSword.h"
#include"BigSword.h"
#include"SmallSword.h"
#include"NormalSword.h"
#include"CameraControl.h"
#include"mHelper.h"
#include"ModelManager.h"
#include"HUD.h"
#include"imgui.h"

SelectSword::~SelectSword()
{
	delete SwordSample[0], SwordSample[1], SwordSample[2], Frame;
}

SelectSword* SelectSword::GetInstance()
{
	static SelectSword instance;
	return &instance;
}

void SelectSword::Finalize()
{
	delete SwordSample[0], SwordSample[1], SwordSample[2], Frame;
	Destroy_unique(PedestalObj);
	for (int i = 0; i < 3; i++)
	{
		Destroy_unique(SampleSword[i]);
	}
}

void SelectSword::SpriteSet()
{
	Sprite::LoadTexture(229, L"Resources/2d/confignav/navselectsword.png");

	Sprite::LoadTexture(230, L"Resources/SwordA.png");
	Sprite::LoadTexture(231, L"Resources/swordB.png");
	Sprite::LoadTexture(232, L"Resources/swordC.png");

	Sprite::LoadTexture(233, L"Resources/SelSwordFrame.png");

	Sprite::LoadTexture(234, L"Resources/2d/attackicon/config_magicattack1.png");
	Sprite::LoadTexture(235, L"Resources/2d/attackicon/config_magicattack2.png");
	Sprite::LoadTexture(236, L"Resources/2d/attackicon/config_magicattack2.png");

	Sprite::LoadTexture(237, L"Resources/2d/attackicon/config_swordattack1.png");
	Sprite::LoadTexture(238, L"Resources/2d/attackicon/config_swordattack2.png");
	Sprite::LoadTexture(239, L"Resources/2d/attackicon/config_swordattack2.png");

	Sprite::LoadTexture(240, L"Resources/2d/attackicon/SetCustom.png");
	NavSprite = Sprite::Create(229, {1.0f, 1.0f});
	SwordSample[0] = Sprite::Create(230, {1.0f, 1.0f});
	SwordSample[1] = Sprite::Create(231, {1.0f, 1.0f});
	SwordSample[2] = Sprite::Create(232, {1.0f, 1.0f});
	Frame = Sprite::Create(233, {1.0f, 1.0f});
	for (int i = 0; i < 3; i++)
	{
		//フィールドにモデル割り当て
		//SwordSample[i]->Initialize();
		SwordSample[i]->SetSize({400.0f, 400.0f});
		SwordSample[i]->SetAnchorPoint({0.5f, 0.5f});
		SwordSample[i]->SetPosition({Position[i]});
	}
	Frame->SetSize({450.0f, 450.0f});
	Frame->SetPosition(Position[0]);
	Frame->SetAnchorPoint({0.5f, 0.5f});


	WeponParamSprite[0] = Sprite::Create(234, {1.0f, 1.0f});
	WeponParamSprite[1] = Sprite::Create(235, {1.0f, 1.0f});
	WeponParamSprite[2] = Sprite::Create(236, {1.0f, 1.0f});

	Equipment = Sprite::Create(240, {1.0f, 1.0f});
	Equipment->SetSize({1000, 600});
	EquipmentPos.x = 200.0f;

	NavSprite->SetSize({1500, 700});
	NavSprite->SetPosition({400, 300});
}

void SelectSword::WeponParamInit()
{
	for (int i = 0; i < 3; i++)
	{
		WeponParamSprite[i]->SetPosition(WeponParamSpritePos[i]);
	}

	for (int i = 0; i < 3; i++)
	{
		WeponParamSprite[i]->SetSize({1000, 800});
		WeponParamSprite[i]->SetAnchorPoint({0.5f, 0.5f});
	}
}

void SelectSword::WeponParamUpdate()
{
	WeponParamSpriteEaseT[0] += 1.0f / 60.0f;
	WeponParamSpriteEaseT[1] += 1.0f / 80.0f;
	WeponParamSpriteEaseT[2] += 1.0f / 100.0f;

	WeponParamSpritePos[0].y = Easing::EaseOut(WeponParamSpriteEaseT[0], 600, ypos_Up);
	WeponParamSpritePos[1].y = Easing::EaseOut(WeponParamSpriteEaseT[1], 600, ypos_Center);
	WeponParamSpritePos[2].y = Easing::EaseOut(WeponParamSpriteEaseT[2], 600, ypos_Bottom);

	int Oldindex = index;
	if (index != Oldindex)
	{
		WeponParamSpriteEaseT[0] = 0.0f;
		WeponParamSpriteEaseT[1] = 0.0f;
		WeponParamSpriteEaseT[2] = 0.0f;
	}
	if (NowSelectSword == SMALL)
	{
		WeponParamSpritePos[0].x = 255.0f;
		WeponParamSpritePos[1].x = 255.0f;
		WeponParamSpritePos[2].x = 255.0f;
	}
	else if (NowSelectSword == NORMAL)
	{
		WeponParamSpritePos[0].x = 650.0f;
		WeponParamSpritePos[1].x = 650.0f;
		WeponParamSpritePos[2].x = 650.0f;
	}
	for (int i = 0; i < 3; i++)
	{
		WeponParamSpriteEaseT[i] = min(WeponParamSpriteEaseT[i], 1.0f);
	}
}

void SelectSword::pedestalSet()
{
	for (int i = 0; i < 3; i++)
	{
		SampleSword[i] = std::make_unique<Object3d>();
	}
	SampleSword[SMALL]->SetModel(ModelManager::GetIns()->GetModel(ModelManager::SMALLSWORD));
	SampleSword[SMALL]->Initialize(CameraControl::GetInstance()->GetCamera());
	sampleSwordRot[SMALL] = {-60.0f, 100.0f, -90.0f};

	SampleSword[NORMAL]->SetModel(ModelManager::GetIns()->GetModel(ModelManager::NORMALSWORD));
	SampleSword[NORMAL]->Initialize(CameraControl::GetInstance()->GetCamera());
	sampleSwordRot[NORMAL] = {-60.0f, 100.0f, -90.0f};

	SampleSword[BIG]->SetModel(ModelManager::GetIns()->GetModel(ModelManager::BIGSWORD));
	SampleSword[BIG]->Initialize(CameraControl::GetInstance()->GetCamera());
	sampleSwordRot[BIG] = {-60.0f, 100.0f, -90.0f};

	PedestalObj = std::make_unique<Object3d>();
	PedestalObj->SetModel(ModelManager::GetIns()->GetModel(ModelManager::PEDESTAL));
	PedestalObj->Initialize(CameraControl::GetInstance()->GetCamera());

	RotAngle = 0.0f;
	RotationF = false;
	EaseTime = 0.0f;
}

void SelectSword::Initialize()
{
	Sword = std::make_unique<NormalSword>();
	Sword->Initialize(CameraControl::GetInstance()->GetCamera());

	SpriteSet();
	pedestalSet();
	input = Input::GetInstance();
	index = 0;
	WeponParamInit();
}

void SelectSword::SetSword(SwordScale nowsword)
{
	switch (nowsword)
	{
	case SMALL:
		Sword = std::make_unique<SmallSword>();
		break;
	case NORMAL:
		Sword = std::make_unique<NormalSword>();
		break;
	case BIG:
		Sword = std::make_unique<BigSword>();
		break;
	default:
		break;
	}
	Sword->Initialize(CameraControl::GetInstance()->GetCamera());
}

void SelectSword::Update()
{
	if (SelectJudg)
	{
		WeponParamUpdate();
		for (int i = 0; i < 3; i++)
		{
			WeponParamSprite[i]->SetPosition(WeponParamSpritePos[i]);
		}
		if (input->TiltStick(input->L_RIGHT))
		{
			ResetEaseTime();
			index++;
		}
		else if (input->TiltStick(input->L_LEFT))
		{
			ResetEaseTime();
			index--;
		}

		if (index == SMALL)
		{
			if (SkillSprite != SMALL)
			{
				WeponParamSprite[0] = Sprite::Create(234, {1.0f, 1.0f});
				WeponParamSprite[1] = Sprite::Create(235, {1.0f, 1.0f});
				WeponParamSprite[2] = Sprite::Create(236, {1.0f, 1.0f});

				WeponParamInit();
				SkillSprite = SMALL;
			}
			NowSelectSword = SMALL;
		}
		else if (index == NORMAL)
		{
			if (SkillSprite != NORMAL)
			{
				WeponParamSprite[0] = Sprite::Create(237, {1.0f, 1.0f});
				WeponParamSprite[1] = Sprite::Create(238, {1.0f, 1.0f});
				WeponParamSprite[2] = Sprite::Create(239, {1.0f, 1.0f});
				WeponParamInit();
				SkillSprite = NORMAL;
			}
			NowSelectSword = NORMAL;
		}
		else if (index == BIG)
		{
			NowSelectSword = BIG;
		}
		if (input->TriggerButton(input->A))
		{
			if (NowSelectSword == SMALL)
			{
				EquipmentPos.x = -200.0f;
				HUD::GetInstance()->SetSkillIcon(HUD::WAND);
			}
			if (NowSelectSword == NORMAL)
			{
				EquipmentPos.x = 200.0f;
				HUD::GetInstance()->SetSkillIcon(HUD::SWORD);
			}
			SetSword(NowSelectSword);
		}
		Frame->SetPosition(Position[index]);
		Equipment->SetPosition(EquipmentPos);
		CameraControl::GetInstance()->GetCamera()->SetEye({0, 0, -30});
		CameraControl::GetInstance()->GetCamera()->SetTarget({10, 0, 0});
	}
	else
	{
		oldcamerapos = CameraControl::GetInstance()->GetCamera()->GetEye();
	}

	Sword->Update(CameraControl::GetInstance()->GetCamera());

	PedestalUpdate();

	index = min(index, 2);
	index = max(index, 0);
}

void SelectSword::PedestalUpdate()
{
	//台座座標
	const XMFLOAT3 PedestalPos = {40, 0, 0};
	//サンプル剣回転時の中心座標からの距離(半径)
	const float Radius = 6.0f;
	const float Height = 3.0f;
	//円回転するやつ
	//右回転
	if (input->TiltStick(input->L_RIGHT))
	{
		rotDir = RIGHT;
	}
	//左回転
	if (input->TiltStick(input->L_LEFT))
	{
		rotDir = LEFT;
	}
	//イージングカウントリセット
	if (rotDir == NON)
	{
		EaseTime = 0.0f;
	}
	SwordRot();

	/*-----------剣の座標---------------------------------------------*/
	XMFLOAT3 samplePos_big = {
		PedestalPos.x + cosf(RotAngle * PI / 180.0f) * Radius,
		PedestalPos.y + Height,
		PedestalPos.z + sinf(RotAngle * PI / 180.0f) * Radius
	};

	XMFLOAT3 samplePos_normal = {
		PedestalPos.x + cosf((RotAngle + 120.0f) * PI / 180.0f) * Radius,
		PedestalPos.y + Height,
		PedestalPos.z + sinf((RotAngle + 120.0f) * PI / 180.0f) * Radius
	};

	XMFLOAT3 samplePos_small = {
		PedestalPos.x + cosf((RotAngle + 240.0f) * PI / 180.0f) * Radius,
		PedestalPos.y + Height,
		PedestalPos.z + sinf((RotAngle + 240.0f) * PI / 180.0f) * Radius
	};
	/*-------------------------------------------------------------*/
	SampleSword[SMALL]->SetPosition(samplePos_small);
	SampleSword[NORMAL]->SetPosition(samplePos_normal);
	SampleSword[BIG]->SetPosition(samplePos_big);

	for (int i = 0; i < 3; i++)
	{
		SampleSword[i]->SetRotation(sampleSwordRot[i]);
		SampleSword[i]->SetScale({2.0f, 2.0f, 2.0f});
		SampleSword[i]->Update({1.0f, 1.0f, 1.0f, 1.0f}, CameraControl::GetInstance()->GetCamera());
	}

	//現在選択中の剣くるくる回る
	switch (index)
	{
	case SMALL:
		sampleSwordRot[SMALL].y--;
		break;
	case NORMAL:
		sampleSwordRot[NORMAL].y--;
		break;
	case BIG:
		sampleSwordRot[BIG].y--;
		break;
	default:
		break;
	}

	PedestalObj->SetScale({10.0f, 10.0f, 10.0f});
	PedestalObj->SetPosition(PedestalPos);
	PedestalObj->SetColor({1.0f, 1.0f, 1.0f, 1.0f});
	PedestalObj->Update({10.0f, 10.0f, 10.0f, 10.0f}, CameraControl::GetInstance()->GetCamera());
}

void SelectSword::SwordRot()
{
	//イージングカウント
	const float EaseC = 0.05f;
	if (rotDir == RIGHT)
	{
		EaseTime += EaseC;
		//現在の角度から120度回転
		RotAngle = Easing::EaseOut(EaseTime, RotAngle_Old[index - 1], RotAngle_Old[index]);
		if (EaseTime >= 1.0f)
		{
			rotDir = NON;
		}
	}
	else if (rotDir == LEFT)
	{
		EaseTime -= EaseC;
		//現在の角度から120度回転
		RotAngle = Easing::EaseOut(EaseTime, RotAngle_Old[index + 1], RotAngle_Old[index]);
		if (EaseTime <= -1.0f)
		{
			rotDir = NON;
		}
	}
}

void SelectSword::Draw()
{
	SpriteDraw();
}

void SelectSword::SpriteDraw()
{
	if (SelectJudg)
	{
		Sprite::PreDraw();
		for (int i = 0; i < 3; i++)
		{
			WeponParamSprite[i]->Draw();
		}
		for (int i = 0; i < 3; i++)
		{
			SwordSample[i]->Draw();
		}

		Frame->Draw();
		Equipment->Draw();
		NavSprite->Draw();
		Sprite::PostDraw();
	}
}

void SelectSword::PedestalDraw()
{
	if (!SelectJudg)
	{
		return;
	}
	Object3d::PreDraw();
	PedestalObj->Draw();
	for (int i = 0; i < 3; i++)
	{
		SampleSword[i]->Draw();
	}
	Object3d::PostDraw();
}

void SelectSword::SwordDraw()
{
	Sword->Draw();
}
