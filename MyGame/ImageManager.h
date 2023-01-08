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
	enum texName
	{
		//プレイヤー周り
		PLAYERHP,
		PLAYERHPFRAME_INNER,
		PLAYERHPFRAME_BORDER,
		//敵の体力周り

		BOSSENEMYHP1,

		BOSSENEMYHPFRAME,
		//チュートリアル時説明スプライト
		STARTANNOUNCE,
		MOVEANNOUNCE,
		CONFIGANNOUNCE,
		ATTACKANNOUNCE,
		CHESTANNOUNCE,
		ENDANNOUNCE,
		//ネームプレート
		BOSSNAME,
		FIELDNAME,
		//スキルボタン
		GAMECLEAR_G,
		GAMECLEAR_A,
		GAMECLEAR_M,
		GAMECLEAR_E ,
		GAMECLEAR_C,
		GAMECLEAR_L,
		GAMECLEAR_R,
		//コンフィグ周り
		CONFIG_CUSBUTON,
		CONFIG_SELSWORD,
		CONFIG_HUDLAYOUT,
		CONFIG_SELBOX,
		CONFIG_MENU,
		//宝箱回収
		CHESTCOUNT,
		CHESTCOUNT_FRAME,
		//
		STITLE,

		
	};

	enum tex2dName
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
	};

public:
	static ImageManager* GetIns(); //取得用

	void Init();
	void Load2D();
	void LoadTex2D(tex2dName imageName, wchar_t* fileName);
	void Finalize();

	UINT GetImage(UINT image) { return image; }
private:
	static std::map<tex2dName, Sprite*> tex; //モデル格納マップ
};
