#pragma once
#include"CollisionPrimitive.h"
#include<DirectXMath.h>
using namespace DirectX;
class Collision
{
public:

	static bool Collision::CheckSphere2Box(const Sphere& sphere, const Box& box);

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

	static void SetCollideOBB(bool f);
	static bool GetCollideOBB();
	/// <summary>
	/// �_�ƎO�p�`�̍ŋߐړ_�����߂�
	/// </summary>
	/// <param name="point">�_</param>
	/// <param name="triangle">�O�p�`</param>
	/// <param name="closest">�ŋߐړ_�i�o�͗p�j</param>
	static void ClosestPtPoint2Triangle(const DirectX::XMVECTOR& point, const Triangle& triangle, DirectX::XMVECTOR* closest);

	/// <summary>
	/// ���Ƌ��̓����蔻��
	/// </summary>
	/// <param name="sphereA">��A</param>
	/// <param name="sphereB">��B</param>
	/// <param name="inter">��_�i�o�͗p�j</param>
	/// <param name="reject">�r�˃x�N�g���i�o�͗p�j</param>
	/// <returns></returns>
	static bool CheckSphere2Sphere(const Sphere& sphereA, const Sphere& sphereB, DirectX::XMVECTOR* inter = nullptr, DirectX::XMVECTOR* reject = nullptr);

	//static bool ChenkSphere2Sphere(XMFLOAT3 object1, XMFLOAT3& object2, float r);
	static bool CheckBox2Box(const Box& box1, const Box& box2);
	
	static bool CheckPoint2Rect(const Point&point,const Box&rect);
	/// <summary>
	/// ���C�ƕ��ʂ̓����蔻��
	/// </summary>
	/// <param name="lay">���C</param>
	/// <param name="plane">����</param>
	/// <param name="inter">�����i�o�͗p�j</param>
	/// <param name="inter">��_�i�o�͗p�j</param>
	/// <returns>�������Ă��邩�ۂ�</returns>
	static bool CheckRay2Plane(const Ray& lay, const Plane& plane, float* distance = nullptr, DirectX::XMVECTOR* inter = nullptr);

	/// <summary>
	/// ���Ɩ@���t���O�p�`�̓����蔻��
	/// </summary>
	/// <param name="sphere">��</param>
	/// <param name="triangle">�O�p�`</param>
	/// <param name="inter">��_�i�O�p�`��̍ŋߐړ_�j</param>
	/// <param name="reject">�r�˃x�N�g���i�o�͗p�j</param>
	/// <returns>�������Ă��邩�ۂ�</returns>
	static bool CheckSphere2Triangle(const Sphere& sphere, const Triangle& triangle, DirectX::XMVECTOR* inter = nullptr, DirectX::XMVECTOR* reject = nullptr);

	/// <summary>
	/// ���C�Ɩ@���t���O�p�`�̓����蔻��
	/// </summary>
	/// <param name="lay">���C</param>
	/// <param name="triangle">�O�p�`</param>
	/// <param name="inter">�����i�o�͗p�j</param>
	/// <param name="inter">��_�i�o�͗p�j</param>
	/// <returns>�������Ă��邩�ۂ�</returns>
	static bool CheckRay2Triangle(const Ray& lay, const Triangle& triangle, float* distance = nullptr, DirectX::XMVECTOR* inter = nullptr);

	/// <summary>
	/// ���C�Ƌ��̓����蔻��
	/// </summary>
	/// <param name="lay">���C</param>
	/// <param name="sphere">��</param>
	/// <param name="sphere">�����i�o�͗p�j</param>
	/// <param name="inter">��_�i�o�͗p�j</param>
	/// <returns>�������Ă��邩�ۂ�</returns>
	static bool CheckRay2Sphere(const Ray& lay, const Sphere& sphere, float* distance = nullptr, DirectX::XMVECTOR* inter = nullptr);

};

