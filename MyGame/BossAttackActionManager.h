#pragma once
#include <DirectXMath.h>

class BossAttackActionManager
{
protected:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

private:
public:
	virtual void Init() =0;
	virtual void Upda() =0;
	virtual void Draw() =0;
};
