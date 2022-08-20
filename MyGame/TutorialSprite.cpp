#include "TutorialSprite.h"
#include"CustomButton.h"
#include"SistemConfig.h"
#include"EnemyControl.h"
#include"FenceControl.h"
TutorialSprite* TutorialSprite::GetInstance()
{
	static TutorialSprite instance;
	return &instance;
}
void TutorialSprite::Initialize()
{
	input = Input::GetInstance();
	Sprite::LoadTexture(170, L"Resources/Tuto");
	Task[0]=Sprite::Create(170, { 10,10 });
}

void TutorialSprite::Update()
{
	//移動とジャンプ
	if (input->LeftTiltStick(input->Down) || input->LeftTiltStick(input->Up) || input->LeftTiltStick(input->Left) || input->LeftTiltStick(input->Right)) {
		Movement++;
	}
	if (CustomButton::GetInstance()->GetJumpAction() == true) {
		Jump = true;
	}

	//歩きとジャンプ
	ClearWalk = Movement > 100&&Jump==true;
	//セッティング
	ClearSetting = SistemConfig::GetInstance()->GetConfigJudgMent() == false;
	//攻撃
	ClearAttack = EnemyControl::GetInstance()->GetTutorialEnemyindex()[0]->GetHP() <= 0;
	//オールコンプリート
	AllTaskClear = ClearWalk && ClearAttack && ClearSetting;

	FenceControl::GetInstance()->SetTutorialFenceOpen(AllTaskClear);
}

void TutorialSprite::Draw()
{

}