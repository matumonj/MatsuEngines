#include "ConfigSprite.h"

ConfigSprite* ConfigSprite::GetInstance()
{
	static ConfigSprite instance;

	return &instance;
}
void ConfigSprite::Initialize()
{
	Sprite::LoadTexture(123, L"Resources/lifegauge.png");
	
	RayOutSprite = Sprite::Create(123, { 1,1 });
	EnemyHPSelectSprite = Sprite::Create(123, { 1,1 });

	
	RayOutSprite->SetPosition(SpritePosition[0]);
	EnemyHPSelectSprite->SetPosition(SpritePosition[1]);
	RayOutSprite->SetSize({ 100,100 });
	EnemyHPSelectSprite->SetSize({ 100,100 });
}

void ConfigSprite::Update()
{

}

void ConfigSprite::Draw()
{
	//Sprite::PreDraw();
	RayOutSprite->Draw();
	EnemyHPSelectSprite->Draw();
	//Sprite::PostDraw();
}