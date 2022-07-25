#pragma once
#include "ControlBase.h"
#include"AreaFence.h"
#include<fstream>
#include<string>
#include<sstream>
#include<ostream>
#include <iostream>
#include"Wood.h"
#include"Player.h"
#include"Enemy.h"
#include"DebugCamera.h"
#include<memory>
class FenceControl :
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
	std::string line;
	std::stringstream popcom;
	std::ifstream file;
	int Fence_Quantity;
	std::vector<std::unique_ptr<AreaFence>>fences;
	std::vector <XMFLOAT3>Load_FencePosition;
	std::vector<XMFLOAT3> fencepos;
	std::vector<int> Fence_Num;

	XMFLOAT3 Player_OldPos;

	bool TutorialFenceOpen;
public:
	static FenceControl* GetInstance();

	void LoadFences(DebugCamera* camera);

	void Initialize(DebugCamera* camera);

	void Update(DebugCamera* camera);

	void Draw();

	void ImGuiDraw();
};

