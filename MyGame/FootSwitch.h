#pragma once
#include"Object3d.h"
#include<array>
#include<memory>

#include<fstream>
#include<string>
#include<sstream>
#include"ObjectManager.h"
#include "Particle.h"

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
	FootSwitch()
	{
	};

	~FootSwitch()
	{
	};

	static FootSwitch* GetIns();
private:
	static constexpr int CEffectSize = 3;

	struct SwitchParam
	{
		//スイッチモデル
		std::unique_ptr<Object3d> Obj = nullptr;
		//台座スイッチモデル
		std::unique_ptr<Object3d> FrameObj = nullptr;
		//座標
		XMFLOAT3 Pos = {};
		//色
		XMFLOAT4 Color = {};
		//色変わる速度イージング用
		float EaseCount = 0.f;
		//ふまれたか
		bool FootSwitchJudg = false;
		bool SwitchClearJudg = false;

		//チャージ用円形モデル
		std::array<std::unique_ptr<Object3d>, CEffectSize> CEffectObj = {nullptr};
		std::array<XMFLOAT3, CEffectSize> CEffectPos = {};
		std::array<float, CEffectSize> CEffectAlpha = {1.f};
		std::array<bool, CEffectSize> CeffectCreateF = {false};
	};

	//スイッチの数
	static constexpr auto SwitchSize = 4;
	//スイッチパラメータ
	std::array<SwitchParam, SwitchSize> switch_param_;
	//大きさ
	static constexpr XMFLOAT3 FootSwitchScl = {3.f, 3.f, 3.f};
	static constexpr XMFLOAT3 FootSwitchFrameScl = {5.f, 2.5f, 5.f};

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
	XMFLOAT4 SwitchChangeColor(XMFLOAT3 switchpos, bool& judg, bool& clearjudg, float& ColoEaseCount);

	void FootEffectUpda();
private:
	//CSV読み込み部分
	std::string line;
	std::stringstream popcom;
	std::ifstream file;
	std::vector<int> Num;
	void LoadCSV();
};
