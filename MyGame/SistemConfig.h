#pragma once
#include"Input.h"
#include"Sprite.h"
#include<memory>
#include"ConfigSprite.h"
class SistemConfig
{
public:
	~SistemConfig();
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

private:
	Sprite*SelectSprite;

	//ConfigSprite�ɗp�ӂ��Ă�X�v���C�g�̐�
	static const int MaxSprite = 2;

	Input* input;
	ConfigSprite* configSprite;

	//�R���t�B�O��ʂ�
	bool m_ConfigFlag;
	//�X�v���C�g���W�w��p
	int m_number = 0;
	//��ʂ������؂�ւ��Ȃ��悤�ɂ��邽�߂̃J�E���^
	int count = 0;
	
public:
	static SistemConfig* GetInstance();

	enum Config
	{
		HUDRAYOUT,
		CUSTOMBUTTON,
		ENEMYHP,
		SWORDSELECT,
		END
	};
	Config GetNowSelect() { return config; }
	Config config;
	float sAlpha = 0;
	//������
	void Initialize();
	//�X�V����
	void Update();
	//�`��
	void Draw();

private:
	Config NowSelectButton();
	bool EndConfigJudg;
public:
	bool GetEndConfig() { return EndConfigJudg; }
	bool GetConfigJudgMent() { return m_ConfigFlag; }
};

