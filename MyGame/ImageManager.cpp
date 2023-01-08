#include "ImageManager.h"
#include "ImageManager.h"

std::map<ImageManager::tex2dName, Sprite*> ImageManager::tex;

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

	Sprite::LoadTexture(BOSSENEMYHP1, L"Resources/bosshp.png");
	Sprite::LoadTexture(BOSSENEMYHPFRAME, L"Resources/bosshp.png");

	Sprite::LoadTexture(STARTANNOUNCE, L"Resources/tutorial1.png");
	Sprite::LoadTexture(MOVEANNOUNCE, L"Resources/tutorial2.png");
	Sprite::LoadTexture(CONFIGANNOUNCE, L"Resources/tutorial3.png");
	Sprite::LoadTexture(ATTACKANNOUNCE, L"Resources/tutorial4.png");
	Sprite::LoadTexture(CHESTANNOUNCE, L"Resources/tutorial5.png");
	Sprite::LoadTexture(ENDANNOUNCE, L"Resources/tutorial6.png");

	Sprite::LoadTexture(BOSSNAME, L"Resources/BossName.png");
	Sprite::LoadTexture(FIELDNAME, L"Resources/warning1.png");

	Sprite::LoadTexture(CONFIG_CUSBUTON, L"Resources/custom.png");
	Sprite::LoadTexture(CONFIG_SELSWORD, L"Resources/swordselect.png");
	Sprite::LoadTexture(CONFIG_HUDLAYOUT, L"Resources/layout.png");
	Sprite::LoadTexture(CONFIG_SELBOX, L"Resources/04 選択肢/button_select2_3.png");
	Sprite::LoadTexture(CONFIG_MENU, L"Resources/03 開閉型メニュー/bg_menu.png");

	Sprite::LoadTexture(CHESTCOUNT, L"Resources/chestCollect1.png");
	Sprite::LoadTexture(CHESTCOUNT_FRAME, L"Resources/2d/Expadian.png");
	Sprite::LoadTexture(STITLE, L"Resources/chestColFrame.png");

	Sprite::LoadTexture(GAMECLEAR_G, L"Resources/2d/ClearTex/g.png");
	Sprite::LoadTexture(GAMECLEAR_A, L"Resources/2d/ClearTex/a.png");
	Sprite::LoadTexture(GAMECLEAR_M, L"Resources/2d/ClearTex/m.png");
	Sprite::LoadTexture(GAMECLEAR_E, L"Resources/2d/ClearTex/e.png");
	Sprite::LoadTexture(GAMECLEAR_C, L"Resources/2d/ClearTex/c.png");
	Sprite::LoadTexture(GAMECLEAR_L, L"Resources/2d/ClearTex/l.png");
	Sprite::LoadTexture(GAMECLEAR_R, L"Resources/2d/ClearTex/r.png");
}

void ImageManager::Init()
{
	LoadTex2D(ENMEYHPFRAME1, L"Resources/2d/enemy/hpframe1.png");
	LoadTex2D(ENMEYHPFRAME2, L"Resources/2d/enemy/hpframe2.png");
	LoadTex2D(ENMEYHPFRAME3, L"Resources/2d/enemy/hpframe3.png");
	LoadTex2D(ENMEYHPFRAME4, L"Resources/2d/enemy/hpframe4.png");
	LoadTex2D(ENEMYNAME_GOLEM, L"Resources/2d/enemy/name_golem.png");
	LoadTex2D(ENEMYNAME_MINIGOLEM, L"Resources/2d/enemy/name_puchigolem.png");
	LoadTex2D(ENEMYNAME_LIZARD, L"Resources/2d/enemy/name_lizard.png");
	LoadTex2D(ENEMYNAME_GUARDIAN, L"Resources/2d/enemy/name_guardian.png");

	LoadTex2D(TITLE1, L"Resources/2d/title/titlesp.png");
	LoadTex2D(TITLE2, L"Resources/title2.png");

	LoadTex2D(GAMEPLAY, L"Resources/2d/title/gameplay.png");

	LoadTex2D(BOSSHPFRAME, L"Resources/HPTex/frame3.png");
	LoadTex2D(BOSSHPFRAMEINNER, L"Resources/HPTex/yellow.png");
	LoadTex2D(BOSSHPFRAMEINNER2, L"Resources/HPTex/red.png");
}

void ImageManager::LoadTex2D(const tex2dName imageName, wchar_t* fileName)
{
	Sprite::LoadTexture(imageName, fileName);
}
