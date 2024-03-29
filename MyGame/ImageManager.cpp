#include "ImageManager.h"
#include "ImageManager.h"

std::map<ImageManager::SpriteName, Sprite*> ImageManager::sprites;
std::map<ImageManager::TexName, Texture*> ImageManager::texs;

ImageManager* ImageManager::GetIns()
{
	static ImageManager instans;
	return &instans;
}

void ImageManager::LoadTex(TexName imageName, wchar_t* fileName)
{
	Texture::LoadTexture(imageName, fileName);
}

void ImageManager::TitleSpriteInit()
{
	LoadSprite(TITLE1, L"Resources/2d/title/titlesp.png");
	LoadSprite(TITLE2, L"Resources/title2.png");

	
	LoadSprite(LOAD, L"Resources/2d/title/load.png");

	LoadSprite(LOADWORD_1, L"Resources/2d/title/load1.png");
		LoadSprite(LOADWORD_2, L"Resources/2d/title/load2.png");
			LoadSprite(LOADWORD_3, L"Resources/2d/title/load3.png");
				LoadSprite(LOADWORD_4, L"Resources/2d/title/load4.png");
					LoadSprite(LOADWORD_5, L"Resources/2d/title/load5.png");
						LoadSprite(LOADWORD_6, L"Resources/2d/title/load6.png");
							LoadSprite(LOADWORD_7, L"Resources/2d/title/load7.png");
								LoadSprite(LOADWORD_8, L"Resources/2d/title/load8.png");
									LoadSprite(LOADWORD_9, L"Resources/2d/title/load9.png");
									LoadSprite(LOADWORD_10, L"Resources/2d/title/load10.png");
									LoadSprite(AREASELECT_BOSS, L"Resources/2d/title/AreaSelect.png");
									LoadSprite(AREASELECT_EXPLO, L"Resources/2d/title/AreaSelect_2.png");
									LoadSprite(AREASELECT_FRAME, L"Resources/2d/title/AreaSelect_Frame.png");
}
void ImageManager::Init()
{
	LoadTex(NAV1, L"Resources/2d/icon/nav1.png");
	LoadTex(NAV2, L"Resources/2d/icon/nav2.png");
	LoadTex(NAV3, L"Resources/2d/icon/nav3.png");
	LoadTex(NAV4, L"Resources/2d/icon/nav4.png");
	LoadTex(EFFECT_SMOKE, L"Resources/2d/BossAttackEffect/BomParticle.png");
	LoadTex(EFFECT_POP, L"Resources/2d/mainEffect/pipo-hiteffect034.png");
	LoadTex(EFFECT_BOM, L"Resources/2d/BossAttackEffect/missileeffect.png");

	//Sprite
	LoadSprite(ENMEYHPFRAME1, L"Resources/2d/enemy/hpframe1.png");
	LoadSprite(ENMEYHPFRAME2, L"Resources/2d/enemy/hpframe2.png");
	LoadSprite(ENMEYHPFRAME3, L"Resources/2d/enemy/hpframe3.png");
	LoadSprite(ENMEYHPFRAME4, L"Resources/2d/enemy/hpframe4.png");
	LoadSprite(ENEMYNAME_GOLEM, L"Resources/2d/enemy/name_golem.png");
	LoadSprite(ENEMYNAME_MINIGOLEM, L"Resources/2d/enemy/name_puchigolem.png");
	LoadSprite(ENEMYNAME_LIZARD, L"Resources/2d/enemy/name_lizard.png");
	LoadSprite(ENEMYNAME_GUARDIAN, L"Resources/2d/enemy/name_guardian.png");
	LoadSprite(BOSSHPFRAME, L"Resources/09 パワーゲージ/gauge_frame2.png");
	LoadSprite(BOSSHPFRAMEINNER, L"Resources/09 パワーゲージ/gauge_base_or2.png");
	LoadSprite(BOSSHPFRAMEINNER2, L"Resources/09 パワーゲージ/gauge_base_bl.png");
	LoadSprite(GAMECLEAR_G, L"Resources/2d/ClearTex/g.png");
	LoadSprite(GAMECLEAR_A, L"Resources/2d/ClearTex/a.png");
	LoadSprite(GAMECLEAR_M, L"Resources/2d/ClearTex/m.png");
	LoadSprite(GAMECLEAR_E, L"Resources/2d/ClearTex/e.png");
	LoadSprite(GAMECLEAR_C, L"Resources/2d/ClearTex/c.png");
	LoadSprite(GAMECLEAR_L, L"Resources/2d/ClearTex/l.png");
	LoadSprite(GAMECLEAR_R, L"Resources/2d/ClearTex/r.png");
	LoadSprite(CLEARBACK, L"Resources/skydome/beautiful_tree-beautiful_natural_landscape_wallpaper_1366x768.jpg");
	LoadSprite(HELPICON, L"Resources/2d/attackEffect/searchPlwyer.png");
	LoadSprite(GETCHEST, L"Resources/2d/PlayTask/getchest.png");
	LoadSprite(NOGETCHEST, L"Resources/2d/PlayTask/nogetchest.png");
	LoadSprite(DEBUGFONT, L"Resources/2d/LevelUp/debugfont - コピー.png");
	LoadSprite(TASKFRAME, L"Resources/04 選択肢/button_select2_2.png");
	LoadSprite(TASK1, L"Resources/2d/PlayTask/Task1.png");
	LoadSprite(TASK2, L"Resources/2d/PlayTask/Task2.png");
	LoadSprite(TASK3, L"Resources/2d/PlayTask/Task3.png");
	LoadSprite(TASK4, L"Resources/2d/PlayTask/Task4.png");
	LoadSprite(OPENTASK, L"Resources/2d/PlayTask/taskmenuopen.png");
	LoadSprite(CLOSETASK, L"Resources/2d/PlayTask/taskmenuclose.png");
	LoadSprite(CHESTANNOUNCE, L"Resources/2d/PlayTask/getchest.png");
}

void ImageManager::LoadSprite(const SpriteName imageName, wchar_t* fileName)
{
	Sprite::LoadTexture(imageName, fileName);
}
