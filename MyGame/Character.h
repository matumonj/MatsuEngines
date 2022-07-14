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
//すべてのオブジェクトの基底クラス
class Character
{
private: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

private:
	//生き死のフラグ
	bool AliveFlag = TRUE;
	//体力
	float HP;
public:
	//攻撃処理
	virtual void Attack();
	//脂肪処理(破棄処理など入れる)
	virtual void Death();
};

