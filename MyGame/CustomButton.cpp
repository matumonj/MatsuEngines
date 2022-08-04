#include "CustomButton.h"

CustomButton* CustomButton::GetInstance()
{
	static CustomButton instance;

	return &instance;
}
void CustomButton::Initialize()
{
	input = Input::GetInstance();
	Sprite::LoadTexture(125, L"Resources/images (1).png");//コントローラー画像
	Sprite::LoadTexture(126, L"Resources/xbutton.png");//Xボタン画像
	Sprite::LoadTexture(127, L"Resources/ybutton.png");//Yボタン画像
	Sprite::LoadTexture(128, L"Resources/abutton.png");//Aボタン画像
	Sprite::LoadTexture(129, L"Resources/bbutton.png");//Bボタン画像
	Sprite::LoadTexture(130, L"Resources/block.png");//枠画像
	Sprite::LoadTexture(131, L"Resources/jump.png");//
	Sprite::LoadTexture(132, L"Resources/jump.png");//
	Sprite::LoadTexture(140, L"Resources/selectsprite.png");//
	//コントローラー画像
	Controller= Sprite::Create(125, { 0.0f,-200.0f });
	Controller->SetPosition({ 900,300 });
	Controller->SetSize({ 500,500 });
	//セレクトスプライト
	SelectSprite = Sprite::Create(140, { 0.0f,-200.0f });
	SelectSprite->SetPosition(SelectSpritePosition[SelectNum]);
	SelectSprite->SetSize({ 100,100 });
	//各ボタンのスプライト
	ButtonSprite[BUTTON_X]= Sprite::Create(126, { 0.0f,-200.0f });
	ButtonSprite[BUTTON_Y]= Sprite::Create(127, { 0.0f,-200.0f });
	ButtonSprite[BUTTON_A]= Sprite::Create(128, { 0.0f,-200.0f });
	ButtonSprite[BUTTON_B]= Sprite::Create(129, { 0.0f,-200.0f });
	for (int i = 0; i < 4; i++) {
		ButtonSprite[i]->SetPosition({ 0,0 });
		ButtonSprite[i]->SetSize({ 100,100 });
	}
	//各ボタンを対応させるアクション項目スプライト
	/*JUMP*/
	MenuSprite[JUMP]= Sprite::Create(131, { 0.0f,-200.0f });
	MenuSprite[JUMP]->SetSize(MenuSpriteSize);
	MenuSprite[JUMP]->SetPosition(JumpSpritePosition);
	/*ATTACK*/
	MenuSprite[ATTACK] = Sprite::Create(131, { 0.0f,-200.0f });
	MenuSprite[ATTACK]->SetSize(MenuSpriteSize);
	MenuSprite[ATTACK]->SetPosition(AttackSpritePosition);
}

void CustomButton::Update()
{
	if (CustomButtonJudg) {

		if (input->TriggerCrossKey(input->Cross_Down)) {
			SelectNum++;
		} else if (input->TriggerCrossKey(input->Cross_Up)) {
			SelectNum--;
		}

		if (SelectNum == JUMP) {
			if (input->TriggerButton(input->Button_B)) {
				JumpCustomFlag = true;//ジャンプの設定処理へ
			}
		}
		SelectSprite->SetPosition(SelectSpritePosition[SelectNum]);
	}
	else {
		JumpCustomFlag = false;
		SelectNum = JUMP;
	}
}

void CustomButton::Draw()
{
	if (CustomButtonJudg) {

		Sprite::PreDraw();
		SelectSprite->Draw();
		MenuSprite[JUMP]->Draw();
		MenuSprite[ATTACK]->Draw();
		Controller->Draw();
		Custom_Draw();
		Sprite::PostDraw();
	}
}

void CustomButton::Custom_JumpButton()
{
	if(JumpCustomFlag){
	//設定部分
	if (input->TriggerButton(input->Button_A)) {
		actionButton[JUMP].selectButton = SBUTTON_A;
	}
	if (input->TriggerButton(input->Button_B)) {
		actionButton[JUMP].selectButton = SBUTTON_B;
	}
	if (input->TriggerButton(input->Button_X)) {
		actionButton[JUMP].selectButton = SBUTTON_X;
	}
	if (input->TriggerButton(input->Button_Y)) {
		actionButton[JUMP].selectButton = SBUTTON_Y;
	}
}

	//反映部分 少しややこしくなってしまった,,,
	switch (actionButton[JUMP].selectButton)
	{
	case SBUTTON_A:
		if (input->TriggerButton(input->Button_A)) {
			actionButton[JUMP].judg = true;
		}
		else {
			actionButton[JUMP].judg = false;
		}
		break;
	case SBUTTON_B:
		if (input->TriggerButton(input->Button_B)) {
			actionButton[JUMP].judg = true;
		}
		else {
			actionButton[JUMP].judg = false;
		}
		break;
	case SBUTTON_X:
		if (input->TriggerButton(input->Button_X)) {
			actionButton[JUMP].judg = true;
		}
		else {
			actionButton[JUMP].judg =false;
		}
		break;
	case SBUTTON_Y:
		if (input->TriggerButton(input->Button_Y)) {
			actionButton[JUMP].judg = true;
		}
		else {
			actionButton[JUMP].judg = false;
		}
		break;
	default:
		break;
		}
	
}

void CustomButton::Custom_AttackButton()
{

}

void CustomButton::Custom_Draw()
{
	if (JumpCustomFlag) {
		if (actionButton[JUMP].selectButton == SBUTTON_X) {
			ButtonSprite[BUTTON_X]->Draw();
		}
		if (actionButton[JUMP].selectButton == SBUTTON_Y) {
			ButtonSprite[BUTTON_Y]->Draw();
		}
		if (actionButton[JUMP].selectButton == SBUTTON_A) {
			ButtonSprite[BUTTON_A]->Draw();
		}
		if (actionButton[JUMP].selectButton == SBUTTON_B) {
			ButtonSprite[BUTTON_B]->Draw();
		}
	}
}