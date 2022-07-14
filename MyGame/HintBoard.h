
#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <string>
#include"DirectXCommon.h"
#include"Object3d.h"
#include"Model.h"
#include"DebugCamera.h"
class HintBoard
{
protected:
	//Microosoft::WRL::Çè»ó™
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectX::Çè»ó™
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;
public:
	XMFLOAT3 position;
	XMFLOAT3 rotation;
	XMFLOAT3 scale;

private:
	Object3d* hintobj = nullptr;
	Model* hintmodel = nullptr;
public:
	void Initialize(DebugCamera*camera);
	void Update(DebugCamera* camera);
	void Draw();
	void Finalize();

};

