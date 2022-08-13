#include "TutorialSprite.h"
void TutorialSprite::Initialize()
{
	Sprite::LoadTexture(170, L"Resources/Tuto");
	Task[0]=Sprite::Create(170, { 10,10 });
}

void TutorialSprite::Update()
{

}

void TutorialSprite::Draw()
{

}