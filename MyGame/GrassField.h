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
	std::unique_ptr<GrassObj> grass;
	XMFLOAT3 FieldPos;
	XMFLOAT3 FieldRot;
	XMFLOAT3 FieldScl;
public:
	void Initialize();
	void Update();
	void Draw();

public:
	XMFLOAT3 GetRotation() { return FieldRot; }
	XMFLOAT3 GetPosition() { return FieldPos; }
	XMFLOAT3 GetScale() { return FieldScl; }

	void SetRotation(XMFLOAT3 rot) { FieldRot = rot; }
	void SetPosition(XMFLOAT3 pos) { FieldPos = pos; }
	void SetScale(XMFLOAT3 scl) { FieldScl = scl; }
};
