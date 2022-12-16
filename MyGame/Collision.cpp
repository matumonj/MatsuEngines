#include "Collision.h"//collision
bool Collision::ColFlag;

void Collision::SetCollideOBB(bool f) { ColFlag = f; }
bool Collision::GetCollideOBB() { return ColFlag; }

bool Collision::line2dCol(Line2D line, Point point)
{
	float len = (line.end.x - line.start.x);
}

/**
* @brief �����Ɖ~�̓����蔻��֐�
* @retval ture ������
* @retval false �������ĂȂ�
* @param[in] line �����f�[�^x
x* @param[in] circle �~�f�[�^
*/
float Collision::CalculationVertexLength(const XMFLOAT2& pos01, const XMFLOAT2& pos02)
{
	return sqrtf((pos01.x - pos02.x) * (pos01.x - pos02.x) + (pos01.y - pos02.y) * (pos01.y - pos02.y));
}

void Collision::ConvertToNomalizeVector(XMFLOAT2& out, XMFLOAT2 in)
{
	float distance = sqrtf((in.x * in.x) + (in.y * in.y));
	if (distance > 0.0f)
	{
		out.x = in.x / distance;
		out.y = in.y / distance;
	}
	else
	{
		out = XMFLOAT2(0.0f, 0.0f);
	}
}

float Collision::CalculationVectorLength(const XMFLOAT2& vec01)
{
	return sqrtf((vec01.x * vec01.x) + (vec01.y * vec01.y));
}

bool Collision::IsCollidingLineAndCircle(Line2D line, Point circle)
{
	// �x�N�g���̍쐬
	auto start_to_center = XMFLOAT2(circle.x - line.start.x, circle.y - line.start.y);
	auto end_to_center = XMFLOAT2(circle.x - line.end.x, circle.y - line.end.y);
	auto start_to_end = XMFLOAT2(line.end.x - line.start.x, line.end.y - line.start.y);
	XMFLOAT2 normal_start_to_end;

	// �P�ʃx�N�g��������
	ConvertToNomalizeVector(normal_start_to_end, start_to_end);

	/*
		�ˉe���������̒���
			�n�_�Ɖ~�̒��S�ŊO�ς��s��
			���n�_ => �I�_�̃x�N�g���͒P�ʉ����Ă���
	*/
	float distance_projection = start_to_center.x * normal_start_to_end.y - normal_start_to_end.x * start_to_center.y;

	// �ˉe�̒��������a����������
	if (fabs(distance_projection) < 40)
	{
		// �n�_ => �I�_�Ǝn�_ => �~�̒��S�̓��ς��v�Z����
		float dot01 = start_to_center.x * start_to_end.x + start_to_center.y * start_to_end.y;
		// �n�_ => �I�_�ƏI�_ => �~�̒��S�̓��ς��v�Z����
		float dot02 = end_to_center.x * start_to_end.x + end_to_center.y * start_to_end.y;

		// ��̓��ς̊|���Z���ʂ�0�ȉ��Ȃ瓖����
		if (dot01 * dot02 <= 0.0f)
		{
			return true;
		}
		/*
			��̏�������R�ꂽ�ꍇ�A�~�͐�����ɂ͂Ȃ��̂ŁA
			�n�_ => �~�̒��S�̒������A�I�_ => �~�̒��S�̒�����
			�~�̔��a�����Z�������瓖����
		*/
		if (CalculationVectorLength(start_to_center) < 40 ||
			CalculationVectorLength(end_to_center) < 40)
		{
			return true;
		}
	}

	return false;
}

bool Collision::CheckBox2Box(const Box& box1, const Box& box2)
{
	/*bool judg1 = box1.LUposition.x < box2.RBposition.x;
	bool judg2 = box2.LUposition.x < box1.RBposition.x;
	bool judg3 = box1.LUposition.y < box2.RUposition.y;
	bool judg4 = box2.LUposition.y < box1.RUposition.y;

	bool alljudg = judg1 && judg2 && judg3 && judg4;

	if (alljudg) {
		return true;
	}
	return false;*/

	return false;
}

float Collision::GetLength(XMFLOAT3 position, XMFLOAT3 position2)
{
	float len;
	len = sqrtf(
		(position.x - position2.x) * (position.x - position2.x) + (position.y - position2.y) * (position.y - position2.
			y) + (position.z - position2.z) * (position.z - position2.z));
	return len;
}

bool Collision::CheckPoint2Rect(const Point& point, const Box& rect)
{
	if (point.x >= rect.position.x && point.x <= (rect.position.x + rect.scale.x) &&
		point.y >= rect.position.y && point.y <= (rect.position.y + rect.scale.y))
	{
		return true;
	}
	return false;
}

float Collision::CollsionPoint2Line2D(const Point& point, const Line2D& lineseg)
{
	float lenY = lineseg.start.y - lineseg.end.y;
	float lenX = lineseg.end.x - lineseg.start.x;

	float lenXY = (-lenX * lineseg.start.y) + (-lenY * lineseg.start.x);

	float root = sqrtf(lenY * lenY + lenX * lenX);

	float len = ((lenY * point.x) + (lenX * point.y) + lenXY) / root;

	if (len < 0.0f)
	{
		len = -len;
	}

	return len;
}

void Collision::ClosestPtPoint2Triangle(const XMVECTOR& point, const Triangle& triangle, XMVECTOR* closest)
{
	// point��p0�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	XMVECTOR p0_p1 = triangle.p1 - triangle.p0;
	XMVECTOR p0_p2 = triangle.p2 - triangle.p0;
	XMVECTOR p0_pt = point - triangle.p0;

	float d1 = XMVector3Dot(p0_p1, p0_pt).m128_f32[0];
	float d2 = XMVector3Dot(p0_p2, p0_pt).m128_f32[0];

	if (d1 <= 0.0f && d2 <= 0.0f)
	{
		// p0���ŋߖT
		*closest = triangle.p0;
		return;
	}

	// point��p1�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	XMVECTOR p1_pt = point - triangle.p1;

	float d3 = XMVector3Dot(p0_p1, p1_pt).m128_f32[0];
	float d4 = XMVector3Dot(p0_p2, p1_pt).m128_f32[0];

	if (d3 >= 0.0f && d4 <= d3)
	{
		// p1���ŋߖT
		*closest = triangle.p1;
		return;
	}

	// point��p0_p1�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p0_p1��ɑ΂���ˉe��Ԃ�
	float vc = d1 * d4 - d3 * d2;
	if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f)
	{
		float v = d1 / (d1 - d3);
		*closest = triangle.p0 + v * p0_p1;
		return;
	}

	// point��p2�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	XMVECTOR p2_pt = point - triangle.p2;

	float d5 = XMVector3Dot(p0_p1, p2_pt).m128_f32[0];
	float d6 = XMVector3Dot(p0_p2, p2_pt).m128_f32[0];
	if (d6 >= 0.0f && d5 <= d6)
	{
		*closest = triangle.p2;
		return;
	}

	// point��p0_p2�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p0_p2��ɑ΂���ˉe��Ԃ�
	float vb = d5 * d2 - d1 * d6;
	if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f)
	{
		float w = d2 / (d2 - d6);
		*closest = triangle.p0 + w * p0_p2;
		return;
	}

	// point��p1_p2�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p1_p2��ɑ΂���ˉe��Ԃ�
	float va = d3 * d6 - d5 * d4;
	if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f)
	{
		float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
		*closest = triangle.p1 + w * (triangle.p2 - triangle.p1);
		return;
	}

	// point�͖ʗ̈�̒��ɂ���Bclosest���d�S���W��p���Čv�Z����
	float denom = 1.0f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;
	*closest = triangle.p0 + p0_p1 * v + p0_p2 * w;
}

bool Collision::CheckSphere2Sphere(const Sphere& sphereA, const Sphere& sphereB, XMVECTOR* inter, XMVECTOR* reject)
{
	// ���S�_�̋����̂Q�� <= ���a�̘a�̂Q��@�Ȃ����
	float dist = XMVector3LengthSq(sphereA.center - sphereB.center).m128_f32[0];

	float radius2 = sphereA.radius + sphereB.radius;
	radius2 *= radius2;

	if (dist <= radius2)
	{
		if (inter)
		{
			// A�̔��a��0�̎����W��B�̒��S�@B�̔��a��0�̎����W��A�̒��S�@�ƂȂ�悤�⊮
			float t = sphereB.radius / (sphereA.radius + sphereB.radius);
			*inter = XMVectorLerp(sphereA.center, sphereB.center, t);
		}
		// �����o���x�N�g�����v�Z
		if (reject)
		{
			float rejectLen = sphereA.radius + sphereB.radius - sqrtf(dist);
			*reject = XMVector3Normalize(sphereA.center - sphereB.center);
			*reject *= rejectLen;
		}
		return true;
	}

	return false;
}

bool Collision::CheckSphere2Triangle(const Sphere& sphere, const Triangle& triangle, XMVECTOR* inter, XMVECTOR* reject)
{
	XMVECTOR p;
	// ���̒��S�ɑ΂���ŋߐړ_�ł���O�p�`��ɂ���_p��������
	ClosestPtPoint2Triangle(sphere.center, triangle, &p);
	// �_p�Ƌ��̒��S�̍����x�N�g��
	XMVECTOR v = p - sphere.center;
	// �����̓������߂�
	//�i�����x�N�g�����m�̓��ς͎O�����̒藝�̃��[�g�����̎��ƈ�v����j
	float distanceSquare = XMVector3Dot(v, v).m128_f32[0];
	// ���ƎO�p�`�̋��������a�ȉ��Ȃ瓖�����Ă��Ȃ�
	if (distanceSquare > sphere.radius * sphere.radius)
	{
		return false;
	}
	// �[����_���v�Z
	if (inter)
	{
		// �O�p�`��̍ŋߐړ_p���^����_�Ƃ���
		*inter = p;
	}
	// �����o���x�N�g�����v�Z
	if (reject)
	{
		float ds = XMVector3Dot(sphere.center, triangle.normal).m128_f32[0];
		float dt = XMVector3Dot(triangle.p0, triangle.normal).m128_f32[0];
		float rejectLen = dt - ds + sphere.radius;
		*reject = triangle.normal * rejectLen;
	}
	return true;
}

bool Collision::CheckRay2Plane(const Ray& lay, const Plane& plane, float* distance, XMVECTOR* inter)
{
	const float epsilon = 1.0e-5f; // �덷�z���p�̔����Ȓl

	float d1 = XMVector3Dot(plane.normal, lay.dir).m128_f32[0];
	// ���ʂɂ͓�����Ȃ�
	if (d1 > -epsilon)
	{
		return false;
	}

	float d2 = XMVector3Dot(plane.normal, lay.start).m128_f32[0];
	float t = (plane.distance - d2) / d1;

	if (t < 0)
	{
		return false;
	}

	// ��������������
	if (distance)
	{
		*distance = t;
	}

	// ��_���v�Z
	if (inter)
	{
		*inter = lay.start + t * lay.dir;
	}

	return true;
}

bool Collision::CheckRay2Triangle(const Ray& lay, const Triangle& triangle, float* distance, XMVECTOR* inter)
{
	// �O�p�`������Ă��镽�ʂ��Z�o
	Plane plane;
	XMVECTOR interPlane;
	plane.normal = triangle.normal;
	plane.distance = XMVector3Dot(triangle.normal, triangle.p0).m128_f32[0];
	// ���C�ƕ��ʂ��������Ă��Ȃ���΁A�������Ă��Ȃ�	
	if (!CheckRay2Plane(lay, plane, distance, &interPlane))
	{
		return false;
	}
	// ���C�ƕ��ʂ��������Ă����̂ŁA�����ƌ�_���������܂ꂽ

	// ���C�ƕ��ʂ̌�_���O�p�`�̓����ɂ��邩����
	const float epsilon = 1.0e-5f; // �덷�z���p�̔����Ȓl
	XMVECTOR m;
	// ��p0_p1�ɂ���
	XMVECTOR pt_p0 = triangle.p0 - interPlane;
	XMVECTOR p0_p1 = triangle.p1 - triangle.p0;
	m = XMVector3Cross(pt_p0, p0_p1);
	// �ӂ̊O��
	if (XMVector3Dot(m, triangle.normal).m128_f32[0] < -epsilon)
	{
		return false;
	}

	// ��p1_p2�ɂ���
	XMVECTOR pt_p1 = triangle.p1 - interPlane;
	XMVECTOR p1_p2 = triangle.p2 - triangle.p1;
	m = XMVector3Cross(pt_p1, p1_p2);
	// �ӂ̊O��
	if (XMVector3Dot(m, triangle.normal).m128_f32[0] < -epsilon)
	{
		return false;
	}

	// ��p2_p0�ɂ���
	XMVECTOR pt_p2 = triangle.p2 - interPlane;
	XMVECTOR p2_p0 = triangle.p0 - triangle.p2;
	m = XMVector3Cross(pt_p2, p2_p0);
	// �ӂ̊O��
	if (XMVector3Dot(m, triangle.normal).m128_f32[0] < -epsilon)
	{
		return false;
	}

	if (inter)
	{
		*inter = interPlane;
	}

	// �����Ȃ̂ŁA�������Ă���
	return true;
}

bool Collision::CheckRay2Sphere(const Ray& lay, const Sphere& sphere, float* distance, XMVECTOR* inter)
{
	XMVECTOR m = lay.start - sphere.center;
	float b = XMVector3Dot(m, lay.dir).m128_f32[0];
	float c = XMVector3Dot(m, m).m128_f32[0] - sphere.radius * sphere.radius;
	// lay�̎n�_��sphere�̊O���ɂ���(c > 0)�Alay��sphere���痣��Ă���������
	// �����Ă���ꍇ(b > 0)�A������Ȃ�
	if (c > 0.0f && b > 0.0f)
	{
		return false;
	}

	float discr = b * b - c;
	// ���̔��ʎ��̓��C�������O��Ă��邱�ƂɈ�v
	if (discr < 0.0f)
	{
		return false;
	}

	// ���C�͋��ƌ������Ă���B
	// ��������ŏ��̒lt���v�Z
	float t = -b - sqrtf(discr);
	// t�����ł���ꍇ�A���C�͋��̓�������J�n���Ă���̂�t���[���ɃN�����v
	if (t < 0)
	{
		t = 0.0f;
	}

	if (distance)
	{
		*distance = t;
	}

	if (inter)
	{
		*inter = lay.start + t * lay.dir;
	}

	return true;
}

bool Collision::CheckOBBCollision(OBB& obb1, OBB& obb2)
{
	// �e�����x�N�g���̊m��
	// �iN***:�W���������x�N�g���j
	XMVECTOR NAe1 = obb1.GetDirect(0), Ae1 = {
		         NAe1.m128_f32[0] * obb1.GetLen_W(0), NAe1.m128_f32[1] * obb1.GetLen_W(0),
		         NAe1.m128_f32[2] * obb1.GetLen_W(0)
	         };
	XMVECTOR NAe2 = obb1.GetDirect(1), Ae2 = {
		         NAe2.m128_f32[0] * obb1.GetLen_W(1), NAe2.m128_f32[1] * obb1.GetLen_W(1),
		         NAe2.m128_f32[2] * obb1.GetLen_W(1)
	         };
	XMVECTOR NAe3 = obb1.GetDirect(2), Ae3 = {
		         NAe3.m128_f32[0] * obb1.GetLen_W(2), NAe3.m128_f32[1] * obb1.GetLen_W(2),
		         NAe3.m128_f32[2] * obb1.GetLen_W(2)
	         };
	XMVECTOR NBe1 = obb2.GetDirect(0), Be1 = {
		         NBe1.m128_f32[0] * obb2.GetLen_W(0), NBe1.m128_f32[1] * obb2.GetLen_W(0),
		         NBe1.m128_f32[2] * obb2.GetLen_W(0)
	         };
	XMVECTOR NBe2 = obb2.GetDirect(1), Be2 = {
		         NBe2.m128_f32[0] * obb2.GetLen_W(1), NBe2.m128_f32[1] * obb2.GetLen_W(1),
		         NBe2.m128_f32[2] * obb2.GetLen_W(1)
	         };
	XMVECTOR NBe3 = obb2.GetDirect(2), Be3 = {
		         NBe3.m128_f32[0] * obb2.GetLen_W(2), NBe3.m128_f32[1] * obb2.GetLen_W(2),
		         NBe3.m128_f32[2] * obb2.GetLen_W(2)
	         };
	XMVECTOR Interval = obb1.GetPos_W() - obb2.GetPos_W();

	// ������ : Ae1
	double rA = sqrt(
		(Ae1.m128_f32[0] * Ae1.m128_f32[0]) + (Ae1.m128_f32[1] * Ae1.m128_f32[1]) + (Ae1.m128_f32[2] * Ae1.m128_f32[
			2]));
	double rB = LenSegOnSeparateAxis(&NAe1, &Be1, &Be2, &Be3);
	double fL = fabs(
		(Interval.m128_f32[0] * NAe1.m128_f32[0]) + (Interval.m128_f32[1] * NAe1.m128_f32[1]) + (Interval.m128_f32[2] *
			NAe1.m128_f32[2]));
	if (fL > rA + rB)
	{
		return false;
	}
	// ������ : Ae2
	rA = sqrt(
		(Ae2.m128_f32[0] * Ae2.m128_f32[0]) + (Ae2.m128_f32[1] * Ae2.m128_f32[1]) + (Ae2.m128_f32[2] * Ae2.m128_f32[
			2]));
	rB = LenSegOnSeparateAxis(&NAe2, &Be1, &Be2, &Be3);
	fL = fabs(
		(Interval.m128_f32[0] * NAe2.m128_f32[0]) + (Interval.m128_f32[1] * NAe2.m128_f32[1]) + (Interval.m128_f32[2] *
			NAe2.m128_f32[2]));

	if (fL > rA + rB)
	{
		return false;
	}
	rA = sqrt(
		(Ae3.m128_f32[0] * Ae3.m128_f32[0]) + (Ae3.m128_f32[1] * Ae3.m128_f32[1]) + (Ae3.m128_f32[2] * Ae3.m128_f32[
			2]));
	rB = LenSegOnSeparateAxis(&NAe3, &Be1, &Be2, &Be3);
	fL = fabs(
		(Interval.m128_f32[0] * NAe3.m128_f32[0]) + (Interval.m128_f32[1] * NAe3.m128_f32[1]) + (Interval.m128_f32[2] *
			NAe3.m128_f32[2]));

	// ������ : Ae3
	if (fL > rA + rB)
	{
		return false;
	}
	// ������ : Be1
	rA = LenSegOnSeparateAxis(&NBe1, &Ae1, &Ae2, &Ae3);
	rB = sqrt(
		(Be1.m128_f32[0] * Be1.m128_f32[0]) + (Be1.m128_f32[1] * Be1.m128_f32[1]) + (Be1.m128_f32[2] * Be1.m128_f32[
			2]));
	fL = fabs(
		(Interval.m128_f32[0] * NBe1.m128_f32[0]) + (Interval.m128_f32[1] * NBe1.m128_f32[1]) + (Interval.m128_f32[2] *
			NBe1.m128_f32[2]));
	if (fL > rA + rB)
	{
		return false;
	}
	// ������ : Be2
	rA = LenSegOnSeparateAxis(&NBe2, &Ae1, &Ae2, &Ae3);
	rB = sqrt(
		(Be2.m128_f32[0] * Be2.m128_f32[0]) + (Be2.m128_f32[1] * Be2.m128_f32[1]) + (Be2.m128_f32[2] * Be2.m128_f32[
			2]));
	fL = fabs(
		(Interval.m128_f32[0] * NBe2.m128_f32[0]) + (Interval.m128_f32[1] * NBe2.m128_f32[1]) + (Interval.m128_f32[2] *
			NBe2.m128_f32[2]));
	if (fL > rA + rB)
	{
		return false;
	}
	// ������ : Be3
	rA = LenSegOnSeparateAxis(&NBe3, &Ae1, &Ae2, &Ae3);
	rB = sqrt(
		(Be3.m128_f32[0] * Be3.m128_f32[0]) + (Be3.m128_f32[1] * Be3.m128_f32[1]) + (Be3.m128_f32[2] * Be3.m128_f32[
			2]));
	fL = fabs(
		(Interval.m128_f32[0] * NBe3.m128_f32[0]) + (Interval.m128_f32[1] * NBe3.m128_f32[1]) + (Interval.m128_f32[2] *
			NBe3.m128_f32[2]));
	if (fL > rA + rB)
	{
		return false;
	}
	// ������ : C11
	XMVECTOR Cross;
	Cross = XMVector3Cross(NAe1, NBe1);
	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
	fL = fabs(
		(Interval.m128_f32[0] * Cross.m128_f32[0]) + (Interval.m128_f32[1] * Cross.m128_f32[1]) + (Interval.m128_f32[2]
			* Cross.m128_f32[2]));
	if (fL > rA + rB)
	{
		return false;
	}
	// ������ : C12
	Cross = XMVector3Cross(NAe1, NBe2);
	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
	fL = fabs(
		(Interval.m128_f32[0] * Cross.m128_f32[0]) + (Interval.m128_f32[1] * Cross.m128_f32[1]) + (Interval.m128_f32[2]
			* Cross.m128_f32[2]));

	if (fL > rA + rB)
	{
		return false;
	}
	// ������ : C13
	Cross = XMVector3Cross(NAe1, NBe3);
	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
	fL = fabs(
		(Interval.m128_f32[0] * Cross.m128_f32[0]) + (Interval.m128_f32[1] * Cross.m128_f32[1]) + (Interval.m128_f32[2]
			* Cross.m128_f32[2]));
	if (fL > rA + rB)
	{
		return false;
	}
	// ������ : C21
	Cross = XMVector3Cross(NAe2, NBe1);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
	fL = fabs(
		(Interval.m128_f32[0] * Cross.m128_f32[0]) + (Interval.m128_f32[1] * Cross.m128_f32[1]) + (Interval.m128_f32[2]
			* Cross.m128_f32[2]));

	if (fL > rA + rB)
	{
		return false;
	}
	// ������ : C22
	Cross = XMVector3Cross(NAe2, NBe2);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
	fL = fabs(
		(Interval.m128_f32[0] * Cross.m128_f32[0]) + (Interval.m128_f32[1] * Cross.m128_f32[1]) + (Interval.m128_f32[2]
			* Cross.m128_f32[2]));
	if (fL > rA + rB)
	{
		return false;
	}
	// ������ : C23
	Cross = XMVector3Cross(NAe2, NBe3);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
	fL = fabs(
		(Interval.m128_f32[0] * Cross.m128_f32[0]) + (Interval.m128_f32[1] * Cross.m128_f32[1]) + (Interval.m128_f32[2]
			* Cross.m128_f32[2]));

	if (fL > rA + rB)
	{
		return false;
	}
	// ������ : C31
	Cross = XMVector3Cross(NAe3, NBe1);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
	rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
	fL = fabs(
		(Interval.m128_f32[0] * Cross.m128_f32[0]) + (Interval.m128_f32[1] * Cross.m128_f32[1]) + (Interval.m128_f32[2]
			* Cross.m128_f32[2]));

	if (fL > rA + rB)
	{
		return false;
	}
	// ������ : C32
	Cross = XMVector3Cross(NAe3, NBe2);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
	//L = XMVector3Dot(Interval, Cross);
	fL = fabs(
		(Interval.m128_f32[0] * Cross.m128_f32[0]) + (Interval.m128_f32[1] * Cross.m128_f32[1]) + (Interval.m128_f32[2]
			* Cross.m128_f32[2]));

	if (fL > rA + rB)
	{
		return false;
	}
	// ������ : C33
	Cross = XMVector3Cross(NAe3, NBe3);

	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
	//L = XMVector3Dot(Interval, Cross);
	fL = fabs(
		(Interval.m128_f32[0] * Cross.m128_f32[0]) + (Interval.m128_f32[1] * Cross.m128_f32[1]) + (Interval.m128_f32[2]
			* Cross.m128_f32[2]));

	if (fL > rA + rB)
	{
		return false;
	}

	// �������ʂ����݂��Ȃ��̂Łu�Փ˂��Ă���v
	return true;
}


// �������ɓ��e���ꂽ���������瓊�e���������Z�o
double Collision::LenSegOnSeparateAxis(XMVECTOR* Sep, XMVECTOR* e1, XMVECTOR* e2, XMVECTOR* e3)
{
	// 3�̓��ς̐�Βl�̘a�œ��e���������v�Z
	// ������Sep�͕W��������Ă��邱��
	//XMVECTOR r1 = XMVector3Dot(Sep, e1);
	double fr1 = fabs(
		(Sep->m128_f32[0] * e1->m128_f32[0]) + (Sep->m128_f32[1] * e1->m128_f32[1]) + (Sep->m128_f32[2] * e1->m128_f32[
			2]));
	// FLOAT r2 = fabs(D3DXVec3Dot(Sep, e2));
	double fr2 = fabs(
		(Sep->m128_f32[0] * e2->m128_f32[0]) + (Sep->m128_f32[1] * e2->m128_f32[1]) + (Sep->m128_f32[2] * e2->m128_f32[
			2]));
	// FLOAT r3 = e3 ? (fabs(D3DXVec3Dot(Sep, e3))) : 0;
	double fr3 = e3
		             ? (fabs(
			             (Sep->m128_f32[0] * e3->m128_f32[0]) + (Sep->m128_f32[1] * e3->m128_f32[1]) + (Sep->m128_f32[2]
				             * e3->m128_f32[2])))
		             : 0;
	return fr1 + fr2 + fr3;
}
