#include "TutorialSprite.h"
#include"CustomButton.h"
#include"SistemConfig.h"
#include"EnemyControl.h"
#include"FenceControl.h"
#include"ChestControl.h"
#include"mHelper.h"
#include"PlayerControl.h"
#include"UI.h"

TutorialSprite* TutorialSprite::GetInstance()
{
	static TutorialSprite instance;
	return &instance;
}

void TutorialSprite::Finalize()
{
	for (int i = 0; i < TaskNum; i++)
	{
		delete Task[i];
	}
}

void TutorialSprite::Initialize()
{
	input = Input::GetInstance();
	Sprite::LoadTexture(171, L"Resources/2d/tutorialstep/startnav.png");
	Texture::LoadTexture(50, L"Resources/2d/tutorialstep/targetpos.png");
	Texture::LoadTexture(51, L"Resources/2d/tutorialstep/targetpos2.png");
	Sprite::LoadTexture(174, L"Resources/2d/tutorialstep/step2.png");
	Sprite::LoadTexture(175, L"Resources/2d/tutorialstep/step3.png");
	Sprite::LoadTexture(176, L"Resources/2d/tutorialstep/step4.png");
	Sprite::LoadTexture(177, L"Resources/2d/tutorialstep/step5.png");
	Sprite::LoadTexture(178, L"Resources/2d/tutorialstep/step7.png");
	Sprite::LoadTexture(170, L"Resources/Tuto");
	Sprite::LoadTexture(183, L"Resources/2d/icon/movecon.png");
	Sprite::LoadTexture(184, L"Resources/2d/icon/motioncon.png");

	//コントローラー画像初期化
	movecn = Sprite::Create(183, {10, 10});
	Attackcon = Sprite::Create(184, {10, 10});

	movecn->SetAnchorPoint({0.5, 0.5});
	Attackcon->SetAnchorPoint({0.5, 0.5});

	movecn->SetSize({400, 400});
	Attackcon->SetSize({400, 400});

	/// <summary>
	/// 目標のピン画像(Ground->円 Icon->三角)
	/// </summary>
	TargetPoint_Ground = Texture::Create(50);
	TargetPoint_Icon = Texture::Create(51);

	TargetPoint_Ground->CreateTexture();
	TargetPoint_Icon->CreateTexture();
	TargetPoint_Ground->SetAnchorPoint({0.5f, 0.5f});
	TargetPoint_Icon->SetAnchorPoint({0.5f, 0.5f});

	/// 各説明用テクスチャ初期化
	Task[HELLO] = Sprite::Create(171, {10, 10});
	Task[WALK] = Sprite::Create(174, {10.f, 10.f});
	Task[ATTACK] = Sprite::Create(175, {10.f, 10.f});
	Task[SELECTWEAPON] = Sprite::Create(176, {10.f, 10.f});
	Task[GETKEY] = Sprite::Create(177, {10.f, 10.f});
	Task[CLEAR] = Sprite::Create(178, {10.f, 10.f});


	targetAlpha = 1.f;
	TargetTexSize = {4.f, 4.f, 3.f};

	for (int i = 0; i < TaskNum; i++)
	{
		SpriteAlpha[i] = 0.0f;
		t[i] = 0.0f;
		Task[i]->SetAnchorPoint({0.f, 0.f});
		ClearTaskJudg[i] = false;
	}

	AllTaskClear = false;
	task = THELLO;
}


void TutorialSprite::CheckAttack()
{
	if (EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0] != nullptr)
	{
		ClearTaskJudg[ATTACK] = EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0]->GetHP() <= 0;
	}
}

void TutorialSprite::Update()
{
	TargetPosTexMove();

	movecn->SetPosition({1700.f, 790.f});
	Attackcon->SetPosition({1700.f, 790.f});
	movecn->setcolor({1, 1, 1, movea});
	Attackcon->setcolor({1, 1, 1, atacka});

	//目標地点ついたら歩きタスクはクリア
	if (Collision::GetLength({109.f, -30, -650.f}, PlayerControl::GetInstance()->GetPlayer()->GetPosition()) < 10.f)
	{
		ClearTaskJudg[WALK] = true;
	}

	//オールコンプリート
	AllTaskClear = ClearTaskJudg[WALK] && ClearTaskJudg[ATTACK] && ClearTaskJudg[GETKEY];

	bool tEnemyDeath = EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0] == nullptr;

	FenceControl::GetInstance()->SetTutorialFenceOpen(tEnemyDeath);

	switch (task)
	{
	case THELLO:
		SpriteAlpha[HELLO] += 0.02f;
		Task[HELLO]->SetPosition({540.f, 0.f});
		Task[HELLO]->SetSize({800.f, 400.f});
		SpriteAlpha[HELLO] = min(SpriteAlpha[HELLO], 3.f);
		SpriteAlpha[HELLO] = max(SpriteAlpha[HELLO], 0.f);
	//説明スプライトの拡縮
		if (SpriteAlpha[HELLO] >= 2.0)
		{
			task = TMOVE;
		}
		break;

	case TMOVE:
		movea += 0.02f;
		SpriteAlpha[HELLO] -= 0.02f;

		NextTask(t[WALK], TATTACK, ClearTaskJudg[WALK] && targetAlpha <= 0.0f);

		Ease_SpriteSize_Up(SpriteAlpha[WALK], t[WALK], WALK);
		break;
	case TATTACK:
		atacka += 0.02f;
		movea -= 0.02f;
	//攻撃
		CheckAttack();

		NextTask(t[ATTACK], TSELECTWEAPON, ClearTaskJudg[ATTACK] && targetAlpha <= 0.0f);


		Ease_SpriteSize_Up(SpriteAlpha[ATTACK], t[ATTACK], ATTACK);
		break;

	case TSELECTWEAPON:

		atacka -= 0.02f;

		if (input->TriggerButton(input->RB) || input->TriggerButton(input->LB))
		{
			ClearTaskJudg[SELECTWEAPON] = true;
		}
		NextTask(t[SELECTWEAPON], TGETKEY, ClearTaskJudg[SELECTWEAPON] && targetAlpha <= 0.0f);

		Ease_SpriteSize_Up(SpriteAlpha[SELECTWEAPON], t[SELECTWEAPON], SELECTWEAPON);

		break;
	case TGETKEY:
		//セッティング
		ClearTaskJudg[GETKEY] = ChestControl::GetInstance()->GetTutorialChest() == true;

		if (MassageCheck[GETKEY])
		{
			NextTask(t[GETKEY], TEND, ClearTaskJudg[GETKEY]);
		}

		Ease_SpriteSize_Up(SpriteAlpha[GETKEY], t[GETKEY], GETKEY);
		break;
	case TEND:
		if (MassageCheck[CLEAR])
		{
			NextTask(t[CLEAR], THELLO, false);
		}

		Ease_SpriteSize_Up(SpriteAlpha[CLEAR], t[CLEAR], CLEAR);
		break;
	default:
		break;
	}


	for (int i = WALK; i < CLEAR + 1; i++)
	{
		Task[i]->SetPosition({460.f, 600.f});
		Task[i]->SetSize({1000.f, 300.f});
		Task[i]->setcolor({1.f, 1.f, 1.f, SpriteAlpha[i]});
	}

	Task[HELLO]->setcolor({1.f, 1.f, 1.f, SpriteAlpha[HELLO]});
	movea = min(movea, 1.0f);
	movea = max(movea, 0.0f);
	atacka = min(atacka, 1.0f);
	atacka = max(atacka, 0.0f);
}

#include"imgui.h"
#include"CameraControl.h"

void TutorialSprite::TargetPosTexMove()
{
	DebugCamera* camera = CameraControl::GetInstance()->GetCamera();
	MoveTime++;
	TargetPoint_GroundRotY++;

	if (task == TMOVE && ClearTaskJudg[WALK])
	{
		TargetTexSize.x += 0.1f;
		TargetTexSize.y += 0.1f;
		targetAlpha -= 0.04f;
	}
	if (task == TATTACK && ClearTaskJudg[ATTACK])
	{
		TargetTexSize.x += 0.1f;
		TargetTexSize.y += 0.1f;
		targetAlpha -= 0.04f;
	}

	if (task == TMOVE)
	{
		TargetIconPosY = -15.0f + sinf(3.14f * 2.f / 90.f * MoveTime) * -10.0f;
		TargetPoint_Icon->SetPosition({109.f, TargetIconPosY, -650.f});
		TargetPoint_Ground->SetColor({1.0f, 1.0f, 1.0f, targetAlpha});
		TargetPoint_Ground->SetPosition({109.f, -30, -650.f});
	}

	if (task == TATTACK)
	{
		if (!ClearTaskJudg[ATTACK])
		{
			TargetTexSize = {4, 4, 3};
			targetAlpha = 0.7f;
			TargetIconPosY = -5 + sinf(3.14f * 2.f / 90.f * MoveTime) * 5;

			if (EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0] != nullptr)
			{
				XMFLOAT3 EPos = EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0]->GetPosition();
				TargetPoint_Icon->SetPosition({EPos.x, TargetIconPosY, EPos.z});
				TargetPoint_Ground->SetPosition({EPos.x, -30, EPos.z});
			}
		}
		TargetPoint_Ground->SetColor({1, 0, 0, targetAlpha});
	}

	TargetPoint_Ground->SetRotation({90, 0, TargetPoint_GroundRotY});
	TargetPoint_Ground->SetScale(TargetTexSize);
	TargetPoint_Ground->SetBillboard(FALSE);
	TargetPoint_Ground->Update(camera);

	TargetPoint_Icon->SetRotation({180, 0, 0});
	TargetPoint_Icon->SetBillboard(TRUE);
	TargetPoint_Icon->SetScale({3, 3, 3});
	TargetPoint_Icon->SetColor({1, 1, 1, 1});
	TargetPoint_Icon->Update(camera);
}

void TutorialSprite::DrawTargetPos()
{
	Texture::PreDraw();
	TargetPoint_Ground->Draw();
	TargetPoint_Icon->Draw();
	Texture::PostDraw();
}

void TutorialSprite::Draw()
{
	Sprite::PreDraw();

	for (int i = HELLO; i < CLEAR + 1; i++)
	{
		Task[i]->Draw();
	}
	movecn->Draw();
	Attackcon->Draw();
	Sprite::PostDraw();
}

void TutorialSprite::Ease_SpriteSize_Up(float& x, float& t, int index)
{
	if (ClearTaskJudg[index])
	{
		UI::GetInstance()->SetTurnoffUIDraw(FALSE);
		MassageCheck[index] = true;
	}
	if (MassageCheck[index])
	{
		x = Easing::EaseOut(t, 0.f, 0.7f);
		if (t >= 0.0f)
		{
			t -= 0.05f;
		}
		//PlayerControl::GetInstance()->GetPlayer()->SetStopFlag(false);
	}
	else
	{
		x = Easing::EaseOut(t, 0.f, 0.7f);
		if (t <= 1.0f)
		{
			t += 0.05f;
		}
	}
}

void TutorialSprite::NextTask(float t, TaskMenu nexttask, bool nextjudg)
{
	if (t <= 0.0f)
	{
		if (nextjudg)
		{
			task = nexttask;
		}
	}
}

TutorialSprite::~TutorialSprite()
{
	for (int i = 0; i < TaskNum; i++)
	{
		delete Task[i];
	}
}
