#pragma once
#include"GrassObj.h"
#include"Model.h"
#include<memory>
#include"DebugCamera.h"
class GrassField
{
public:
	GrassField();
	~GrassField();
private:
	// DirectX::Çè»ó™
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

private:
	std::unique_ptr<GrassObj>grass;
	XMFLOAT3 FieldPos;
	XMFLOAT3 FieldRot;
	XMFLOAT3 FieldScl;
public:
	void Initialize(DebugCamera* camera);
	void Update(DebugCamera* camera);
	void Draw();
};

