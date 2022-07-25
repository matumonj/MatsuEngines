#pragma once
#include"Enemy.h"
#include<fstream>
#include<string>
#include<sstream>
#include<ostream>
#include <iostream>

#include"DebugCamera.h"
class ControlBase
{
public:
	virtual void Load(DebugCamera* camera)=0;

	virtual void Initialize(DebugCamera* camera)=0;

	virtual void Update(DebugCamera* camera)=0;

	virtual void Draw()=0;

protected:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

protected:
	//エディタの読み込みに必要な変数
	int Quantity;
	std::vector<XMFLOAT3> pos;
	std::string line;
	std::stringstream popcom;
	std::ifstream file;
	std::vector<int> Num;

public:
	int GetQuentity() { return Quantity; }


};

