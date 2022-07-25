#pragma once
#include "ControlBase.h"
#include<fstream>
#include<string>
#include<sstream>
#include<ostream>
#include <iostream>
#include"Wood.h"
#include"Enemy.h"
#include"Player.h"
#include"DebugCamera.h"
#include<memory>
class WoodControl :
    public ControlBase
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
	int Wood_Quantity;
	std::vector<std::unique_ptr<Wood>>woods;
	std::vector <XMFLOAT3>Load_WoodPosition;
	std::vector<int> Wood_Num;
	std::vector<XMFLOAT3> woodpos;

	std::string line;
	std::stringstream popcom;
	std::ifstream file;


	XMFLOAT3 Player_OldPos;
public:
	static WoodControl* GetInstance();

	void LoadWood(DebugCamera* camera);

	void Initialize(DebugCamera* camera);

	void Update(DebugCamera* camera);

	void Draw();

	void ImGuiDraw();
};

