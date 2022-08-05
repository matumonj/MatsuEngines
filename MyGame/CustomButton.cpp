#include "CustomButton.h"

CustomButton* CustomButton::GetInstance()
{
	static CustomButton instance;

	return &instance;
}
void CustomButton::Initialize()
{
	input = Input::GetInstance();
	Sprite::LoadTexture(125, L"Resources/con.png");//コントローラー画像
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
	SelectSprite->SetSize({ 200,200 });
	//コントローラーボタンのスプライト(各アクションごとに用意)
	for (int i = 0; i < menuNum; i++) {//i={JUMP,ATTACK,}
		ButtonSprite[i][BUTTON_X] = Sprite::Create(126, { 0.0f,-200.0f });
		ButtonSprite[i][BUTTON_Y] = Sprite::Create(127, { 0.0f,-200.0f });
		ButtonSprite[i][BUTTON_A] = Sprite::Create(128, { 0.0f,-200.0f });
		ButtonSprite[i][BUTTON_B] = Sprite::Create(129, { 0.0f,-200.0f });
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
	
	//ボタン囲う枠画像、コントローラーボタン画像
	//ほんとは[i],[j]とかより[JUMP][Button_A]とか明示的に書いてあげたほうがわかりやすけど,,,行数削減のため
	for (int j = 0; j < menuNum; j++) {
		ButtonFrame[j] = Sprite::Create(130, { 0.0f,-200.0f });
		ButtonFrame[j]->SetPosition({ MenuSprite[j]->GetPosition().x + 100,MenuSprite[j]->GetPosition().y });
		ButtonFrame[j]->SetSize({ 400,300 });
		
		for (int i = 0; i < ActionMax; i++) {
			ButtonSprite[j][i]->SetPosition(ButtonFrame[j]->GetPosition());
			ButtonSprite[j][i]->SetSize({ 400,400 });
		}
	}
	//初期各アクション対応ボタン
	actionButton[JUMP].selectButton = SBUTTON_A;
	actionButton[ATTACK].selectButton = SBUTTON_B;
}

void CustomButton::Update()
{
	if (CustomButtonJudg) {
		/*設定画面入った瞬間は番号割当なし(設定画面はいるのにAボタン押すけどジャンプボタンも瞬間的にAボタンに
		切り替わるのを防ぐため)*/
		if (SelectNum == NONE) {
			if (input->TriggerCrossKey(input->Cross_Down) || input->TriggerCrossKey(input->Cross_Up)) {
				SelectNum = JUMP;
			}
		}
		else {
			if (input->TriggerCrossKey(input->Cross_Down)) {
				SelectNum++;
			} else if (input->TriggerCrossKey(input->Cross_Up)) {
				SelectNum--;
			}
			SelectSprite->SetPosition(SelectSpritePosition[SelectNum]);
		}

		//ここはリファクタリング
		if (SelectNum == JUMP) {
			JumpCustomFlag = true;//ジャンプの設定処理へ
		}
		else {
			JumpCustomFlag = false;
		}
		if (SelectNum == ATTACK) {
			AttackCustomFlag = true;//ジャンプの設定処理へ
		} else {
			AttackCustomFlag = false;
		}
	}
	else {//設定閉じたら、各設定フラグ切る、セレクト番号の割当もなくす
		JumpCustomFlag = false;
		AttackCustomFlag = false;
		SelectNum =NONE;
	}

	Custom_Button(JumpCustomFlag, JUMP);
	Custom_Button(AttackCustomFlag, ATTACK);
}

//(各アクションボタン設定フラグ,対応アクション番号)flagとbuttonは同じアクションに
void CustomButton::Custom_Button(bool customflag, int index)
{
	if(customflag){
	//設定部分
	if (input->TriggerButton(input->Button_A)) {
		actionButton[index].selectButton = SBUTTON_A;
	}
	if (input->TriggerButton(input->Button_B)) {
		actionButton[index].selectButton = SBUTTON_B;
	}
	if (input->TriggerButton(input->Button_X)) {
		actionButton[index].selectButton = SBUTTON_X;
	}
	if (input->TriggerButton(input->Button_Y)) {
		actionButton[index].selectButton = SBUTTON_Y;
	}
}

	//反映部分 少しややこしくなってしまった,,,言うならジャンプトリガーみたいな
	switch (actionButton[index].selectButton)
	{
	case SBUTTON_A:
		if (input->TriggerButton(input->Button_A)) {
			actionButton[index].judg = true;
		}
		else {
			actionButton[index].judg = false;
		}
		break;
	case SBUTTON_B:
		if (input->TriggerButton(input->Button_B)) {
			actionButton[index].judg = true;
		}
		else {
			actionButton[index].judg = false;
		}
		break;
	case SBUTTON_X:
		if (input->TriggerButton(input->Button_X)) {
			actionButton[index].judg = true;
		}
		else {
			actionButton[index].judg =false;
		}
		break;
	case SBUTTON_Y:
		if (input->TriggerButton(input->Button_Y)) {
			actionButton[index].judg = true;
		}
		else {
			actionButton[index].judg = false;
		}
		break;
	default:
		break;
		}
	
}

void CustomButton::Custom_Draw()
{
		for (int i = 0; i < menuNum; i++) {
			if (actionButton[i].selectButton == SBUTTON_X) {
				ButtonSprite[i][BUTTON_X]->Draw();
			}
			if (actionButton[i].selectButton == SBUTTON_Y) {
				ButtonSprite[i][BUTTON_Y]->Draw();
			}
			if (actionButton[i].selectButton == SBUTTON_A) {
				ButtonSprite[i][BUTTON_A]->Draw();
			}
			if (actionButton[i].selectButton == SBUTTON_B) {
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
		Sprite::PostDraw();
	}
}