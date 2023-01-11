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
	/// �_�Ɛ����̋��������߂�
	/// </summary>
	/// <param name="point">�_</param>
	/// <param name="line">����</param>
	/// <param name="dist">�����i�o�͗p�j</param>
	static float CollsionPoint2Line2D(const Point& point, const Line2D& lineseg);
	/// <summary>
	/// �_�ƎO�p�`�̍ŋߐړ_�����߂�
	/// </summary>
	/// <param name="point">�_</param>
	/// <param name="triangle">�O�p�`</param>
	/// <param name="closest">�ŋߐړ_�i�o�͗p�j</param>
	static void ClosestPtPoint2Triangle(const XMVECTOR& point, const Triangle& triangle, XMVECTOR* closest);

	/// <summary>
	/// ���Ƌ��̓����蔻��
	/// </summary>
	/// <param name="sphereA">��A</param>
	/// <param name="sphereB">��B</param>
	/// <param name="inter">��_�i�o�͗p�j</param>
	/// <param name="reject">�r�˃x�N�g���i�o�͗p�j</param>
	/// <returns></returns>
	static bool CheckSphere2Sphere(const Sphere& sphereA, const Sphere& sphereB, XMVECTOR* inter = nullptr,
	                               XMVECTOR* reject = nullptr);

	//static bool ChenkSphere2Sphere(XMFLOAT3 object1, XMFLOAT3& object2, float r);
	static bool CheckBox2Box(const Box& box1, const Box& box2);

	static bool CheckPoint2Rect(const Point& point, const Box& rect);
	/// <summary>
	/// ���C�ƕ��ʂ̓����蔻��
	/// </summary>
	/// <param name="lay">���C</param>
	/// <param name="plane">����</param>
	/// <param name="inter">�����i�o�͗p�j</param>
	/// <param name="inter">��_�i�o�͗p�j</param>
	/// <returns>�������Ă��邩�ۂ�</returns>
	static bool CheckRay2Plane(const Ray& lay, const Plane& plane, float* distance = nullptr,
	                           XMVECTOR* inter = nullptr);

	/// <summary>
	/// ���Ɩ@���t���O�p�`�̓����蔻��
	/// </summary>
	/// <param name="sphere">��</param>
	/// <param name="triangle">�O�p�`</param>
	/// <param name="inter">��_�i�O�p�`��̍ŋߐړ_�j</param>
	/// <param name="reject">�r�˃x�N�g���i�o�͗p�j</param>
	/// <returns>�������Ă��邩�ۂ�</returns>
	static bool CheckSphere2Triangle(const Sphere& sphere, const Triangle& triangle, XMVECTOR* inter = nullptr,
	                                 XMVECTOR* reject = nullptr);

	/// <summary>
	/// ���C�Ɩ@���t���O�p�`�̓����蔻��
	/// </summary>
	/// <param name="lay">���C</param>
	/// <param name="triangle">�O�p�`</param>
	/// <param name="inter">�����i�o�͗p�j</param>
	/// <param name="inter">��_�i�o�͗p�j</param>
	/// <returns>�������Ă��邩�ۂ�</returns>
	static bool CheckRay2Triangle(const Ray& lay, const Triangle& triangle, float* distance = nullptr,
	                              XMVECTOR* inter = nullptr);

	/// <summary>
	/// ���C�Ƌ��̓����蔻��
	/// </summary>
	/// <param name="lay">���C</param>
	/// <param name="sphere">��</param>
	/// <param name="sphere">�����i�o�͗p�j</param>
	/// <param name="inter">��_�i�o�͗p�j</param>
	/// <returns>�������Ă��邩�ۂ�</returns>
	static bool CheckRay2Sphere(const Ray& lay, const Sphere& sphere, float* distance = nullptr,
	                            XMVECTOR* inter = nullptr);

public:
	static bool CheckOBBCollision(OBB& obb1, OBB& obb2);
private:
	static double LenSegOnSeparateAxis(XMVECTOR* Sep, XMVECTOR* e1, XMVECTOR* e2, XMVECTOR* e3 = nullptr);
};
