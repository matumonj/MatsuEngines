#pragma once
/// <summary>
/// 当たり判定プリミティブ
/// </summary>

#include <DirectXMath.h>

/// <summary>
/// 球
/// </summary>
struct Sphere
{
	// 中心座標
	DirectX::XMVECTOR center = {};
	// 半径
	float radius = 1.0f;
};

/// <summary>
/// 平面
/// </summary>
struct Plane
{
	// 法線ベクトル
	DirectX::XMVECTOR normal = {0, 1, 0};
	// 原点(0,0,0)からの距離
	float distance = 0.0f;
};

/// <summary>
/// 法線付き三角形（時計回りが表面）
/// </summary>
class Triangle
{
public:
	// 頂点座標3つ
	DirectX::XMVECTOR p0;
	DirectX::XMVECTOR p1;
	DirectX::XMVECTOR p2;
	// 法線ベクトル
	DirectX::XMVECTOR normal;

	/// <summary>
	/// 法線の計算
	/// </summary>
	void ComputeNormal();
};

/// <summary>
/// レイ（半直線）
/// </summary>
struct Ray
{
	// 始点座標
	DirectX::XMVECTOR start = {0, 0, 0, 1};
	// 方向
	DirectX::XMVECTOR dir = {1, 0, 0, 0};
};

struct AABB
{
};

struct Box
{
	DirectX::XMFLOAT2 position;
	DirectX::XMFLOAT2 scale;
};

struct Point
{
	float x;
	float y;
};

struct Line2D
{
	DirectX::XMFLOAT2 start;
	DirectX::XMFLOAT2 end;
};

class OBB
{
private:
	DirectX::XMVECTOR m_Pos; // 位置
	DirectX::XMVECTOR m_NormaDirect[3]; // 方向ベクトル 回転ベクトル打ち込む
	float m_fLength[3]; // 各軸方向の長さ
public:
	DirectX::XMVECTOR GetDirect(int elem) { return m_NormaDirect[elem]; } // 指定軸番号の方向ベクトルを取得
	float GetLen_W(int elem) { return m_fLength[elem]; } // 指定軸方向の長さを取得
	DirectX::XMVECTOR GetPos_W() { return m_Pos; }

	void SetOBBParam_Pos(DirectX::XMMATRIX posMat);
	void SetOBBParam_Pos(DirectX::XMFLOAT3 pos);
	void SetOBBParam_Scl(DirectX::XMFLOAT3 scl);
	void SetOBBParam_Rot(DirectX::XMMATRIX rotMat);
};
