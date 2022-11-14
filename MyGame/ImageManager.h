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
		//�v���C���[����
		PLAYERHP,
		PLAYERHPFRAME_INNER,
		PLAYERHPFRAME_BORDER,
		//�G�̗͎̑���
		ENMEYHP,
		BOSSENEMYHP,
		BOSSENEMYHPFRAME,
		//�`���[�g���A���������X�v���C�g
		STARTANNOUNCE,
		MOVEANNOUNCE,
		CONFIGANNOUNCE,
		ATTACKANNOUNCE,
		CHESTANNOUNCE,
		ENDANNOUNCE,
		//�l�[���v���[�g
		BOSSNAME,
		FIELDNAME,
		//�X�L���{�^��
		SKILL_1,
		SKILL_2,
		SKILL_3,
		SKILL_4,
		COOLDOWN,
		//�R���t�B�O����
		CONFIG_CUSBUTON,
		CONFIG_SELSWORD,
		CONFIG_HUDLAYOUT,
		CONFIG_SELBOX,
		CONFIG_MENU,
		//�󔠉��
		CHESTCOUNT,
		CHESTCOUNT_FRAME,
		//
		STITLE
	};

	enum tex2dName
	{
		ENMEYHP_MULTI,

		PARTICLE,
		SLASH,

		CELESFIELD,

		BOSSATK_CIRCLE,
		BOSSATK_HALF,
		BOSSATK_KNOCK
	};

public:
	static ImageManager* GetIns(); //�擾�p

	void Load2D();
	void LoadTex2D();
	void Finalize();
};
