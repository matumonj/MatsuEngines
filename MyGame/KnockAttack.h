#pragma once
#include"Texture.h"
#include"Input.h"
class KnockAttack
{
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;
private:
	Texture* KnockTex;
public:
	static KnockAttack* GetInstance();
	void Initialize();
	void Draw();
	void ActionJudg();
};

