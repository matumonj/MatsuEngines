#include "SelectSword.h"
#include"BigSword.h"
#include"SmallSword.h"
#include"CameraControl.h"

void SelectSword::Initialize()
{
	Sprite::LoadTexture(230, L"Resources/sword.png");
	Sprite::LoadTexture(230, L"Resources/sword.png");
	Sprite::LoadTexture(230, L"Resources/sword.png");
	SwordSample[0] = Sprite::Create(230, { 1,1 });
	SwordSample[0] = Sprite::Create(230, { 1,1 });
	SwordSample[0] = Sprite::Create(230, { 1,1 });

	for (int i = 0; i < 3; i++) {
		//フィールドにモデル割り当て
		SwordSample[i]->Initialize();
	}
}
void SelectSword::SetSmallSword(SwordScale nowsword)
{
	switch (nowsword)
	{
	case SelectSword::SMALL:
		Sword = std::make_unique<SmallSword>();
		break;
	case SelectSword::NORMAL:
		Sword = std::make_unique<BigSword>();
		break;
	case SelectSword::BIG:
		Sword = std::make_unique<BigSword>();
		break;
	default:
		break;
	}
	//
	//Sword = std::make_unique<SmallSword>();
	Sword->Initialize(CameraControl::GetInstance()->GetCamera());
}

void SelectSword::Update()
{

}

void SelectSword::Draw()
{
	Sprite::PreDraw();
	for (int i = 0; i < 3; i++) {
		SwordSample[i]->Draw();
	}
	Sprite::PostDraw();
}