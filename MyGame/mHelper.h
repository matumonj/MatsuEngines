#pragma once
#include<math.h>
#define PI 3.14f

namespace Easing
{
	inline int EaseIn(int t)
	{
		return t * t;
	}

	inline float EaseOut(float t, float start, float end)
	{
		return (end - start) * (1.0f - cosf(t * PI / 2.0f)) + start;
	}

	inline float EaseInOut(int& t);
	inline float EaseOutIn(int& t);
}

namespace Percent
{
	inline float GetParcent(const float amount, const float num)
	{
		return num / amount * 100.00f;
	}
}

namespace MatCal
{
	inline DirectX::XMVECTOR WDivi(const DirectX::XMVECTOR& pos, const DirectX::XMMATRIX& mat, const bool s)
	{
		float x, y, z, w;

		x = (pos.m128_f32[0] * mat.r[0].m128_f32[0]) + (pos.m128_f32[1] * mat.r[1].m128_f32[0]) + (pos.m128_f32[2] * mat
			.r[
				2].m128_f32[0]) + (1.0f * mat.r[3].m128_f32[0]);
		y = (pos.m128_f32[0] * mat.r[0].m128_f32[1]) + (pos.m128_f32[1] * mat.r[1].m128_f32[1]) + (pos.m128_f32[2] * mat
			.r[
				2].m128_f32[1]) + (1.0f * mat.r[3].m128_f32[1]);
		z = (pos.m128_f32[0] * mat.r[0].m128_f32[2]) + (pos.m128_f32[1] * mat.r[1].m128_f32[2]) + (pos.m128_f32[2] * mat
			.r[
				2].m128_f32[2]) + (1.0f * mat.r[3].m128_f32[2]);

		w = pos.m128_f32[3];
		if (s)
		{
			w = z;
		}

		x = x / w;
		y = y / w;
		z = z / w;

		return DirectX::XMVECTOR{x, y, z, w};
	}

	inline DirectX::XMVECTOR WDivi(const DirectX::XMVECTOR& pos, const bool s)
	{
		float x, y, z, w;
		x = pos.m128_f32[0];
		y = pos.m128_f32[1];
		z = pos.m128_f32[2];
		w = pos.m128_f32[3];

		if (s)
		{
			w = z;
		}

		x = x / w;
		y = y / w;
		z = z / w;

		return DirectX::XMVECTOR{x, y, z, w};
	}

	inline DirectX::XMVECTOR PosDivi(const DirectX::XMVECTOR& pos, const DirectX::XMMATRIX& mat, const bool s)
	{
		float x, y, z, w;

		x = (pos.m128_f32[0] * mat.r[0].m128_f32[0]) + (pos.m128_f32[1] * mat.r[1].m128_f32[0]) + (pos.m128_f32[2] * mat
			.r[
				2].m128_f32[0]) + (1.0f * mat.r[3].m128_f32[0]);
		y = (pos.m128_f32[0] * mat.r[0].m128_f32[1]) + (pos.m128_f32[1] * mat.r[1].m128_f32[1]) + (pos.m128_f32[2] * mat
			.r[
				2].m128_f32[1]) + (1.0f * mat.r[3].m128_f32[1]);
		z = (pos.m128_f32[0] * mat.r[0].m128_f32[2]) + (pos.m128_f32[1] * mat.r[1].m128_f32[2]) + (pos.m128_f32[2] * mat
			.r[
				2].m128_f32[2]) + (1.0f * mat.r[3].m128_f32[2]);

		w = 1.0f;

		if (s)
		{
			w = z;
		}

		return DirectX::XMVECTOR{x, y, z, w};
	}
}


namespace FollowRot
{
	inline float FollowA_B(DirectX::XMVECTOR A,DirectX::XMVECTOR B)
	{
		//�v���C���[�ƓG�̃x�N�g���̒���(��)�����߂�
		DirectX::XMVECTOR SubVector = DirectX::XMVectorSubtract(B,A); // positionA - positionB;

		float RotY;
		//�p�x�̎擾 �v���C���[���G�̍��G�ʒu�ɓ�������������v���C���[�̕���
		RotY = atan2f(SubVector.m128_f32[0], SubVector.m128_f32[2]);

		//�߂�l��Ώۂ�Rot.Y�ɂ�����
		return RotY;
	}
}