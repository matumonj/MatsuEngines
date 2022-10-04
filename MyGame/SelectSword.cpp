#include "SelectSword.h"
#include"BigSword.h"
#include"SmallSword.h"
#include"NormalSword.h"
#include"CameraControl.h"
#include"mHelper.h"
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
}

void SelectSword::SpriteSet()
{
	Sprite::LoadTexture(230, L"Resources/SwordA.png");
	Sprite::LoadTexture(231, L"Resources/swordB.png");
	Sprite::LoadTexture(232, L"Resources/swordC.png");

	Sprite::LoadTexture(233, L"Resources/SelSwordFrame.png");
	SwordSample[0] = Sprite::Create(230, { 1.0f,1.0f });
	SwordSample[1] = Sprite::Create(231, { 1.0f,1.0f });
	SwordSample[2] = Sprite::Create(232, { 1.0f,1.0f });
	Frame = Sprite::Create(233, { 1.0f,1.0f });
	for (int i = 0; i < 3; i++) {
		//フィールドにモデル割り当て
		//SwordSample[i]->Initialize();
		SwordSample[i]->SetSize({ 400.0f,400.0f });
		SwordSample[i]->SetAnchorPoint({ 0.5f,0.5f });
		SwordSample[i]->SetPosition({ Position[i] });
	}
	Frame->SetSize({ 450.0f,450.0f });
	Frame->SetPosition(Position[0]);
	Frame->SetAnchorPoint({ 0.5f,0.5f });
}

void SelectSword::pedestalSet()
{

}

void SelectSword::Initialize()
{
	Sword = std::make_unique<NormalSword>();
	Sword->Initialize(CameraControl::GetInstance()->GetCamera());
	
	SpriteSet();
	pedestalSet();
	input = Input::GetInstance();
	index = 0;
}

void SelectSword::SetSword(SwordScale nowsword)
{
	switch (nowsword)
	{
	case SelectSword::SMALL:
		Sword = std::make_unique<SmallSword>();
		break;
	case SelectSword::NORMAL:
		Sword = std::make_unique<NormalSword>();
		break;
	case SelectSword::BIG:
		Sword = std::make_unique<BigSword>();
		break;
	default:
		break;
	}
	Sword->Initialize(CameraControl::GetInstance()->GetCamera());
}

void SelectSword::Update()
{
	if (SelectJudg) {
		if (input->TriggerCrossKey(input->Cross_Right)) {
			index++;
		} else if (input->TriggerCrossKey(input->Cross_Left)) {
			index--;
		}

		if (index == 0) {
			NowSelectSword = SMALL;
		} else if (index == 1) {
			NowSelectSword = NORMAL;
		} else if (index == 2) {
			NowSelectSword = BIG;
		}
		if (input->TriggerButton(input->Button_A)) {
			SetSword(NowSelectSword);
		}
		Frame->SetPosition(Position[index]);
	}
	Sword->Update(CameraControl::GetInstance()->GetCamera());

	PedestalUpdate();

	index = min(index, 2);
	index = max(index, 0);
}

void SelectSword::PedestalUpdate()
{

}

void SelectSword::Draw()
{
	SpriteDraw();
}

void SelectSword::SpriteDraw()
{
	if (SelectJudg) {
		Sprite::PreDraw();

		for (int i = 0; i < 3; i++) {
			SwordSample[i]->Draw();
		}

		Frame->Draw();
		Sprite::PostDraw();
	}
}

void SelectSword::PedestalDraw()
{

}

void SelectSword::SwordDraw()
{
	Sword->Draw();
}

