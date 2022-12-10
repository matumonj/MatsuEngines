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
	Sprite::LoadTexture(176, L"Resources/2d/tutorialstep/step6.png");
	Sprite::LoadTexture(177, L"Resources/2d/tutorialstep/step7.png");
	Sprite::LoadTexture(170, L"Resources/Tuto");

	Sprite::LoadTexture(183, L"Resources/2d/icon/movecon.png");
	Sprite::LoadTexture(184, L"Resources/2d/icon/motioncon.png");

	movecn =Sprite::Create(183, { 10, 10 });
	Attackcon = Sprite::Create(184, { 10, 10 });
	movecn->SetAnchorPoint({ 0.5,0.5 });
	Attackcon->SetAnchorPoint({ 0.5,0.5 });
	movecn->SetSize({ 400,400 });
	Attackcon->SetSize({ 400,400 });

	Task[HELLO] = Sprite::Create(171, { 10, 10 });
	TargetPoint_Ground = Texture::Create(50);
	TargetPoint_Icon = Texture::Create(51);

	TargetPoint_Ground->CreateTexture();
	TargetPoint_Icon->CreateTexture();
	TargetPoint_Ground->SetAnchorPoint({ 0.5f,0.5f });
	TargetPoint_Icon->SetAnchorPoint({ 0.5f,0.5f });

	Task[WALK] = Sprite::Create(174, { 10, 10 });
	Task[ATTACK] = Sprite::Create(175, { 10, 10 });
	Task[GETKEY] = Sprite::Create(176, { 10, 10 });
	Task[CLEAR] = Sprite::Create(177, { 10, 10 });

	
	targetAlpha = 1.f;
	TargetTexSize = { 4,4,3 };
	for (int i = 0; i < TaskNum; i++)
	{
		Task[i]->SetAnchorPoint({ 0, 0 });
		SpriteSizeX[i] = 1900.0f;
		SpriteAlpha[i] = 0.0f;
		MassageCheck[i] = false;
		ClearTaskJudg[i] = false;
	}
	
	Task[CLEAR]->SetSize({ 1000, 1000 });
	AllTaskClear = false;
	task = THELLO;
	Jump = false;
	Movement = 0;
}

void TutorialSprite::CheckMove_Camera_Player()
{

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
	conpos = { 1700,790 };
	movecn->SetPosition(conpos);
	Attackcon->SetPosition(conpos);
	movecn->setcolor({ 1,1,1,movea });
	Attackcon->setcolor({ 1,1,1,atacka });
	if (CustomButton::GetInstance()->GetJumpAction() == true)
	{
		Jump = true;
	}

	//歩きとジャンプ
	if (Collision::GetLength({ 109.f,-30,-650.f }, PlayerControl::GetInstance()->GetPlayer()->GetPosition()) < 10.f) {
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
		Task[HELLO]->SetPosition({ 400,200 });
		Task[HELLO]->SetSize({ 800,400 });

		SpriteAlpha[HELLO] = min(SpriteAlpha[HELLO], 3.f);
		SpriteAlpha[HELLO] = max(SpriteAlpha[HELLO], 0.f);
		//説明スプライトの拡縮
		if (SpriteAlpha[HELLO] >= 2.0) {
			task = TMOVE;
		}
		break;

	case TMOVE:

		SpriteAlpha[HELLO] -= 0.02f;
		CheckMove_Camera_Player();
		if (ClearTaskJudg[WALK])
		{
			NextTask(t[WALK], TATTACK, ClearTaskJudg[WALK]);
		}

		Ease_SpriteSize_Up(SpriteAlpha[WALK], t[WALK], WALK);
		break;
	case TATTACK:
		atacka += 0.02f;
		movea -= 0.02f;
		//攻撃
		CheckAttack();
		if (ClearTaskJudg[ATTACK])
		{
			NextTask(t[ATTACK], TGETKEY, ClearTaskJudg[ATTACK]);
		}

		Ease_SpriteSize_Up(SpriteAlpha[ATTACK], t[ATTACK], ATTACK);
		break;

	case TGETKEY:
		//	n//otTaskXpos[ENEMYDESTROY] += 30;
		//セッティング
		ClearTaskJudg[GETKEY] = ChestControl::GetInstance()->GetTutorialChest() == true;

		if (MassageCheck[GETKEY])
		{
			NextTask(t[GETKEY], TEND, ClearTaskJudg[GETKEY]);
		}

		Ease_SpriteSize_Up(SpriteSizeX[GETKEY], t[GETKEY], GETKEY);
		break;
	case TEND:
		if (MassageCheck[CLEAR])
		{
			NextTask(t[CLEAR], THELLO, false);
		}

		Ease_SpriteSize_Up(SpriteSizeX[CLEAR], t[CLEAR], CLEAR);
		break;
	default:
		break;
	}
	
	 
	for (int i = 1; i < 5; i++)
	{
		Task[i]->SetPosition(spos);
		Task[i]->SetSize({ 1000,300 });
		Task[i]->setcolor({ 1,1,1,SpriteAlpha[i] });
	}

	Task[HELLO]->setcolor({ 1,1,1,SpriteAlpha[HELLO] });
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

	if (ClearTaskJudg[WALK]) {
		TargetTexSize.x += 0.1f;
		TargetTexSize.y += 0.1f;
		targetAlpha -= 0.04f;
		if (targetAlpha <= 0.f) {
			PlayerControl::GetInstance()->GetPlayer()->SetStopFlag(FALSE);
		}
		else {
			PlayerControl::GetInstance()->GetPlayer()->SetStopFlag(TRUE);
		}
	}
	
	TargetPoint_Ground->SetPosition({ 109.f,-30,-650.f });
	TargetPoint_Ground->SetRotation({ 90,0,TargetPoint_GroundRotY });
	TargetPoint_Ground->SetScale(TargetTexSize);
	TargetPoint_Ground->SetBillboard(FALSE);
	TargetPoint_Ground->SetColor({ 1,1,1,targetAlpha });
	TargetPoint_Ground->Update(camera);

	if (!ClearTaskJudg[WALK]) {
		TargetIconPosY = -15 + sinf(3.14f * 2.f / 90.f * MoveTime) * -10;

		TargetPoint_Icon->SetPosition({ 109.f,TargetIconPosY,-650.f });
	}
	if(!ClearTaskJudg[ATTACK]&& ClearTaskJudg[WALK]){
		TargetIconPosY = -5 + sinf(3.14f * 2.f / 90.f * MoveTime) * 5;

		if (EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0] == nullptr)return;
		XMFLOAT3 EPos = EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0]->GetPosition();
		TargetPoint_Icon->SetPosition({ EPos.x,TargetIconPosY,EPos.z });
	}
	TargetPoint_Icon->SetRotation({ 180,0,0});
	TargetPoint_Icon->SetBillboard(TRUE);
	TargetPoint_Icon->SetScale({ 3,3,3 });
	TargetPoint_Icon->SetColor({ 1,1,1,1 });
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
	ImGui::Begin("pos");
	ImGui::SliderFloat("x", &spos.x,0,1200);
	ImGui::SliderFloat("y", &spos.y, 0, 1000);
	ImGui::End();
	Sprite::PreDraw();
	
	for (int i = 0; i < 5; i++)
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
		x = Easing::EaseOut(t, 0, 0.7);
		if (t >= 0.0f)
		{
			t -= 0.05f;
		}
		//PlayerControl::GetInstance()->GetPlayer()->SetStopFlag(false);
	} else
	{
		x = Easing::EaseOut(t, 0, 0.7);
		if (t <= 1.0f)
		{
			t += 0.05f;
		}
		//PlayerControl::GetInstance()->GetPlayer()->SetStopFlag(true);
		//UI::GetInstance()->SetTurnoffUIDraw(TRUE);
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
