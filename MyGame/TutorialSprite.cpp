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
	//�ړ��ƃW�����v
	if (input->LeftTiltStick(input->Down) || input->LeftTiltStick(input->Up) || input->LeftTiltStick(input->Left) || input->LeftTiltStick(input->Right)) {
		Movement++;
	}
	if (CustomButton::GetInstance()->GetJumpAction() == true) {
		Jump = true;
	}

	//�����ƃW�����v
	ClearWalk = Movement > 100&&Jump==true;
	//�Z�b�e�B���O
	ClearSetting = SistemConfig::GetInstance()->GetConfigJudgMent() == false;
	//�U��
	ClearAttack = EnemyControl::GetInstance()->GetTutorialEnemyindex()[0]->GetHP() <= 0;
	//�I�[���R���v���[�g
	AllTaskClear = ClearWalk && ClearAttack && ClearSetting;

	FenceControl::GetInstance()->SetTutorialFenceOpen(AllTaskClear);
}

void TutorialSprite::Draw()
{

}