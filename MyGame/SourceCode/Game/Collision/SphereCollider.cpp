#include "SphereCollider.h"
//#include "Object3d.h"

using namespace DirectX;

void SphereCollider::Update()
{
	// ���[���h�s�񂩂���W�𒊏o
	const XMMATRIX& matWorld = object3d->GetMatWorld();

	// ���̃����o�ϐ����X�V
	center = matWorld.r[3] + offset;
	Sphere::radius = radius;
}