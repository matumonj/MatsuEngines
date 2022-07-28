#pragma once
#include"mEffekseer.h"
#include"DirectXCommon.h"
#include"DebugCamera.h"
class Effects
{
private:
	//Microosoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private:
	//エフェクト用(ただプログラムでつくれるものはプログラムで作る方がいい　多用はいくない)
	mEffekseer* efk = nullptr;
	//mEffekseer* efk1 = nullptr;
	bool ff;
	bool sf;
	bool tf;
public:
	//XMFLOAT3 GetPosition
	XMFLOAT3 Effect_Pos = { -50,-10,90 };
	XMFLOAT3 Effect_Rot;
	XMFLOAT3 Effect_SCl;

public:
	static Effects* GetInstance();
	void Initialize(DebugCamera* camera);
	void Update(DebugCamera* camera);
	void Draw();
	void Finalize();

	XMFLOAT3 GetPos() { return Effect_Pos; }
};

