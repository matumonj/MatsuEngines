#pragma once
#include"Enemy.h"
#include<fstream>
#include<string>
#include<sstream>
#include"Collision.h"

class ControlBase
{
public:
	virtual ~ControlBase();
public:
	virtual void Load() =0;

	virtual void Initialize();
	virtual void Update();
	virtual void Draw();


	virtual void Update_Tutorial() = 0;
	virtual void Update_Play() = 0;
	virtual void Update_Boss() = 0;

	virtual void Init_Tutorial() = 0;
	virtual void Init_Play() = 0;
	virtual void Init_Boss() = 0;

	virtual void Draw_Tutorial() = 0;
	virtual void Draw_Play() = 0;
	virtual void Draw_Boss() = 0;


	virtual void Finalize() = 0;
private:
	static void (ControlBase::* initTable[])();
	static void (ControlBase::* updateTable[])();
	static void (ControlBase::* drawTable[])();
protected:
	template <class T>
	using ComPtr = ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = XMFLOAT2;
	using XMFLOAT3 = XMFLOAT3;
	using XMFLOAT4 = XMFLOAT4;
	using XMMATRIX = XMMATRIX;
	using XMVECTOR = XMVECTOR;

protected:
	//エディタの読み込みに必要な変数
	int Quantity;
	std::vector<XMFLOAT3> pos;
	std::vector<XMFLOAT3> rot;
	std::vector<XMFLOAT3> scl;
	std::string line;
	std::stringstream popcom;
	std::ifstream file;
	std::vector<int> Num;
	//オブジェクトの更新と描画がされる範囲
	float UpdateRange;
public:
	int GetQuentity() { return Quantity; }
};
