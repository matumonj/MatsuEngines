#include "ImageManager.h"
#include "ImageManager.h"

ImageManager* ImageManager::GetIns()
{
	static ImageManager instans;
	return &instans;
}

void ImageManager::Load2D()
{
	// テクスチャ読み込み
	Sprite::LoadTexture(PLAYERHP, L"Resources/HPTex/HPgauge.png");
	Sprite::LoadTexture(PLAYERHPFRAME_INNER, L"Resources/HPTex/frame.png");
	Sprite::LoadTexture(PLAYERHPFRAME_BORDER, L"Resources/HPTex/frame2.png");

	Sprite::LoadTexture(ENMEYHP, L"Resources/bosshp.png");
	Sprite::LoadTexture(BOSSENEMYHP, L"Resources/bosshp.png");
	Sprite::LoadTexture(BOSSENEMYHPFRAME, L"Resources/bosshp.png");

	Sprite::LoadTexture(STARTANNOUNCE, L"Resources/tutorial1.png");
	Sprite::LoadTexture(MOVEANNOUNCE, L"Resources/tutorial2.png");
	Sprite::LoadTexture(CONFIGANNOUNCE, L"Resources/tutorial3.png");
	Sprite::LoadTexture(ATTACKANNOUNCE, L"Resources/tutorial4.png");
	Sprite::LoadTexture(CHESTANNOUNCE, L"Resources/tutorial5.png");
	Sprite::LoadTexture(ENDANNOUNCE, L"Resources/tutorial6.png");

	Sprite::LoadTexture(BOSSNAME, L"Resources/BossName.png");
	Sprite::LoadTexture(FIELDNAME, L"Resources/warning1.png");

	Sprite::LoadTexture(SKILL_1, L"Resources/jump.png");
	Sprite::LoadTexture(SKILL_2, L"Resources/attack.png");
	Sprite::LoadTexture(SKILL_3, L"Resources/attack2.png");
	Sprite::LoadTexture(SKILL_4, L"Resources/attack3.png");
	Sprite::LoadTexture(COOLDOWN, L"Resources/cool.png");

	Sprite::LoadTexture(CONFIG_CUSBUTON, L"Resources/custom.png");
	Sprite::LoadTexture(CONFIG_SELSWORD, L"Resources/swordselect.png");
	Sprite::LoadTexture(CONFIG_HUDLAYOUT, L"Resources/layout.png");
	Sprite::LoadTexture(CONFIG_SELBOX, L"Resources/04 選択肢/button_select2_3.png");
	Sprite::LoadTexture(CONFIG_MENU, L"Resources/03 開閉型メニュー/bg_menu.png");

	Sprite::LoadTexture(CHESTCOUNT, L"Resources/chestCollect1.png");
	Sprite::LoadTexture(CHESTCOUNT_FRAME, L"Resources/2d/Expadian.png");
	Sprite::LoadTexture(STITLE, L"Resources/chestColFrame.png");
}

void ImageManager::LoadTex2D()
{
	// テクスチャ読み込み
	Texture::LoadTexture(ENMEYHP_MULTI, L"Resources/bosshp.png");
	Texture::LoadTexture(PARTICLE, L"Resources/2d/SushiHP/CHP.png");

	Texture::LoadTexture(PARTICLE, L"Resources/2d/SushiHP/CHP.png");
	Texture::LoadTexture(SLASH, L"Resources/2d/SushiHP/CHP.png");
	Texture::LoadTexture(CELESFIELD, L"Resources/2d/SushiHP/CHP.png");
	Texture::LoadTexture(BOSSATK_CIRCLE, L"Resources/2d/SushiHP/CHP.png");
	Texture::LoadTexture(BOSSATK_HALF, L"Resources/2d/SushiHP/CHP.png");
	Texture::LoadTexture(BOSSATK_KNOCK, L"Resources/2d/SushiHP/CHP.png");
}
