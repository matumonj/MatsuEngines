#pragma once
#include <map>
#include <string>
#include "Sprite.h"
#include "Texture.h"

class ImageManager final
{
private:
	ImageManager() = default;
	~ImageManager() = default;
	ImageManager(const ImageManager& r) = default;
	ImageManager& operator=(const ImageManager& r) = default;

public:
	enum TexName
	{
		//チュートリアル時説明スプライト
		STARTANNOUNCE,
		MOVEANNOUNCE,
		CONFIGANNOUNCE,
		ATTACKANNOUNCE,
		CHESTA,
		ENDANNOUNCE,
		//ネームプレート
		BOSSNAME,
		FIELDNAME,
		//スキルボタン
		SKILL_1,
		SKILL_2,
		SKILL_3,
		SKILL_4,
		COOLDOWN,

		//宝箱回収
		CHESTCOUNT,
		CHESTCOUNT_FRAME,
		//
		STITLE,
		NAV1,
		NAV2,
		NAV3,
		NAV4,
		EFFECT_SMOKE=64,
		EFFECT_BOM=66,
		EFFECT_POP=65
	};

	enum SpriteName
	{
		ENMEYHP_MULTI,
		ENMEYHPFRAME1=44,
		ENMEYHPFRAME2=45,
		ENMEYHPFRAME3=46,
		ENMEYHPFRAME4 = 47,
		PARTICLE,
		SLASH,

		CELESFIELD,

		BOSSATK_CIRCLE,
		BOSSATK_HALF,
		BOSSATK_KNOCK,
		ENEMYNAME_GOLEM,
		ENEMYNAME_MINIGOLEM,
		ENEMYNAME_LIZARD,
		ENEMYNAME_GUARDIAN,

		BOSSHPFRAME,
		BOSSHPFRAMEINNER,
		BOSSHPFRAMEINNER2,

		TITLE1,
		TITLE2,
		GAMEPLAY,

		GAMECLEAR_G = 27,
		GAMECLEAR_A = 28,
		GAMECLEAR_M = 29,
		GAMECLEAR_E = 30,
		GAMECLEAR_C = 31,
		GAMECLEAR_L = 32,
		GAMECLEAR_R = 33,

		HELPICON=34,
		CHESTANNOUNCE = 80,
		CLEARBACK,
		GETCHEST,
		NOGETCHEST,
		DEBUGFONT,
		TASKFRAME,
		TASK1,
		TASK2,
		TASK3,
		TASK4,
		OPENTASK,
		CLOSETASK,

		/*
	Texture::LoadTexture(18, L"Resources/2d/icon/nav1.png");
	Texture::LoadTexture(19, L"Resources/2d/icon/nav2.png");
	Texture::LoadTexture(20, L"Resources/2d/icon/nav3.png");
	Texture::LoadTexture(21, L"Resources/2d/icon/nav4.png");*/
	};

public:
	static ImageManager* GetIns(); //取得用

	void Init();
	void Load2D();
	void LoadSprite(SpriteName imageName, wchar_t* fileName);
	void LoadTex(TexName imageName, wchar_t* fileName);
	void Finalize();

	UINT GetImage(UINT image) { return image; }
private:
	static std::map<SpriteName, Sprite*> sprites; //モデル格納マップ

	static std::map<TexName, Texture*> texs; //モデル格納マップ
};
