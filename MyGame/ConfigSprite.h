#pragma once
#include<memory>
#include"Sprite.h"
class ConfigSprite
{
public:
	~ConfigSprite();
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

private:
	//�R���t�B�O���ɗp�ӂ���X�v���C�g�̖���
	static const int MaxSprite = 3;
	//�e�X�v���C�g�Ԃ̊Ԋu
	const float Space = 200.0f;
	//HUD���C�A�E�g
	Sprite*RayOutSprite;
	//�G�̗͕̑\��
	Sprite*EnemyHPSelectSprite;
	//��
	Sprite* SwordSelectSprite;
	Sprite* MenuSprite;
	XMFLOAT2 SpritePosition[MaxSprite] = { {300,200},{300,350},{300,500} };
public:
	static ConfigSprite* GetInstance();

	XMFLOAT2 GetSpritePosition(int number) { return SpritePosition[number]; }
	void Initialize();

	void Update();

	void Draw();
};

