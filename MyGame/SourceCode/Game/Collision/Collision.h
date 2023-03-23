#pragma once
#include"CollisionPrimitive.h"
#include<DirectXMath.h>
using namespace DirectX;

class Collision
{
public:
	static float GetLength(XMFLOAT3 position, XMFLOAT3 position2);

	struct BoxVertex
	{
		float Up;
		float Down;
		float Left;
		float Right;
	};

	static bool ColFlag;
public:
	static bool line2dCol(Line2D line, Point point, float dis);
	static void SetCollideOBB(bool f);
	static bool GetCollideOBB();
	static float CalculationVertexLength(const XMFLOAT2& pos01, const XMFLOAT2& pos02);
	static void ConvertToNomalizeVector(XMFLOAT2& out, XMFLOAT2 in);
	static float CalculationVectorLength(const XMFLOAT2& vec01);
	static bool IsCollidingLineAndCircle(Line2D line, Point circle, float dis);
	/// <summary>
	/// 点と線分の距離を求める
	/// </summary>
	/// <param name="point">点</param>
	/// <param name="line">線分</param>
	/// <param name="dist">距離（出力用）</param>
	static float CollsionPoint2Line2D(const Point& point, const Line2D& lineseg);
	/// <summary>
	/// 点と三角形の最近接点を求める
	/// </summary>
	/// <param name="point">点</param>
	/// <param name="triangle">三角形</param>
	/// <param name="closest">最近接点（出力用）</param>
	static void ClosestPtPoint2Triangle(const XMVECTOR& point, const Triangle& triangle, XMVECTOR* closest);

	/// <summary>
	/// 球と球の当たり判定
	/// </summary>
	/// <param name="sphereA">球A</param>
	/// <param name="sphereB">球B</param>
	/// <param name="inter">交点（出力用）</param>
	/// <param name="reject">排斥ベクトル（出力用）</param>
	/// <returns></returns>
	static bool CheckSphere2Sphere(const Sphere& sphereA, const Sphere& sphereB, XMVECTOR* inter = nullptr,
	                               XMVECTOR* reject = nullptr);

	//static bool ChenkSphere2Sphere(XMFLOAT3 object1, XMFLOAT3& object2, float r);
	static bool CheckBox2Box(const Box& box1, const Box& box2);

	static bool CheckPoint2Rect(const Point& point, const Box& rect);
	/// <summary>
	/// レイと平面の当たり判定
	/// </summary>
	/// <param name="lay">レイ</param>
	/// <param name="plane">平面</param>
	/// <param name="inter">距離（出力用）</param>
	/// <param name="inter">交点（出力用）</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckRay2Plane(const Ray& lay, const Plane& plane, float* distance = nullptr,
	                           XMVECTOR* inter = nullptr);

	/// <summary>
	/// 球と法線付き三角形の当たり判定
	/// </summary>
	/// <param name="sphere">球</param>
	/// <param name="triangle">三角形</param>
	/// <param name="inter">交点（三角形上の最近接点）</param>
	/// <param name="reject">排斥ベクトル（出力用）</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckSphere2Triangle(const Sphere& sphere, const Triangle& triangle, XMVECTOR* inter = nullptr,
	                                 XMVECTOR* reject = nullptr);

	/// <summary>
	/// レイと法線付き三角形の当たり判定
	/// </summary>
	/// <param name="lay">レイ</param>
	/// <param name="triangle">三角形</param>
	/// <param name="inter">距離（出力用）</param>
	/// <param name="inter">交点（出力用）</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckRay2Triangle(const Ray& lay, const Triangle& triangle, float* distance = nullptr,
	                              XMVECTOR* inter = nullptr);

	/// <summary>
	/// レイと球の当たり判定
	/// </summary>
	/// <param name="lay">レイ</param>
	/// <param name="sphere">球</param>
	/// <param name="sphere">距離（出力用）</param>
	/// <param name="inter">交点（出力用）</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckRay2Sphere(const Ray& lay, const Sphere& sphere, float* distance = nullptr,
	                            XMVECTOR* inter = nullptr);

public:
	static bool CheckOBBCollision(OBB& obb1, OBB& obb2);
private:
	static double LenSegOnSeparateAxis(XMVECTOR* Sep, XMVECTOR* e1, XMVECTOR* e2, XMVECTOR* e3 = nullptr);
};
