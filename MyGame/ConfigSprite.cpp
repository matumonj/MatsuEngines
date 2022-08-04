#include "ConfigSprite.h"

ConfigSprite* ConfigSprite::GetInstance()
{
	static ConfigSprite instance;

	return &instance;
}
ConfigSprite::~ConfigSprite()
{
	delete RayOutSprite, EnemyHPSelectSprite;
}
void ConfigSprite::Initialize()
{
	Sprite::LoadTexture(122, L"Resources/hudrayout.png");
	Sprite::LoadTexture(123, L"Resources/keybound.png");

	RayOutSprite = Sprite::Create(122, { 1,1 });
	EnemyHPSelectSprite = Sprite::Create(123, { 1,1 });

	RayOutSprite->SetAnchorPoint({ 0.5f,0.5f });
	EnemyHPSelectSprite->SetAnchorPoint({ 0.5f,0.5f });
	RayOutSprite->SetPosition(SpritePosition[0]);
	EnemyHPSelectSprite->SetPosition(SpritePosition[1]);
	RayOutSprite->SetSize({ 600,600 });
	EnemyHPSelectSprite->SetSize({ 600,600 });
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