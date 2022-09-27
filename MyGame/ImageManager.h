#pragma once
#include <map>
#include <string>
#include "Sprite.h"
#include "Texture.h"
class ImageManager  final {
private:
	ImageManager() = default;
	~ImageManager() = default;
	ImageManager(const ImageManager& r) = default;
	ImageManager& operator= (const ImageManager& r) = default;

public:

	enum texName {
		//プレイヤー周り
		PLAYERHP,
		PLAYERHPFRAME_INNER,
		PLAYERHPFRAME_BORDER,
		//敵の体力周り
		ENMEYHP,
		BOSSENEMYHP,
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
		SKILL_1,
		SKILL_2,
		SKILL_3,
		SKILL_4,
		COOLDOWN,
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
		STITLE
	};
	enum tex2dName {
		ENMEYHP_MULTI,
		
		PARTICLE,
		SLASH,
		
		CELESFIELD,

		BOSSATK_CIRCLE,
		BOSSATK_HALF,
		BOSSATK_KNOCK
	};
public:
	static ImageManager* GetIns();		//取得用

	void Load2D();
	void LoadTex2D();
	void Finalize();
};

