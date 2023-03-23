#pragma once
/// <summary>
/// �����蔻��v���~�e�B�u
/// </summary>

#include <DirectXMath.h>

/// <summary>
/// ��
/// </summary>
struct Sphere
{
	// ���S���W
	DirectX::XMVECTOR center = {};
	// ���a
	float radius = 1.0f;
};

/// <summary>
/// ����
/// </summary>
struct Plane
{
	// �@���x�N�g��
	DirectX::XMVECTOR normal = {0, 1, 0};
	// ���_(0,0,0)����̋���
	float distance = 0.0f;
};

/// <summary>
/// �@���t���O�p�`�i���v��肪�\�ʁj
/// </summary>
class Triangle
{
public:
	// ���_���W3��
	DirectX::XMVECTOR p0;
	DirectX::XMVECTOR p1;
	DirectX::XMVECTOR p2;
	// �@���x�N�g��
	DirectX::XMVECTOR normal;

	/// <summary>
	/// �@���̌v�Z
	/// </summary>
	void ComputeNormal();
};

/// <summary>
/// ���C�i�������j
/// </summary>
struct Ray
{
	// �n�_���W
	DirectX::XMVECTOR start = {0, 0, 0, 1};
	// ����
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
	DirectX::XMVECTOR m_Pos; // �ʒu
	DirectX::XMVECTOR m_NormaDirect[3]; // �����x�N�g�� ��]�x�N�g���ł�����
	float m_fLength[3]; // �e�������̒���
public:
	DirectX::XMVECTOR GetDirect(int elem) { return m_NormaDirect[elem]; } // �w�莲�ԍ��̕����x�N�g�����擾
	float GetLen_W(int elem) { return m_fLength[elem]; } // �w�莲�����̒������擾
	DirectX::XMVECTOR GetPos_W() { return m_Pos; }

	void SetOBBParam_Pos(DirectX::XMMATRIX posMat);
	void SetOBBParam_Pos(DirectX::XMFLOAT3 pos);
	void SetOBBParam_Scl(DirectX::XMFLOAT3 scl);
	void SetOBBParam_Rot(DirectX::XMMATRIX rotMat);
};
