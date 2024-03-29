#pragma once

#include "f_Model.h"
#include "Camera.h"

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <string>
#include"LightGroup.h"
class BaseCollider;

class f_Object3d
{
protected: //エイリアス
	//Microosoft::WRL::を省略
	template <class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;
public: //静的メンバ関数
	//setter
	static void SetDevice(ID3D12Device* device) { f_Object3d::device = device; }
	static void SetCamera(Camera* camera) { f_Object3d::camera = camera; }


	/// <summary>
	///グラフィックパイプラインの生成
	/// </summary>
	static void CreateGraphicsPipeline();

private: //静的メンバ変数
	//デバイス
	static ComPtr<ID3D12Device> device;
	static ComPtr<ID3D12GraphicsCommandList> cmdList;
	//カメラ
	static Camera* camera;
	//ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootsignature;
	//パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelinestate;

public: //サブクラス
	//定数バッファ用データ構造体（座標変換行列）
	struct ConstBufferDataTransform
	{
		XMFLOAT4 color;
		XMMATRIX viewproj; //ビュープロジェクション行列
		XMMATRIX world; //ワールド行列
		XMFLOAT3 cameraPos; //カメラ行列（ワールド座標）
	};

	XMFLOAT3 ambient;
	XMFLOAT3 diffuse;
	XMFLOAT3 specular;
	float alpha;
	// 定数バッファ
	ComPtr<ID3D12Resource> constBuff;

	struct ConstBufferDataB1
	{
		XMFLOAT3 ambient;
		float pad1;
		XMFLOAT3 diffuse;
		float pad2;
		XMFLOAT3 specular;
		float alpha;
	};

public: //メンバ関数
	void SetCollider(BaseCollider* collider);

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム
	/// </summary>
	void Updata(bool animeloop);
	void Updata();
	/// </summary>
	void Update(bool Loop, double Speed, bool& Stop);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="cmdList"></param>
	void Draw();

	/// <summary>
	/// モデルのセット
	/// </summary>
	/// <param name="model">モデル</param>
	void SetModel(f_Model* model) { this->model = model; }

	/// <summary>
	/// アニメーション
	/// </summary>
	void PlayAnimation(int number);
	void PlayAnimation();
protected: //メンバ変数
	//定数バッファ
	ComPtr<ID3D12Resource> constBuffTransform;
	//ローカルスケール
	XMFLOAT3 scale = {0.1f, 0.1f, 0.1f};
	//X,Y,Z軸回りのローカル回転角
	XMFLOAT3 rotation = {0.0f, 0.0f, 0.0f};
	//ローカル座標
	XMFLOAT3 position = {0.0f, 0.0f, 0.0f};
	//ローカルワールド変換行列
	XMMATRIX matWorld;
	//モデル
	f_Model* model = nullptr;
	//定数バッファ（スキン）
	ComPtr<ID3D12Resource> constBuffSkin;
	//1フレーム
	FbxTime frameTime;
	//アニメーション開始時間
	FbxTime startTime;
	//アニメーション終了時間
	FbxTime endTime;
	//現在時間（アニメーション）
	FbxTime currentTime;
	//アニメーション再生中
	bool isPlay = false;
	XMMATRIX matScale, matRot, matTrans;
	XMFLOAT4 color;
	float f_time;
	int fbxTime = 0;
	float start_time;
	double end_time;

	bool AttackFlag;
	float NormalAttackTime;
	bool DeathFlag;
	float DeathTime;

public: //定数
	void SetColor(XMFLOAT4 color) { this->color = color; }
	void SetPosition(XMFLOAT3 pos) { position = pos; }
	void SetRotation(XMFLOAT3 rot) { rotation = rot; }
	void SetScale(XMFLOAT3 scl) { scale = scl; }

	XMMATRIX GetMatRot() { return matRot; }

	XMMATRIX GetMatTrans() { return matTrans; }

	XMMATRIX GetMatScale() { return matScale; }
	//ボーンの最大数
	static const int MAX_BONES = 88;

	XMFLOAT3 GetPosition() { return position; }

	//定数バッファ用データ構造体（スキニング）
	struct ConstBufferDataSkin
	{
		XMMATRIX bones[MAX_BONES];
	};

	bool stopFlag;
	void SeteCurrent(bool f) { stopFlag = f; }
	XMMATRIX HandMatWorld;
	XMMATRIX Posmat;
	XMFLOAT3 pos;
	XMMATRIX GetPos() { return Posmat; }
	XMMATRIX GetMatrot() { return matRot; }
	XMMATRIX GetHandBoneMatWorld() { return HandMatWorld; }
	FbxTime SetCurrent() { return currentTime = startTime; }
	// ライト
	//static LightGroup* lightGroup;
	static LightGroup* lightGroups;

	static void SetLightGroup(LightGroup* lightGroup)
	{
		lightGroups = lightGroup;
	}

	int bindexs = 13;
	void SetFogPos(XMFLOAT3 pos) { fogpos = pos; }
	void SetHandBoneIndex(int bindex) { bindexs = bindex; }
	void GetBoneIndexMat(int index, XMMATRIX& matworld);

	double GetFTime() { return currentTime.GetSecondDouble(); }
private:
	XMFLOAT3 fogpos;
	bool nowAttack;
	bool nowDeath;

	// アニメーション用データ構造体
	struct Animation
	{
		FbxAnimStack* stack;
		FbxTakeInfo* info;
	};

	//アニメーションのデータ
	std::vector<Animation> animationData;
	bool isendtime;

	double AnimationSpeed;
public:
	void LoadAnimation();
	void SetFbxTime(float time) { f_time = time; }
	void SetFTime(int time) { fbxTime = time; }
	void SetAttackFlag(bool flag) { AttackFlag = flag; }
	void SetAttackTime(float time) { NormalAttackTime = time; }
	void SetDeathFlag(bool flag) { DeathFlag = flag; }
	void SetDeathTime(float time) { DeathTime = time; }
	double GetEndTime() { return endTime.GetSecondDouble(); }
	double GetAnimeTime() { return currentTime.GetSecondDouble(); }
	XMMATRIX GetWorld() { return matWorld; }

	XMMATRIX ExtractPositionMat(XMMATRIX matworld);
	XMMATRIX ExtractRotationMat(XMMATRIX matworld);
	XMMATRIX ExtractScaleMat(XMMATRIX matworld);
};
