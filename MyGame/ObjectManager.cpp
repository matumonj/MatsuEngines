#include "ObjectManager.h"
#include"SphereCollider.h"
#include"CollisionAttribute.h"
#include"TouchableObject.h"
#include"CollisionManager.h"
#include"Collision.h"
ObjectManager::~ObjectManager()
{
	delete m_Model;
	delete m_fbxModel;
}
void ObjectManager::SetCollider()
{
	float radius = 5.0f;
	m_Object->SetCollider(new SphereCollider(XMVECTOR({ 0,radius,0,0 }), radius));
	m_Object->collider->SetAttribute(COLLISION_ATTR_ALLIES);
}
void ObjectManager::CollisionField(DebugCamera* camera)
{
	// ���[���h�s��X�V
	m_Object->UpdateWorldMatrix();

	// ��������
	if (!onGround) {
		// �����������x
		const float fallAcc = -0.01f;
		const float fallVYMin = -0.5f;
		// ����
		fallV.m128_f32[1] = max(fallV.m128_f32[1] + fallAcc, fallVYMin);
		// �ړ�
		Position.y += fallV.m128_f32[1];
	}

	// ���[���h�s��X�V
	m_Object->UpdateWorldMatrix();
	m_Object->collider->Update();

	SphereCollider* sphereCollider = dynamic_cast<SphereCollider*>(m_Object->collider);
	assert(sphereCollider);

	// �N�G���[�R�[���o�b�N�N���X
	class PlayerQueryCallback : public QueryCallback
	{
	public:
		PlayerQueryCallback(Sphere* sphere) : sphere(sphere) {};

		// �Փˎ��R�[���o�b�N�֐�
		bool OnQueryHit(const QueryHit& info) {

			const XMVECTOR up = { 0,1,0,0 };

			XMVECTOR rejectDir = XMVector3Normalize(info.reject);
			float cos = XMVector3Dot(rejectDir, up).m128_f32[0];

			// �n�ʔ��肵�����l
			const float threshold = cosf(XMConvertToRadians(30.0f));

			if (-threshold < cos && cos < threshold) {
				sphere->center += info.reject;
				move += info.reject;
			}

			return true;
		}

		Sphere* sphere = nullptr;
		DirectX::XMVECTOR move = {};
	};

	PlayerQueryCallback callback(sphereCollider);

	// ���ƒn�`�̌�����S����
	CollisionManager::GetInstance()->QuerySphere(*sphereCollider, &callback, COLLISION_ATTR_LANDSHAPE);
	// �����ɂ��r�˕�������
	Position.x += callback.move.m128_f32[0];
	Position.y += callback.move.m128_f32[1];
	Position.z += callback.move.m128_f32[2];
	// ���[���h�s��X�V
	m_Object->UpdateWorldMatrix();
	m_Object->collider->Update();

	// ���̏�[���狅�̉��[�܂ł̃��C�L���X�g
	Ray ray;
	ray.start = sphereCollider->center;
	ray.start.m128_f32[1] += sphereCollider->GetRadius() +radius_adjustment;
	ray.dir = { 0,-1,0,0 };
	RaycastHit raycastHit;

	// �ڒn���
	if (onGround) {
		// �X���[�Y�ɍ������ׂ̋z������
		const float adsDistance = 5.0f;
		// �ڒn���ێ�
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.0f + adsDistance)) {
			onGround = true;
			Position.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
		}
		// �n�ʂ��Ȃ��̂ŗ���
		else {
			onGround = false;
			fallV = {};
		}
	}
	// �������

	else if (fallV.m128_f32[1] <= 0.0f) {
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.0f)) {
			// ���n
			onGround = true;
			Position.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
		}
	}
	// �s��̍X�V�Ȃ�
	m_Object->Update({ 1,1,1,1 }, camera);

}

void ObjectManager::ParameterSet_Obj(DebugCamera* camera)
{
	m_Object->SetPosition({ Position.x,Position.y-3,Position.z });
	m_Object->SetRotation(Rotation);
	m_Object->SetScale(Scale);
	m_Object->SetColor(Color);
	m_Object->Update({1,1,1,1},camera);
}


void ObjectManager::ParameterSet_Fbx(DebugCamera* camera)
{
	m_fbxObject->SetPosition({ Position.x,Position.y ,Position.z });

	m_fbxObject->SetRotation(Rotation);
	m_fbxObject->SetScale(Scale);

	m_Object->SetPosition(Position);
	//m_fbxObject->SetColor({1,1,1,1});
	m_fbxObject->Updata(true);
	
}

void ObjectManager::Draw_Obj()
{
	m_Object->PreDraw();
	m_Object->Draw();
	m_Object->PostDraw();
}

void ObjectManager::Draw_Fbx()
{
	m_fbxObject->Draw();
}