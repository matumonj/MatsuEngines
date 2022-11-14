#pragma once
#include<memory>
#include"Sprite.h"

class EncountGuardianSprite
{
private:
	template <class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::Çè»ó™
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private:
	enum Site
	{
		BOTTOM,
		UP,
		FORM
	};

	std::unique_ptr<Sprite> AppearranceSprite[3];
public:
	static EncountGuardianSprite* GetInstance();
	void Init();
	void Update();
	void Draw();
	void Finalize();

private:
	float SpriteAlpha = 0.0f;
};
