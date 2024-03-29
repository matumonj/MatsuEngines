#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include<forward_list>

/// <summary>
/// 3Dオブジェクト
/// </summary>
class ParticleManager
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // サブクラス
	struct VertexPos
	{
		XMFLOAT3 pos; // xyz座標
		float scale;
		XMFLOAT3 rot;
	};

	// 定数バッファ用データ構造体
	struct ConstBufferData
	{
		XMFLOAT4 color; // 色 (RGBA)
		XMMATRIX mat; // ３Ｄ変換行列]
		XMMATRIX matBillboard; //ビルボード行列
	};

	XMFLOAT4 color = {1, 1, 1, 1};

	struct Particle
	{
		using XMFLOAT3 = DirectX::XMFLOAT3;
		//座標
		XMFLOAT3 position = {};
		//速度
		XMFLOAT3 velocity = {};
		XMFLOAT3 velocity_old = {};
		//加速度
		XMFLOAT3 accel = {};
		//現在フレーム
		int frame = 0;
		//終了フレーム
		int num_frame = 10;
		float scale = 1.0f;
		float s_scale = 1.0f;
		float e_scale = 0.0f;
		XMFLOAT4 color;
		XMFLOAT4 e_color = {1, 0, 0, 1};
		XMFLOAT4 s_color = {1, 0, 0, 1};
		XMFLOAT3 rot = {0, 0, 0};
	};

private: // 定数

public: // 静的メンバ関数
	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="window_width">画面幅</param>
	/// <param name="window_height">画面高さ</param>
	/// <returns>成否</returns>
	static bool StaticInitialize(int window_width, int window_height);

	/// <summary>
	/// 描画前処理
	/// </summary>
	/// <param name="cmdList">描画コマンドリスト</param>
	static void PreDraw();

	/// <summary>
	/// 描画後処理
	/// </summary>
	static void PostDraw();

	/// <summary>
	/// 3Dオブジェクト生成
	/// </summary>
	/// <returns></returns>

private: // 静的メンバ変数
	// デバイス
	static ID3D12Device* device;
	// デスクリプタサイズ
	static UINT descriptorHandleIncrementSize;
	// コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootsignature;
	// パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelinestate;
	// デスクリプタヒープ
	static ComPtr<ID3D12DescriptorHeap> descHeap;
	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
	// インデックスバッファ
	//static ComPtr<ID3D12Resource> indexBuff;
	// テクスチャバッファ
	static ComPtr<ID3D12Resource> texbuff[100];
	// シェーダリソースビューのハンドル(CPU)
	static CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	// シェーダリソースビューのハンドル(CPU)
	static CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	// ビュー行列
	UINT texNumber = 0;
	static XMMATRIX matView;
	// 射影行列
	static XMMATRIX matProjection;
	// 視点座標
	static XMFLOAT3 eye;
	// 注視点座標
	static XMFLOAT3 target;
	// 上方向ベクトル
	static XMFLOAT3 up;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView;
	//ビルボード行列
	static XMMATRIX matBillboard;
	static XMMATRIX matBillboardY;
private: // 静的メンバ関数
	/// <summary>
	/// デスクリプタヒープの初期化
	/// </summary>
	/// <returns></returns>
	static bool InitializeDescriptorHeap();

	/// <summary>
	/// カメラ初期化
	/// </summary>
	/// <param name="window_width">画面横幅</param>
	/// <param name="window_height">画面縦幅</param>
	static void InitializeCamera(int window_width, int window_height);

	/// <summary>
	/// グラフィックパイプライン生成
	/// </summary>
	/// <returns>成否</returns>
	static bool InitializeGraphicsPipeline();

	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	/// <returns>成否</returns>

	/// <summary>
	/// モデル作成
	/// </summary>

	/// <summary>
	/// ビュー行列を更新
	/// </summary>
	static void UpdateViewMatrix();

public:
	static bool LoadTexture(UINT texnum, const wchar_t* filename);
	void CreateModel();
	void SetColor(XMFLOAT4 color) { this->color = color; }
private: // メンバ変数
	ComPtr<ID3D12Resource> constBuff; // 定数バッファ
	// 色
	//XMFLOAT4 colors;// = colors;
	// ローカルスケール
	XMFLOAT3 scale = {1, 1, 1};

	//パーティクル配列
	std::forward_list<Particle> particles;

public:
	enum ParticleType
	{
		NORMAL,
		ABSORPTION,
		FOLLOW,
		CHARGE
	};

	//普通の拡散エフェクト
	void Normal();
	//一定距離行ったらパーティクルが元の位置にもどってくる
	void Absorption();
	//一定距離行ったらposition方向に戻ってくる
	void Follow(XMFLOAT3 position, int lifejudg);
	//チャージもどき
	void Charge(XMFLOAT3 position);
public: // メンバ関数
	bool Initialize(UINT texnum);
	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update(ParticleType type, XMFLOAT3 position = {0, 0, 0}, int lifejudg = false);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();
	//パーティクルの追加
	void Add(int life, XMFLOAT3 position, XMFLOAT3 velocity, XMFLOAT3 accel, float start_scale, float end_scale);
	static ParticleManager* Create(UINT texnum, const wchar_t* filename);
};
