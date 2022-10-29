#include "TutorialSprite.h"
#include"CustomButton.h"
#include"SistemConfig.h"
#include"EnemyControl.h"
#include"FenceControl.h"
#include"ChestControl.h"
#include"mHelper.h"
TutorialSprite* TutorialSprite::GetInstance()
{
	static TutorialSprite instance;
	return &instance;
}
void TutorialSprite::Finalize()
{
	for (int i = 0; i < TaskNum; i++) {
		delete Task[i];
	}
}

void TutorialSprite::Initialize()
{
	input = Input::GetInstance();
	Sprite::LoadTexture(171, L"Resources/2d/tutorialstep/step1.png");
	Sprite::LoadTexture(172, L"Resources/2d/tutorialstep/step2.png");
	Sprite::LoadTexture(173, L"Resources/2d/tutorialstep/step3.png");
	Sprite::LoadTexture(174, L"Resources/2d/tutorialstep/step4.png");
	Sprite::LoadTexture(175, L"Resources/2d/tutorialstep/step5.png");
	Sprite::LoadTexture(176, L"Resources/2d/tutorialstep/step6.png");
	Sprite::LoadTexture(170, L"Resources/Tuto");

	Sprite::LoadTexture(177, L"Resources/2d/tutorialstep/task_movechara.png");
	Sprite::LoadTexture(178, L"Resources/2d/tutorialstep/task_movecamera.png");
	Sprite::LoadTexture(179, L"Resources/2d/tutorialstep/task_attackenemy.png");
	Sprite::LoadTexture(180, L"Resources/2d/tutorialstep/task_config.png");

	Task[HELLO] = Sprite::Create(171, { 10,10 });
	Task[WALK] = Sprite::Create(172, { 10,10 });
	Task[SETTING] = Sprite::Create(173, { 10,10 });
	Task[ATTACK] = Sprite::Create(174, { 10,10 });
	Task[GETKEY] = Sprite::Create(175, { 10,10 });
	Task[CLEAR] = Sprite::Create(176, { 10,10 });

	notClearTask[MOVE_CHARA]= Sprite::Create(177, { 10,10 });
	notClearTask[MOVE_CAMERA] = Sprite::Create(178, { 10,10 });
	notClearTask[ENEMYDESTROY] = Sprite::Create(179, { 10,10 });
	notClearTask[CONFIG] = Sprite::Create(180, { 10,10 });

	for (int i = 0; i < TaskNum; i++) {
		Task[i]->SetAnchorPoint({ 0,0 });
		SpriteSizeX[i] = 0;
		MassageCheck[i]=false;
		ClearTaskJudg[i] = false;
	}
	for (int i = 0; i < 4; i++) {
		notTaskXpos[i] = -1000;
		notClearTask[i]->SetPosition({ -300.0f,0.0f });
	}
	AllTaskClear=false;
	task = THELLO;
	Jump = false;
	Movement = 0;
}

void TutorialSprite::CheckMove_Camera_Player()
{
	//移動とジャンプ
	if (input->TiltPushStick(Input::L_UP, 0.0f) ||
		input->TiltPushStick(Input::L_DOWN, 0.0f) ||
		input->TiltPushStick(Input::L_RIGHT, 0.0f) ||
		input->TiltPushStick(Input::L_LEFT, 0.0f)) {

		//	if (input->LeftTiltStick(input->Down) || input->LeftTiltStick(input->Up) || input->LeftTiltStick(input->Left) || input->LeftTiltStick(input->Right)) {
		Movement++;
	}
	if (input->TiltPushStick(Input::R_UP, 0.0f) ||
		input->TiltPushStick(Input::R_DOWN, 0.0f) ||
		input->TiltPushStick(Input::R_RIGHT, 0.0f) ||
		input->TiltPushStick(Input::R_LEFT, 0.0f)) {
		Movement_Camera++;
	}
	if (Movement < 180) {
		notTaskXpos[MOVE_CHARA] += 30;
	} else {
		notTaskXpos[MOVE_CHARA] -= 30;
	}
	if (Movement_Camera < 180) {
		notTaskXpos[MOVE_CAMERA] += 30;
	} else {
		notTaskXpos[MOVE_CAMERA] -= 30;
	}
}
void TutorialSprite::Update()
{

	if (CustomButton::GetInstance()->GetJumpAction() == true) {
		Jump = true;
	}
	
	//歩きとジャンプ
	ClearTaskJudg[WALK]= Movement > 180&& Movement_Camera > 180;
	
	//オールコンプリート
	AllTaskClear = ClearTaskJudg[WALK] && ClearTaskJudg[SETTING] && ClearTaskJudg[ATTACK]&& ClearTaskJudg[GETKEY];
	//チュートリアルの宝箱
	ClearTaskJudg[GETKEY]= ChestControl::GetInstance()->GetTutorialChest() == true;
	
	FenceControl::GetInstance()->SetTutorialFenceOpen(AllTaskClear);
	switch (task)
	{
	case TutorialSprite::THELLO:
		if (MassageCheck[HELLO]) {
			//次のタスクへの条件(３引数)
			NextTask(t[HELLO], TMOVE, true);
		}
		//説明スプライトの拡縮
		Ease_SpriteSize_Up(SpriteSizeX[HELLO], t[HELLO], HELLO);
		break;

	case TutorialSprite::TMOVE:
		CheckMove_Camera_Player();
		if (MassageCheck[WALK]) {
			NextTask(t[WALK], TATTACK, ClearTaskJudg[WALK]);
		}
		
		Ease_SpriteSize_Up(SpriteSizeX[WALK], t[WALK], WALK);
		break;

	case TutorialSprite::TSETTING:
		
		//セッティング
		ClearTaskJudg[SETTING] = SistemConfig::GetInstance()->GetEndConfig();
		
		if (MassageCheck[SETTING]) {
			NextTask(t[SETTING], TGETKEY,ClearTaskJudg[SETTING]);
		}

		Ease_SpriteSize_Up(SpriteSizeX[SETTING], t[SETTING], SETTING);
		break;

	case TutorialSprite::TATTACK:
		notTaskXpos[MOVE_CHARA] -= 30;
		notTaskXpos[MOVE_CAMERA] -= 30;
		//攻撃
		if (EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0] != nullptr) {
			ClearTaskJudg[ATTACK]= EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL)[0]->GetHP() <= 0;
		}
		if (MassageCheck[ATTACK]) {
			NextTask(t[ATTACK], TGETKEY, ClearTaskJudg[ATTACK]);
		}

		Ease_SpriteSize_Up(SpriteSizeX[ATTACK], t[ATTACK], ATTACK);
		break;

	case TutorialSprite::TGETKEY:
		if (MassageCheck[GETKEY]) {
			NextTask(t[GETKEY], TEND,ClearTaskJudg[GETKEY]);
		}

		Ease_SpriteSize_Up(SpriteSizeX[GETKEY], t[GETKEY], GETKEY);
		break;
	case TutorialSprite::TEND:
		if (MassageCheck[CLEAR]) {
			NextTask(t[CLEAR],THELLO,false);
		}

		Ease_SpriteSize_Up(SpriteSizeX[CLEAR], t[CLEAR], CLEAR);
		break;
	default:
		break;
	}
	for (int i = 0; i < TaskNum; i++) {
		Task[i]->SetSize({ SpriteSizeX[i],1000 });
	}
	for (int i = 0; i < 4; i++) {
		notTaskXpos[i] = min(notTaskXpos[i], 0);
		notTaskXpos[i] = max(notTaskXpos[i], -1000);
		notClearTask[i]->SetPosition({ notTaskXpos[i],0});
		notClearTask[i]->SetSize({ 1300,800 });
	}
}

void TutorialSprite::Draw()
{
	Sprite::PreDraw();
	for (int i = 0; i < 4; i++) {
		notClearTask[i]->Draw();
	}
	for (int i = 0; i < TaskNum; i++) {
		Task[i]->Draw();
	}
	
	Sprite::PostDraw();
}

void TutorialSprite::Ease_SpriteSize_Up(float& x, float& t, int index)
{

	if (input->TriggerButton(input->A)) {
		MassageCheck[index] = true;
	}
	if (MassageCheck[index]) {
		x = Easing::EaseOut(t, 0, 1900);
		if (t >= 0.0f) {
			t -= 0.05f;
		}
	} else {
		x = Easing::EaseOut(t, 0, 1900);
		if (t <= 1.0f) {
			t += 0.05f;
		}
	}
}

void TutorialSprite::NextTask(float t, TaskMenu nexttask, bool nextjudg)
{
	if (t <= 0.0f) {
		if (nextjudg) {
			task = nexttask;
		}
	}
}

TutorialSprite::~TutorialSprite()
{
	for (int i = 0; i < TaskNum; i++) {
		delete Task[i];
	}
	
}