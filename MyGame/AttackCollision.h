#pragma once
#include"Object3d.h"
#include"Model.h"
#include"Input.h"
class AttackCollision
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::Çè»ó™
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;
private:
	Input* input;
	Object3d* cObj;
	Model* cModel;
	XMFLOAT3 Position;
public:
	void Init();

	void Update();

	void Draw();
};

