#pragma once
#include"Object3d.h"
#include<array>
#include<memory>
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
	FootSwitch();
	~FootSwitch();
private:
	//スイッチの数
	static constexpr auto SwitchSize = 4;
	//スイッチモデル
	std::array<std::unique_ptr<Object3d>, SwitchSize>FootSwitchObj;
	//座標
	std::array<XMFLOAT3, SwitchSize>FootSwitchPos;
	//色
	std::array<XMFLOAT4, SwitchSize>FootSwitchColor;
	//ふまれたか
	std::array<bool, SwitchSize>FootSwitchJudg;

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

private:
	//踏まれた判定
	bool FootJudg(XMFLOAT3 switchpos);
	//踏まれた時の色変え
	XMFLOAT4 SwitchChangeColor(XMFLOAT3 switchpos,bool& judg);
};

