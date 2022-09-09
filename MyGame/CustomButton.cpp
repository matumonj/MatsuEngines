#include "CustomButton.h"
#include"HUDLayOut.h"
CustomButton* CustomButton::GetInstance()
{
	static CustomButton instance;

	return &instance;
}

CustomButton::~CustomButton()
{
	delete Controller, SelectSprite, ErrorSprite, Custommenu;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			delete ButtonSprite[i][j];
		}
	}
	for (int i = 0; i < 4; i++) {
		delete MenuSprite[i], ButtonFrame[i];
	}
}
void CustomButton::Initialize()
{
	input = Input::GetInstance();
	Sprite::LoadTexture(125, L"Resources/con.png");//�R���g���[���[�摜
	Sprite::LoadTexture(126, L"Resources/xbutton.png");//X�{�^���摜
	Sprite::LoadTexture(127, L"Resources/ybutton.png");//Y�{�^���摜
	Sprite::LoadTexture(128, L"Resources/abutton.png");//A�{�^���摜
	Sprite::LoadTexture(129, L"Resources/bbutton.png");//B�{�^���摜
	Sprite::LoadTexture(130, L"Resources/05 �{�^��/button.png");//�g�摜
	Sprite::LoadTexture(131, L"Resources/jumpaction.png");//
	Sprite::LoadTexture(132, L"Resources/attackaction.png");//
	Sprite::LoadTexture(140, L"Resources/selectsprite.png");//
	Sprite::LoadTexture(141, L"Resources/error.png");//
	Sprite::LoadTexture(142, L"Resources/custommenu.png");//
	//�R���g���[���[�摜
	Controller = Sprite::Create(125, { 0.0f,-200.0f });
	Controller->SetPosition({ 900,600 });
	Controller->SetSize({ 500,500 });
	//�Z���N�g�X�v���C�g
	SelectSprite = Sprite::Create(140, { 0.0f,-200.0f });
	SelectSprite->SetPosition(SelectSpritePosition[SelectNum]);
	SelectSprite->SetSize({ 200,200 });

	Custommenu = Sprite::Create(142, { 0.0f,-200.0f });
	Custommenu->SetPosition({500,-650});
	Custommenu->SetSize({ 1000,1500 });

	//�R���g���[���[�{�^���̃X�v���C�g(�e�A�N�V�������Ƃɗp��)
	for (int i = 0; i < menuNum; i++) {//i={JUMP,ATTACK,}
		ButtonSprite[i][BUTTON_X] = Sprite::Create(126, { 0.0f,-200.0f });
		ButtonSprite[i][BUTTON_Y] = Sprite::Create(127, { 0.0f,-200.0f });
		ButtonSprite[i][BUTTON_A] = Sprite::Create(128, { 0.0f,-200.0f });
		ButtonSprite[i][BUTTON_B] = Sprite::Create(129, { 0.0f,-200.0f });
	}
	//�e�{�^����Ή�������A�N�V�������ڃX�v���C�g
	/*JUMP*/
	MenuSprite[JUMP] = Sprite::Create(131, { 0.0f,-200.0f });
	MenuSprite[JUMP]->SetSize(MenuSpriteSize);
	MenuSprite[JUMP]->SetPosition(JumpSpritePosition);
	/*ATTACK*/
	MenuSprite[ATTACK] = Sprite::Create(132, { 0.0f,-200.0f });
	MenuSprite[ATTACK]->SetSize(MenuSpriteSize);
	MenuSprite[ATTACK]->SetPosition(AttackSpritePosition);

	MenuSprite[SATTACK] = Sprite::Create(131, { 0.0f,-200.0f });
	MenuSprite[SATTACK]->SetSize(MenuSpriteSize);
	MenuSprite[SATTACK]->SetPosition(SAttackSpritePosition);

	MenuSprite[TATTACK] = Sprite::Create(131, { 0.0f,-200.0f });
	MenuSprite[TATTACK]->SetSize(MenuSpriteSize);
	MenuSprite[TATTACK]->SetPosition(TAttackSpritePosition);

	//�{�^���͂��g�摜�A�R���g���[���[�{�^���摜
	//�ق�Ƃ�[i],[j]�Ƃ����[JUMP][Button_A]�Ƃ������I�ɏ����Ă������ق����킩��₷����,,,�s���팸�̂���
	for (int j = 0; j < menuNum; j++) {
		ButtonFrame[j] = Sprite::Create(130, { 0.0f,-200.0f });
		ButtonFrame[j]->SetPosition({ MenuSprite[j]->GetPosition().x + 400,MenuSprite[j]->GetPosition().y+80 });
		ButtonFrame[j]->SetSize({ 400,150 });

		for (int i = 0; i < ActionMax; i++) {
			ButtonSprite[j][i]->SetPosition({ ButtonFrame[j]->GetPosition().x+100,ButtonFrame[j]->GetPosition().y-50 });
			ButtonSprite[j][i]->SetSize({ 300,300 });
			ButtonSprite[j][i]->SetAnchorPoint({ 0,0});
		}
	}
	ErrorSprite = Sprite::Create(141, { 0.0f,-200.0f });
	ErrorSprite->SetPosition({ 900,100 });
	ErrorSprite->SetSize({ 700,700 });
	//�����e�A�N�V�����Ή��{�^��
	actionButton[JUMP].selectButton_Before = SBUTTON_A;
	actionButton[ATTACK].selectButton_Before = SBUTTON_B;
	actionButton[SATTACK].selectButton_Before = SBUTTON_X;
	actionButton[TATTACK].selectButton_Before = SBUTTON_Y;
}

void CustomButton::Update()
{
	//HUDLayOut::GetInstance()->SetLayOutMode(false);
	if (CustomButtonJudg) {
		/*�ݒ��ʓ������u�Ԃ͔ԍ������Ȃ�(�ݒ��ʂ͂���̂�A�{�^���������ǃW�����v�{�^�����u�ԓI��A�{�^����
		�؂�ւ��̂�h������)*/
		if (SelectNum == NONE) {
			if (input->TriggerCrossKey(input->Cross_Down) || input->TriggerCrossKey(input->Cross_Up)) {
				SelectNum = JUMP;
			}
		} else {
			if (input->TriggerCrossKey(input->Cross_Down)) {
				SelectNum++;
			} else if (input->TriggerCrossKey(input->Cross_Up)) {
				SelectNum--;
			}
			SelectSprite->SetPosition(SelectSpritePosition[SelectNum]);
		}
		//�W�����v�{�^���ݒ�t���O����
		JumpCustomFlag = (SelectNum == JUMP);
		//�{�^���ݒ�t���O����
		AttackCustomFlag = (SelectNum == ATTACK);
		//�{�^���ݒ�t���O����
		SAttackCustomFlag = (SelectNum == SATTACK);
		//�{�^���ݒ�t���O����
		TAttackCustomFlag = (SelectNum == TATTACK);

		Custom_Button(JumpCustomFlag, JUMP);
		Custom_Button(AttackCustomFlag, ATTACK);
		Custom_Button(SAttackCustomFlag, SATTACK);
		Custom_Button(TAttackCustomFlag, TATTACK);
		if (Check() == false) {
			ErrorFlag = true;
		}
		else {
			ErrorFlag = false;
		}
	} else {//�ݒ������A�e�ݒ�t���O�؂�A�Z���N�g�ԍ��̊������Ȃ���
			ErrorJudg();/*memo:������check�̊֐��񂷂̑��������Ȃ��̂Ŏ��s�����牽������̃g���K�[�ň�u�Ő؂�*/
		
		SelectNum = NONE;

	}

	//for���ň͂��Ă��������ǌ�Ō��Ă��킩��₷���悤�ɍ��͂�������

	ConsiderationButton(JUMP);

	ConsiderationButton(ATTACK);

	ConsiderationButton(SATTACK);

	ConsiderationButton(TATTACK);

	//�{�^���d�����x���̃t�F�[�h
	if (ErrorFlag) {
		ErrorSpriteAlpha += 0.04f;
	}
	else {
		ErrorSpriteAlpha -= 0.04f;
	}
	ErrorSprite->setcolor({ 1,1,1,ErrorSpriteAlpha });
	ErrorSpriteAlpha = min(ErrorSpriteAlpha, 1);
	ErrorSpriteAlpha = max(ErrorSpriteAlpha, 0);
	SelectNum = min(SelectNum, 4);
	SelectNum = max(SelectNum, 0);
}

//(�e�A�N�V�����{�^���ݒ�t���O,�Ή��A�N�V�����ԍ�)flag��button�͓����A�N�V������
void CustomButton::Custom_Button(bool customflag, int index)
{
	if (customflag) {
		//�ݒ蕔��
		if (input->TriggerButton(input->Button_A)) {
			actionButton[index].selectButton_Before = SBUTTON_A;
		}
		if (input->TriggerButton(input->Button_B)) {
			actionButton[index].selectButton_Before = SBUTTON_B;
		}
		if (input->TriggerButton(input->Button_X)) {
			actionButton[index].selectButton_Before = SBUTTON_X;
		}
		if (input->TriggerButton(input->Button_Y)) {
			actionButton[index].selectButton_Before = SBUTTON_Y;
		}
	}
}

void CustomButton::ConsiderationButton(int index)
{
	//���f���� ������₱�����Ȃ��Ă��܂���,,,�����Ȃ�W�����v�g���K�[�݂�����
	switch (actionButton[index].selectButton_Before)
	{
	case SBUTTON_A:
		if (input->TriggerButton(input->Button_A)) {
			actionButton[index].judg = true;
		} else {
			actionButton[index].judg = false;
		}
		break;
	case SBUTTON_B:
		if (input->TriggerButton(input->Button_B)) {
			actionButton[index].judg = true;
		} else {
			actionButton[index].judg = false;
		}
		break;
	case SBUTTON_X:
		if (input->TriggerButton(input->Button_X)) {
			actionButton[index].judg = true;
		} else {
			actionButton[index].judg = false;
		}
		break;
	case SBUTTON_Y:
		if (input->TriggerButton(input->Button_Y)) {
			actionButton[index].judg = true;
		} else {
			actionButton[index].judg = false;
		}
		break;
	}
}

void CustomButton::Custom_Draw()
{
	for (int i = 0; i < menuNum; i++) {
		if (actionButton[i].selectButton_Before == SBUTTON_X) {
			ButtonSprite[i][BUTTON_X]->Draw();
		}
		if (actionButton[i].selectButton_Before == SBUTTON_Y) {
			ButtonSprite[i][BUTTON_Y]->Draw();
		}
		if (actionButton[i].selectButton_Before == SBUTTON_A) {
			ButtonSprite[i][BUTTON_A]->Draw();
		}
		if (actionButton[i].selectButton_Before == SBUTTON_B) {
			ButtonSprite[i][BUTTON_B]->Draw();
		}
	}

}

void CustomButton::Draw()
{
	if (CustomButtonJudg) {

		Sprite::PreDraw();
		Controller->Draw();
		if (SelectNum != NONE) {
			SelectSprite->Draw();
		}
		for (int i = 0; i < menuNum; i++) {
			MenuSprite[i]->Draw();
			ButtonFrame[i]->Draw();
		}
		Custom_Draw();
		Custommenu->Draw();
		//if (ErrorFlag) {
			ErrorSprite->Draw();
		//}
		Sprite::PostDraw();
	}
}

void CustomButton::ErrorJudg()
{
	for (int i = 0; i < 4; i++) {
		if (Check() == true) {//���Ȃ���Ό���̃{�^���z�u��ۑ�
			actionButton[i].OldselectButton = actionButton[i].selectButton_Before;
		} else {//��肪�������ꍇ�ȑO�ۑ����ꂽ��肪�Ȃ��{�^���z�u��K�p�i�v�͕ύX���e��j���j
			actionButton[i].selectButton_Before = actionButton[i].OldselectButton;
			actionButton[i].OldselectButton = actionButton[i].selectButton_Before;
		}
	}
}

bool CustomButton::Check()
{
	for (int i = 0; i < menuNum - 1; ++i) {
		for (int j = i + 1; j < menuNum; ++j) {
			if (actionButton[i].selectButton_Before == actionButton[j].selectButton_Before) {
				return false;
			}
		}
	}
	return true;
}