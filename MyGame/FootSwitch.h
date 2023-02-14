#pragma once
#include"Object3d.h"
#include<array>
#include<memory>

#include<fstream>
#include<string>
#include<sstream>
#include"ObjectManager.h"
class FootSwitch
{
private:
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;
public:
	FootSwitch(){};
	~FootSwitch(){};

	static FootSwitch* GetIns();
private:
	//スイッチの数
	static constexpr auto SwitchSize = 4;
	//スイッチモデル
	std::array<std::unique_ptr<Object3d>, SwitchSize>FootSwitchObj;
	//台座スイッチモデル
	std::array<std::unique_ptr<Object3d>, SwitchSize>FootSwitchFrameObj;
	//座標
	std::array<XMFLOAT3, SwitchSize>FootSwitchPos;
	//大きさ
	static constexpr XMFLOAT3 FootSwitchScl={3.f,3.f,3.f};
	static constexpr XMFLOAT3 FootSwitchFrameScl = { 5.f,2.5f,5.f };
	//色
	std::array<XMFLOAT4, SwitchSize>FootSwitchColor;
	//色変わる速度イージング用
	std::array<float, SwitchSize>FootSwitchEaseCount;

	//ふまれたか
	std::array<bool, SwitchSize>FootSwitchJudg;
	std::array<bool, SwitchSize>SwitchClearJudg;
	//今踏んだスイッチの数
	int ClearSwitchQuantity;
	//全て踏み終えたか
	bool AllClear;
public:
	//初期化
	void Init();
	//更新
	void Upda();
	//描画
	void Draw();


	//4つのスイッチの中心点を返す
	//宝箱の座標用
	XMFLOAT3 Switchs_CenterPos();
	//ふまれたスイッチの数
	const int GetClearSwicthsQuantity() { return ClearSwitchQuantity; }
private:
	//踏まれた判定
	bool FootJudg(XMFLOAT3 switchpos);
	//踏まれた時の色変え
	XMFLOAT4 SwitchChangeColor(XMFLOAT3 switchpos,bool& judg, bool& clearjudg, float& ColoEaseCount);

private:
	//CSV読み込み部分
	std::string line;
	std::stringstream popcom;
	std::ifstream file;
	std::vector<int> Num;
	void LoadCSV();


};

